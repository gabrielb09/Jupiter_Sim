

#include "JupiterPrimaryGeneratorAction.hh"
#include "ConstructJupiter.hh"

#include "G4GeneralParticleSource.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SingleParticleSource.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include <math.h>
#include "global.h"
#include "G4SystemOfUnits.hh"
//#include "Randomize.hh"


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
	extern global_struct global;
  GeneralParticleSource -> GeneratePrimaryVertex(anEvent);
}
