#include <QApplication>

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#define BOOST_TEST_MODULE ribi_classes_test_desktop_module

bool initialise() { return true; }

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  ::boost::unit_test::unit_test_main(&initialise, argc, argv);
}
