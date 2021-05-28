// Jupiter Auroral Simulation
// GEANT4-10.6.2
// Gabriel Bridges Aug, 2020

// Defines the physics lists

#include "JupiterPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4NeutronTrackingCut.hh"

#include "G4HadronPhysicsQGSP_BERT.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
JupiterPhysicsList::JupiterPhysicsList():  G4VModularPhysicsList()
{
    G4cout << "<<< Geant4 Physics List simulation engine: QGSP_BERT 3.4"<<G4endl;
    G4cout <<G4endl;


    defaultCutValue = 0.7*CLHEP::mm;
    G4int ver = 1;
    SetVerboseLevel(ver);

    // EM Physics
    //RegisterPhysics( new G4EmStandardPhysics(ver) );
    RegisterPhysics( new G4EmPenelopePhysics(ver) );

    // Synchroton Radiation & GN Physics
    RegisterPhysics( new G4EmExtraPhysics(ver) );

    // Decays
    RegisterPhysics( new G4DecayPhysics(ver) );

    // Hadron Elastic scattering
    RegisterPhysics( new G4HadronElasticPhysics(ver) );

    // Hadron Physics
    RegisterPhysics( new G4HadronPhysicsQGSP_BERT(ver));

    // Stopping Physics
    RegisterPhysics( new G4StoppingPhysics(ver) );

    // Ion Physics
    RegisterPhysics( new G4IonPhysics(ver));

    // Neutron tracking cut
    RegisterPhysics( new G4NeutronTrackingCut(ver));
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
