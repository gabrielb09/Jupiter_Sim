// Jupiter Auroral Simulation
// GEANT4-11.0.0
// Gabriel Bridges Mar, 2021

// Loading GEANT libraries
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisExecutive.hh"
#include "G4UItcsh.hh"
// Loading User libraries
#include "ConstructJupiter.hh"
#include "JupiterPhysicsList.hh"
#include "JupiterPrimaryGeneratorAction.hh"
#include "JupiterRunAction.hh"
#include "JupiterEventAction.hh"
#include "global.h"
global_struct global;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    // at start of program set the random seed to the clock time for more randomness
    CLHEP::HepRandom::setTheSeed(time(NULL));

    // First we need to create a run manager which will handle the overall run
    // the run manager will need to have your simulation's geometry, physics list, and run action registered with it
    G4RunManager * runManager = new G4RunManager;

    // register required user objects
    // create and register user geometry
    ConstructJupiter* detector = new ConstructJupiter;
    runManager -> SetUserInitialization(detector);

    // create and register user physics list
    G4VUserPhysicsList* physics = new JupiterPhysicsList();
    physics -> SetDefaultCutValue(1.0*km); // set a physics 'cut'
    runManager -> SetUserInitialization(physics);

    // create and register user generator action
    G4VUserPrimaryGeneratorAction* gen_action = new JupiterPrimaryGeneratorAction();
    runManager -> SetUserAction(gen_action);


    // register optional User actions
    // create and register user run action
    G4UserRunAction* run_action = new JupiterRunAction;
    runManager -> SetUserAction(run_action);

    //create and register user event action
    G4UserEventAction* event_action = new JupiterEventAction;
    runManager -> SetUserAction(event_action);

    // Initialize geant kernel
    runManager -> Initialize();

    // Create and initialize Geant visualization
    G4VisManager* visManager = new G4VisExecutive();
    visManager -> Initialize();

    // Finally, set up UI session
    // get pointer to UI manager
    G4UImanager* UI = G4UImanager::GetUIpointer();

    // automatically run a command to point the UI manager to the macros directory
    UI -> ApplyCommand("/control/macroPath ../macro/");

    // configure the UI terminal
    if(argc==1)  // if we have not indicated which macro to run at the start launch a UI session
    {
        G4UIsession* session = 0;
        session = new G4UIterminal(new G4UItcsh);
        UI -> ApplyCommand("/control/execute"); // execute nothing to get it started
        session -> SessionStart();
        delete session;
    }
    else // If there is a macro, execute it
    {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI -> ApplyCommand(command+fileName); // execute the desired macro
    }

    // after simulation is complete be sure to delete the managers
    delete visManager;
    delete runManager;

    return 0;
}
