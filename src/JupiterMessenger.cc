// Jupiter Auroral Simulation
// GEANT4-10.6.2
// Gabriel Bridges Aug, 2020

// Messenger Manager

//includes
// user headers
#include "JupiterMessenger.hh"
#include "ConstructJupiter.hh"
#include "global.h"
// Geant headers
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

JupiterMessenger::JupiterMessenger(ConstructJupiter* myDet)
:myDetector(myDet)
{
// output directry
  OutDirCmd = new G4UIcmdWithAString("/OutputDirectory", this);
  OutDirCmd->SetGuidance("Directory for output files");
  OutDirCmd->SetParameterName("OutputDirectory", true );
  OutDirCmd->SetDefaultValue("./tmp");

// output file
  OutFileCmd = new G4UIcmdWithAString("/OutputFile", this );
  OutFileCmd->SetGuidance("Name of output file");
  OutFileCmd->SetParameterName("OutputFile",true);
  OutFileCmd->SetDefaultValue("test");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JupiterMessenger::~JupiterMessenger()
{
	delete OutDirCmd;
  delete OutFileCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JupiterMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
 extern global_struct global;
	if(command == OutDirCmd) strcpy(global.outdir, newValue );
	if(command == OutFileCmd) strcpy(global.outfile, newValue );
}
