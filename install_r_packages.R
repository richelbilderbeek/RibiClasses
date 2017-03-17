# Installs the R packages needed
lib_path <- "~/R"

install.packages("devtools", repos = "http://cran.uk.r-project.org", lib = lib_path)

# v1.2, does not support Endemic&NonEndemic yet
install.packages("DAISIE", repos = "http://cran.uk.r-project.org", lib = lib_path)
# v1.3, does support Endemic&NonEndemic, but is closed
# devtools::install_github("richelbilderbeek/DAISIE_1_3", subdir = "DAISIE")

devtools::install_github("richelbilderbeek/nLTT", lib = lib_path)
devtools::install_github("rsetienne/PBD", subdir = "PBD", lib = lib_path)
warnings()

#packageVersion("DAISIE")
packageVersion("devtools")
packageVersion("nLTT")
packageVersion("PBD")
