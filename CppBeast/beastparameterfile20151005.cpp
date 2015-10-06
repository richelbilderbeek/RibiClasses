#include "beastparameterfile.h"

#include <cassert>
#include <sstream>

#include "dnasequence.h"

std::vector<std::string> ribi::BeastParameterFile::ToBirthDeathXml20151005() const noexcept
{
  // Birth death model
  std::vector<std::string> v;
  v.push_back("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?><beast beautitemplate='Standard' beautistatus='' namespace=\"beast.core:beast.evolution.alignment:beast.evolution.tree.coalescent:beast.core.util:beast.evolution.nuc:beast.evolution.operators:beast.evolution.sitemodel:beast.evolution.substitutionmodel:beast.evolution.likelihood\" version=\"2.0\">");
  v.push_back("");
  v.push_back("");
  const std::string filename_base{m_alignment_base_filename}; //Probably the alignment filename
  // I use the same (non-)indentation as BEAUti
  v.push_back("    <data");
  v.push_back("id=\"" + filename_base + "\"");
  v.push_back("name=\"alignment\">");
  for (const auto& p: m_sequences)
  {
    std::stringstream nextline;
    nextline
      << "                    <sequence id=\"seq_"
      << p.GetDescription()
      << "\" taxon=\""
      << p.GetDescription()
      << "\" totalcount=\"4\" value=\""
      << p.GetSequence()
      << "\"/>";
    ;
    v.push_back(nextline.str());
  }
  v.push_back("                </data>");
  v.push_back("");
  v.push_back("");
  v.push_back("    ");
  v.push_back("");
  v.push_back("");
  v.push_back("    ");
  v.push_back("");
  v.push_back("");
  v.push_back("    ");
  v.push_back("<map name=\"Uniform\">beast.math.distributions.Uniform</map>");
  v.push_back("<map name=\"Exponential\">beast.math.distributions.Exponential</map>");
  v.push_back("<map name=\"LogNormal\">beast.math.distributions.LogNormalDistributionModel</map>");
  v.push_back("<map name=\"Normal\">beast.math.distributions.Normal</map>");
  v.push_back("<map name=\"Beta\">beast.math.distributions.Beta</map>");
  v.push_back("<map name=\"Gamma\">beast.math.distributions.Gamma</map>");
  v.push_back("<map name=\"LaplaceDistribution\">beast.math.distributions.LaplaceDistribution</map>");
  v.push_back("<map name=\"prior\">beast.math.distributions.Prior</map>");
  v.push_back("<map name=\"InverseGamma\">beast.math.distributions.InverseGamma</map>");
  v.push_back("<map name=\"OneOnX\">beast.math.distributions.OneOnX</map>");
  v.push_back("");
  v.push_back("");
  const std::string mcmc_chainlength_str{std::to_string(m_mcmc_chainlength)};
  assert(std::stoi(mcmc_chainlength_str) == m_mcmc_chainlength);
  v.push_back("<run id=\"mcmc\" spec=\"MCMC\" chainLength=\"" + mcmc_chainlength_str + "\">");
  v.push_back("    <state id=\"state\" storeEvery=\"5000\">");
  v.push_back("        <tree id=\"Tree.t:" + filename_base + "\" name=\"stateNode\">");
  v.push_back("            <taxonset id=\"TaxonSet." + filename_base + "\" spec=\"TaxonSet\">");
  v.push_back("                <alignment idref=\"" + filename_base + "\"/>");
  v.push_back("            </taxonset>");
  v.push_back("        </tree>");
  if (GetTreePrior() == ribi::TreePrior::birth_death)
  {
    v.push_back("        <parameter id=\"birthRate2.t:" + filename_base + "\" lower=\"0.0\" name=\"stateNode\" upper=\"10000.0\">1.0</parameter>");
    v.push_back("        <parameter id=\"relativeDeathRate2.t:" + filename_base + "\" lower=\"0.0\" name=\"stateNode\" upper=\"1.0\">0.5</parameter>");
  }
  else
  {
    assert(GetTreePrior() == ribi::TreePrior::coalescent_constant_population);
    v.push_back("        <parameter id=\"popSize.t:" + filename_base + "\" name=\"stateNode\">0.3</parameter>");
  }
  v.push_back("    </state>");
  v.push_back("");
  v.push_back("    <init id=\"RandomTree.t:" + filename_base + "\" spec=\"beast.evolution.tree.RandomTree\" estimate=\"false\" initial=\"@Tree.t:" + filename_base + "\" taxa=\"@" + filename_base + "\">");
  v.push_back("        <populationModel id=\"ConstantPopulation0.t:" + filename_base + "\" spec=\"ConstantPopulation\">");
  v.push_back("            <parameter id=\"randomPopSize.t:" + filename_base + "\" name=\"popSize\">1.0</parameter>");
  v.push_back("        </populationModel>");
  v.push_back("    </init>");
  v.push_back("");
  v.push_back("    <distribution id=\"posterior\" spec=\"util.CompoundDistribution\">");
  v.push_back("        <distribution id=\"prior\" spec=\"util.CompoundDistribution\">");
  if (GetTreePrior() == ribi::TreePrior::birth_death)
  {
    v.push_back("            <distribution id=\"BirthDeath.t:" + filename_base + "\" spec=\"beast.evolution.speciation.BirthDeathGernhard08Model\" birthDiffRate=\"@birthRate2.t:" + filename_base + "\" relativeDeathRate=\"@relativeDeathRate2.t:" + filename_base + "\" tree=\"@Tree.t:" + filename_base + "\"/>");
  }
  else
  {
    assert(GetTreePrior() == ribi::TreePrior::coalescent_constant_population);
    v.push_back("            <distribution id=\"CoalescentConstant.t:" + filename_base + "\" spec=\"Coalescent\">");
    v.push_back("                <populationModel id=\"ConstantPopulation.t:" + filename_base + "\" popSize=\"@popSize.t:" + filename_base + "\" spec=\"ConstantPopulation\"/>");
    v.push_back("                <treeIntervals id=\"TreeIntervals.t:" + filename_base + "\" spec=\"TreeIntervals\" tree=\"@Tree.t:" + filename_base + "\"/>");
    v.push_back("            </distribution>");
  }
  if (GetTreePrior() == ribi::TreePrior::birth_death)
  {
    v.push_back("            <prior id=\"BirthRatePrior.t:" + filename_base + "\" name=\"distribution\" x=\"@birthRate2.t:" + filename_base + "\">");
    v.push_back("                <Uniform id=\"Uniform.0\" name=\"distr\" upper=\"1000.0\"/>");
    v.push_back("            </prior>");
    v.push_back("            <prior id=\"DeathRatePrior.t:" + filename_base + "\" name=\"distribution\" x=\"@relativeDeathRate2.t:" + filename_base + "\">");
    v.push_back("                <Uniform id=\"Uniform.01\" name=\"distr\"/>");
  }
  else
  {
    assert(GetTreePrior() == ribi::TreePrior::coalescent_constant_population);
    v.push_back("            <prior id=\"PopSizePrior.t:" + filename_base + "\" name=\"distribution\" x=\"@popSize.t:" + filename_base + "\">");
    v.push_back("                <OneOnX id=\"OneOnX.0\" name=\"distr\"/>");
  }
  v.push_back("            </prior>");
  v.push_back("        </distribution>");
  v.push_back("        <distribution id=\"likelihood\" spec=\"util.CompoundDistribution\">");
  v.push_back("            <distribution id=\"treeLikelihood." + filename_base + "\" spec=\"TreeLikelihood\" data=\"@" + filename_base + "\" tree=\"@Tree.t:" + filename_base + "\">");
  v.push_back("                <siteModel id=\"SiteModel.s:" + filename_base + "\" spec=\"SiteModel\">");
  v.push_back("                    <parameter id=\"mutationRate.s:" + filename_base + "\" estimate=\"false\" name=\"mutationRate\">1.0</parameter>");
  v.push_back("                    <parameter id=\"gammaShape.s:" + filename_base + "\" estimate=\"false\" name=\"shape\">1.0</parameter>");
  v.push_back("                    <parameter id=\"proportionInvariant.s:" + filename_base + "\" estimate=\"false\" lower=\"0.0\" name=\"proportionInvariant\" upper=\"1.0\">0.0</parameter>");
  v.push_back("                    <substModel id=\"JC69.s:" + filename_base + "\" spec=\"JukesCantor\"/>");
  v.push_back("                </siteModel>");
  v.push_back("                <branchRateModel id=\"StrictClock.c:" + filename_base + "\" spec=\"beast.evolution.branchratemodel.StrictClockModel\">");
  v.push_back("                    <parameter id=\"clockRate.c:" + filename_base + "\" estimate=\"false\" name=\"clock.rate\">1.0</parameter>");
  v.push_back("                </branchRateModel>");
  v.push_back("            </distribution>");
  v.push_back("        </distribution>");
  v.push_back("    </distribution>");
  v.push_back("");
  v.push_back("    <operator id=\"treeScaler.t:" + filename_base + "\" spec=\"ScaleOperator\" scaleFactor=\"0.5\" tree=\"@Tree.t:" + filename_base + "\" weight=\"3.0\"/>");
  v.push_back("");
  v.push_back("    <operator id=\"treeRootScaler.t:" + filename_base + "\" spec=\"ScaleOperator\" rootOnly=\"true\" scaleFactor=\"0.5\" tree=\"@Tree.t:" + filename_base + "\" weight=\"3.0\"/>");
  v.push_back("");
  v.push_back("    <operator id=\"UniformOperator.t:" + filename_base + "\" spec=\"Uniform\" tree=\"@Tree.t:" + filename_base + "\" weight=\"30.0\"/>");
  v.push_back("");
  v.push_back("    <operator id=\"SubtreeSlide.t:" + filename_base + "\" spec=\"SubtreeSlide\" tree=\"@Tree.t:" + filename_base + "\" weight=\"15.0\"/>");
  v.push_back("");
  v.push_back("    <operator id=\"narrow.t:" + filename_base + "\" spec=\"Exchange\" tree=\"@Tree.t:" + filename_base + "\" weight=\"15.0\"/>");
  v.push_back("");
  v.push_back("    <operator id=\"wide.t:" + filename_base + "\" spec=\"Exchange\" isNarrow=\"false\" tree=\"@Tree.t:" + filename_base + "\" weight=\"3.0\"/>");
  v.push_back("");
  v.push_back("    <operator id=\"WilsonBalding.t:" + filename_base + "\" spec=\"WilsonBalding\" tree=\"@Tree.t:" + filename_base + "\" weight=\"3.0\"/>");
  v.push_back("");
  if (GetTreePrior() == ribi::TreePrior::birth_death)
  {
  v.push_back("    <operator id=\"BirthRateScaler.t:" + filename_base + "\" spec=\"ScaleOperator\" parameter=\"@birthRate2.t:" + filename_base + "\" scaleFactor=\"0.75\" weight=\"3.0\"/>");
  v.push_back("");
  v.push_back("    <operator id=\"DeathRateScaler.t:" + filename_base + "\" spec=\"ScaleOperator\" parameter=\"@relativeDeathRate2.t:" + filename_base + "\" scaleFactor=\"0.75\" weight=\"3.0\"/>");
  }
  else
  {
    assert(GetTreePrior() == ribi::TreePrior::coalescent_constant_population);
    v.push_back("    <operator id=\"PopSizeScaler.t:" + filename_base + "\" parameter=\"@popSize.t:" + filename_base + "\" scaleFactor=\"0.75\" spec=\"ScaleOperator\" weight=\"3.0\"/>");
  }
  v.push_back("");
  v.push_back("    <logger id=\"tracelog\" fileName=\"" + filename_base + ".log\" logEvery=\"1000\" model=\"@posterior\" sanitiseHeaders=\"true\" sort=\"smart\">");
  v.push_back("        <log idref=\"posterior\"/>");
  v.push_back("        <log idref=\"likelihood\"/>");
  v.push_back("        <log idref=\"prior\"/>");
  v.push_back("        <log idref=\"treeLikelihood." + filename_base + "\"/>");
  v.push_back("        <log id=\"TreeHeight.t:" + filename_base + "\" spec=\"beast.evolution.tree.TreeHeightLogger\" tree=\"@Tree.t:" + filename_base + "\"/>");
  //v.push_back("        <log idref=\"RBcount.s:" + filename_base + "\"/>");
  //v.push_back("        <parameter idref=\"RBrates.s:" + filename_base + "\" name=\"log\"/>");
  if (GetTreePrior() == ribi::TreePrior::birth_death)
  {
    v.push_back("        <log idref=\"BirthDeath.t:" + filename_base + "\"/>");
    v.push_back("        <log idref=\"birthRate2.t:" + filename_base + "\"/>");
    v.push_back("        <log idref=\"relativeDeathRate2.t:" + filename_base + "\"/>");
  }
  else
  {
    assert(GetTreePrior() == ribi::TreePrior::coalescent_constant_population);
    v.push_back("        <parameter idref=\"popSize.t:" + filename_base + "\" name=\"log\"/>");
    v.push_back("        <log idref=\"CoalescentConstant.t:" + filename_base + "\"/>");
  }
  v.push_back("    </logger>");
  v.push_back("");
  v.push_back("    <logger id=\"screenlog\" logEvery=\"1000\">");
  v.push_back("        <log idref=\"posterior\"/>");
  v.push_back("        <log id=\"ESS.0\" spec=\"util.ESS\" arg=\"@posterior\"/>");
  //v.push_back("        <log arg=\"@posterior\" id=\"ESS.0\" spec=\"util.ESS\"/>");
  v.push_back("        <log idref=\"likelihood\"/>");
  v.push_back("        <log idref=\"prior\"/>");
  v.push_back("    </logger>");
  v.push_back("");
  v.push_back("    <logger id=\"treelog.t:" + filename_base + "\" fileName=\"$(tree).trees\" logEvery=\"1000\" mode=\"tree\">");
  v.push_back("        <log id=\"TreeWithMetaDataLogger.t:" + filename_base + "\" spec=\"beast.evolution.tree.TreeWithMetaDataLogger\" tree=\"@Tree.t:" + filename_base + "\"/>");
  v.push_back("    </logger>");
  v.push_back("");
  v.push_back("</run>");
  v.push_back("");
  v.push_back("</beast>");
  return v;

}

