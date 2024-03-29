#include "birthdeathmodelhelper.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include <boost/math/constants/constants.hpp>

#include "RInside.h"
#include "fileio.h"
#include "birthdeathmodelparameters.h"
#include "phylogeny_r.h"
#include "ribi_rinside.h"

ribi::bdm::Helper::Helper()
{

}

double ribi::bdm::Helper::CalcLogLikelihood(
  const std::string& newick,
  const Rate& birth_rate,
  const Rate& death_rate
) const
{
  return CalcLogLikelihoodLaser(
    newick,
    birth_rate,
    death_rate
  );
}

double ribi::bdm::Helper::CalcLogLikelihoodDdd(
  const std::string& newick,
  const Rate& birth_rate,
  const Rate& death_rate,
  const Part part
) const
{
  assert(!newick.empty());
  auto& r = ribi::Rinside().Get();

  r.parseEval("library(ape)");
  r.parseEval("library(DDD)");
  r["newick"] = newick;
  r.parseEval("phylogeny <- read.tree(text = newick)");
  r.parseEval("branch_lengths <- phylogeny$edge.length");
  r["lambda"] = birth_rate.value();
  r["mu"] = death_rate.value();

  // model of time-dependence:
  // - 0: no time dependence
  // - 1: speciation and/or extinction rate is exponentially declining with time
  // - 2: stepwise decline in speciation rate as in diversity-dependence without extinction
  // - 3: decline in speciation rate following deterministic logistic equation for ddmodel = 1
  r["model_of_time_dependence"] = 0;

  // conditioning:
  // - 0: conditioning on stem or crown age
  // - 1: conditioning on stem or crown age and non-extinction of the phylogeny
  // - 2: conditioning on stem or crown age and on the total number of extant taxa (including missing species)
  // - 3: conditioning on the total number of extant taxa (including missing species)
  r["conditioning"] = 1;

  // Likelihood of what:
  // - 0: branching times
  // - 1: the phylogeny
  switch (part)
  {
    case Part::branch_lengths: r["likelihood_of_what"] = 0; break;
    case Part::phylogeny     : r["likelihood_of_what"] = 1; break;
  }

  // Show parameters and likelihood on screen:
  // - 0: no
  // - 1: yes
  r["show_parameters_and_likelihood_on_screen"] = 0;

  // first data point is:
  // - 1: stem age
  // - 2: crown age
  r["first_data_point_is"] = 2;

  r.parseEval(
    "log_likelihood_ddd <- bd_loglik("
    "  pars1 = c(lambda,mu),"
    "  pars2 = c("
    "    model_of_time_dependence,"
    "    conditioning,"
    "    likelihood_of_what,"
    "    show_parameters_and_likelihood_on_screen,"
    "    first_data_point_is"
    "  ),"
    "  brts=branch_lengths,"
    "  missnumspec = 0"
    ")"
  );

  Rcpp::DoubleVector v = r["log_likelihood_ddd"];
  assert(v.size() == 1);
  return v[0];
}

double ribi::bdm::Helper::CalcLogLikelihoodLaser(
  const std::string& newick,
  const Rate& birth_rate,
  const Rate& death_rate
) const
{
  assert(!newick.empty());
  auto& r = ribi::Rinside().Get();

  r.parseEval("library(ape)");
  r.parseEval("library(laser)");
  r["newick"] = newick;
  r.parseEval("phylogeny <- read.tree(text = newick)");
  r.parseEval("branch_lengths <- phylogeny$edge.length");
  r["lambda"] = birth_rate.value();
  r["mu"] = death_rate.value();
  Rcpp::DoubleVector v
    = r.parseEval("calcLHbd(x = branch_lengths, r = lambda - mu, a = mu / lambda)"
  );
  assert(v.size() == 1);
  return v[0];
}


void ribi::bdm::Helper::CalcMaxLikelihood(
  const std::string& newick,
  Rate& birth_rate,
  Rate& death_rate,
  const Part part
) const
{
  assert(!newick.empty());
  auto& r = ribi::Rinside().Get();

  r.parseEval("library(ape)");
  r.parseEval("library(DDD)");
  r["newick"] = newick;
  r.parseEval("phylogeny <- read.tree(text = newick)");
  r.parseEval("branch_lengths <- phylogeny$edge.length");

  switch (part)
  {
    case Part::branch_lengths: r["part"] = 0; break;
    case Part::phylogeny     : r["part"] = 1; break;
  }

  r.parseEval(
    "max_likelihood <- bd_ML("
    "  brts = branch_lengths,"
    "  cond = 1," // # cond == 1 : conditioning on stem or crown age and non-extinction of the phylogeny
    "  btorph = part"
    ")"
  );

  r.parseEval("lambda0 <- max_likelihood$lambda0");
  r.parseEval("mu0 <- max_likelihood$mu0");
  r.parseEval("conv <- max_likelihood$conv");
  const Rcpp::DoubleVector lambda0 = r["lambda0"];
  const Rcpp::DoubleVector mu0 = r["mu0"];
  const Rcpp::IntegerVector conv = r["conv"];
  assert(lambda0.size() == 1);
  assert(mu0.size() == 1);
  assert(conv.size() == 1);
  const bool has_converged = conv[0] == 0;
  if (!has_converged)
  {
    std::stringstream s;
    s << __func__ << ": has not converged" ;
    throw std::runtime_error(s.str());
  }
  birth_rate = lambda0[0] / boost::units::si::second;
  death_rate = mu0[0] / boost::units::si::second;
}

std::string ribi::bdm::Helper::CreateSimulatedPhylogeny(
  const Parameters& parameters
) const
{
  ribi::fileio::FileIo f;
  auto& r = ribi::Rinside().Get();

  r.parseEvalQ("library(ape)");
  r.parseEvalQ("library(geiger)");
  const double birth_rate{parameters.GetBirthRate().value()};
  const double death_rate{parameters.GetDeathRate().value()};
  const int n_taxa{parameters.GetNumberOfTaxa()};
  const int rng_seed{parameters.GetRngSeed()};
  const double t_end{parameters.GetTime().value()};

  assert(t_end == 0 && "Not yet implemented running to a certain time");

  r["birth_rate"] = birth_rate;
  r["death_rate"] = death_rate;
  r["n_taxa"] = n_taxa;
  r["rng_seed"] = rng_seed;
  r["t_end"] = t_end;
  // /home/richel/R/i686-pc-linux-gnu-library/3.1/Rcpp/include/Rcpp/vector/Vector.h
  assert(Rcpp::DoubleVector(r["birth_rate"])[0] == birth_rate);
  assert(Rcpp::DoubleVector(r["death_rate"])[0] == death_rate);
  assert(Rcpp::IntegerVector(r["n_taxa"])[0] == n_taxa);
  assert(Rcpp::IntegerVector(r["rng_seed"])[0] == rng_seed);
  assert(Rcpp::DoubleVector(r["t_end"])[0] == t_end);

  //Vital! If forgotten, the branch lengths will become N/A or 0
  std::setlocale(LC_ALL,"en_US.UTF-8");

  r.parseEvalQ("tree_full <- sim.bdtree(b=birth_rate,d=death_rate,stop=\"taxa\",n=n_taxa,extinct=FALSE,seed=rng_seed)");
  const Rcpp::String s = r.parseEval("write.tree(tree_full)");
  return std::string(s);
}
