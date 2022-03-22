// Jupiter Auroral Simulation
// GEANT4-11.0.0
// Gabriel Bridges Mar, 2021


#include "JupiterPrimaryGeneratorAction.hh"

#include "G4GeneralParticleSource.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JupiterPrimaryGeneratorAction::JupiterPrimaryGeneratorAction()
:G4VUserPrimaryGeneratorAction()
{
    GeneralParticleSource = new G4GeneralParticleSource();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JupiterPrimaryGeneratorAction::~JupiterPrimaryGeneratorAction()
{
	delete GeneralParticleSource;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JupiterPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    GeneralParticleSource -> GeneratePrimaryVertex(anEvent);
}
