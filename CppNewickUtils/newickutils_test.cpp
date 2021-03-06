#include "newickutils.h"

#include <cassert>

#include "fileio.h"
#include "phylogeny_r.h"
#include "testtimer.h"
#include "trace.h"

#ifndef NDEBUG
void ribi::NewickUtils::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    PhylogenyR();
  }
  const ribi::TestTimer test_timer(__func__,__FILE__,1.0);
  const bool verbose{false};
  if (verbose) { TRACE("GetPhylogeny"); }
  {
    const std::string newick{"(1,2);"};
    std::vector<std::string> v{NewickUtils().GetPhylogeny(newick)};
    assert(!v.empty());
  }
  if (verbose) { TRACE("DropExtinct"); }
  {
    assert(
      NewickUtils().DropExtinct(
           "(L:1,(((((XD:1,ZD:1):1,CE:2):1,(FE:2,EE:2):1):1,(GD:1,ID:1):1,BD:1):3,(AC:1,EC:1):1,(((TC:1,FD:2):1,QC:1,RC:1):1,((((AE:1,BE:1):1,(WD:1,YD:1):1):1,HD:1):2,MC:1):1):1):3);"
      ) == "((((XD:1,ZD:1):1,CE:2):1,(FE:2,EE:2):1):4,((AE:1,BE:1):1,(WD:1,YD:1):1):5);"
    );
  }
  if (verbose) { TRACE("NewickToPhylogeny on easy phylogeny"); }
  {
    const std::string temp_svg_filename{
      ribi::fileio::FileIo().GetTempFileName(".svg")
    };
    const bool plot_fossils{true};
    const std::string newick{"((F:2,G:2):1,H:3);"};
    NewickUtils().NewickToPhylogeny(
      newick,
      temp_svg_filename,
      NewickUtils::GraphicsFormat::svg,
      plot_fossils
    );
    assert(ribi::fileio::FileIo().IsRegularFile(temp_svg_filename));
    //Clean up
    ribi::fileio::FileIo().DeleteFile(temp_svg_filename.c_str());
  }
  if (verbose) { TRACE("NewickToPhylogeny on hard phylogeny"); }
  //ISSUE_267 does affect PhylogenyR, but not this class
  {
    const std::string newick{
      "(L:1,(((((XD:1,ZD:1):1,CE:2):1,(FE:2,EE:2):1):1,(GD:1,ID:1):1,BD:1):3,(AC:1,EC:1):1,(((TC:1,FD:2):1,QC:1,RC:1):1,((((AE:1,BE:1):1,(WD:1,YD:1):1):1,HD:1):2,MC:1):1):1):3);"
    };
    for (bool plot_fossils: {true,false})
    {
      const std::string temp_svg_filename{
        ribi::fileio::FileIo().GetTempFileName(".svg")
      };
      NewickUtils().NewickToPhylogeny(newick,temp_svg_filename,
        NewickUtils::GraphicsFormat::svg,
        plot_fossils
      );
      assert(ribi::fileio::FileIo().IsRegularFile(temp_svg_filename));
      ribi::fileio::FileIo().DeleteFile(temp_svg_filename.c_str());
    }
  }
  #ifdef FIX_ISSUE_269
  //NewickToPhylogeny as PNG
  {
    const std::string temp_png_filename{
      ribi::fileio::FileIo().GetTempFileName(".png")
    };
    const bool plot_fossils{false};
    const std::string newick{"((F:2,G:2):1,H:3);"};
    NewickUtils().NewickToPhylogeny(
      newick,
      temp_png_filename,
      NewickUtils::GraphicsFormat::png,
      plot_fossils
    );
    assert(ribi::fileio::FileIo().IsRegularFile(temp_png_filename));
    //Clean up
    ribi::fileio::FileIo().DeleteFile(temp_png_filename.c_str());
  }
  #endif // FIX_ISSUE_269
}
#endif

