// Jupiter Auroral Simulation
// GEANT4-11.0.0
// Gabriel Bridges Mar, 2021

// Load Geant Libraries
#include "G4Event.hh"
#include "G4ios.hh"
// Load user Libraries
#include "JupiterEventAction.hh"
#include "global.h"


JupiterEventAction::JupiterEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JupiterEventAction::~JupiterEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JupiterEventAction::BeginOfEventAction(const G4Event* evt)
{
  extern global_struct global;
  global.eventID = evt -> GetEventID();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JupiterEventAction::EndOfEventAction(const G4Event* evt)
{
    G4int event_id = evt -> GetEventID();
    if (event_id < 10 || event_id%1000 == 0) G4cout << ">>> Event " << evt -> GetEventID() << "\r";
}
