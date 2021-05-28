// Jupiter Auroral Simulation
// GEANT4-10.6.2
// Gabriel Bridges Aug, 2020

// Handles event actions

//includes
// user headers
#include "JupiterEventAction.hh"
#include "JupiterAnalysis.hh"
#include "global.h"
// geant headers
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include <unistd.h>


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
    if (event_id < 10 || event_id%1000 == 0) G4cout << ">>> Event " << evt -> GetEventID() << G4endl;
}
