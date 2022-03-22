// Jupiter Auroral Simulation
// GEANT4-11.0.0
// Gabriel Bridges Mar, 2021

// Messenger Manager

// Load GEANT Libraries
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
// Load User Libraries
#include "JupiterMessenger.hh"
#include "ConstructJupiter.hh"
#include "global.h"

JupiterMessenger::JupiterMessenger()
{
    // output directry
    OutDirCmd = new G4UIcmdWithAString("/OutputDirectory", this);
    OutDirCmd -> SetGuidance("Directory for output files");
    OutDirCmd -> SetParameterName("OutputDirectory", true );
    OutDirCmd -> SetDefaultValue("./tmp");

    // output file
    OutFileCmd = new G4UIcmdWithAString("/OutputFile", this);
    OutFileCmd -> SetGuidance("Name of output file");
    OutFileCmd -> SetParameterName("OutputFile",true);
    OutFileCmd -> SetDefaultValue("test");
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
	if(command == OutDirCmd) strcpy(global.outdir, newValue);
	if(command == OutFileCmd) strcpy(global.outfile, newValue);
}
