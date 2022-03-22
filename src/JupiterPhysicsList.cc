// Jupiter Auroral Simulation
// GEANT4-10.6.2
// Gabriel Bridges Aug, 2020

// Defines the physics lists

// Load GEANT Libraries
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4IonElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronInelasticQBBC.hh"
#include "G4NeutronTrackingCut.hh"
// Load User Libraries
#include "JupiterPhysicsList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
JupiterPhysicsList::JupiterPhysicsList():G4VModularPhysicsList()
{
    defaultCutValue = 0.7*CLHEP::mm;
    G4int ver = 1;
    SetVerboseLevel(ver);

    // EM Physics
    RegisterPhysics( new G4EmPenelopePhysics(ver) );

    // Synchroton Radiation & GN Physics
    RegisterPhysics( new G4EmExtraPhysics(ver) );

    // Decays
    RegisterPhysics( new G4DecayPhysics(ver) );

    // Hadron Physics
    RegisterPhysics( new G4HadronElasticPhysicsXS(ver) );

    RegisterPhysics( new G4StoppingPhysics(ver) );

    RegisterPhysics( new G4IonPhysicsXS(ver) );

    RegisterPhysics( new G4IonElasticPhysics(ver) );

    RegisterPhysics( new G4HadronInelasticQBBC(ver));

    // Neutron tracking cut
    RegisterPhysics( new G4NeutronTrackingCut(ver) );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
JupiterPhysicsList::~JupiterPhysicsList()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void JupiterPhysicsList::SetCuts()
{
    // Use default cut values gamma and e processes
    SetCutsWithDefault();
}
