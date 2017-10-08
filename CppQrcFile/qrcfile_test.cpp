#include "qrcfile.h"

#include <fstream>
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(ribi_qrcfile_basic_use)
{
  //Create a Qt resource file
  {
    std::ofstream f("tmp.txt");
    f << "<RCC>"
      << "    <qresource prefix=\"/images\">"
      << "        <file>Background.png</file>"
      << "        <file>Groups.png</file>"
      << "        <file>Cycle_100.png</file>"
      << "        <file>Experiment_100.png</file>"
      << "        <file>Period_100_58.png</file>"
      << "        <file>ProjectGtstParticipantDialogChatParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogChooseActionParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogFinishedParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogGroupAssign2Parameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogGroupAssignParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogGroupReAssignParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogLoggedInParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogViewResultsGroupParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogNotLoggedInParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogVotingParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogViewResultsVotingParameters.png</file>"
      << "        <file>ProjectGtstParticipantDialogAssignPayoffParameters.png</file>"
      << "    </qresource>"
      << "</RCC>";
  }
  //Check the project file
  const QrcFile p("tmp.txt");
  assert(p.GetFiles().count("Period_100_58.png"));
  assert(p.GetFiles().count("ProjectGtstParticipantDialogAssignPayoffParameters.png"));
}

