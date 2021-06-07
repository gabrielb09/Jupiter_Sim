#include "ConstructJupiter.hh"
#include "JupiterPhysicsList.hh"
#include "JupiterPrimaryGeneratorAction.hh"
#include "JupiterRunAction.hh"
#include "JupiterEventAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

//#include "QGSP.hh"
#include "QGSP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BIC_HP.hh"
//#include "LHEP_BERT.hh"
#include "Shielding.hh"
#include "G4SystemOfUnits.hh"

//#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
//#endif
#include "global.h"


global_struct global;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // User Initialization classes (mandatory)
  //
  ConstructJupiter* detector = new ConstructJupiter;
  runManager->SetUserInitialization(detector);

  G4VUserPhysicsList* physics = new JupiterPhysicsList();

	physics->SetDefaultCutValue(1.0*km) ;
	runManager->SetUserInitialization(physics);

//#ifdef G4VIS_USE
  // Visualization, if you choose to have it!
  //
  G4VisManager* visManager = new G4VisExecutive();
  visManager->Initialize();
//#endif

  // User Action classes
  //
  G4VUserPrimaryGeneratorAction* gen_action = new JupiterPrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);
  //
  G4UserRunAction* run_action = new JupiterRunAction;
  runManager->SetUserAction(run_action);
  //
  G4UserEventAction* event_action = new JupiterEventAction;
  runManager->SetUserAction(event_action);
  // Initialize G4 kernel
  //
  runManager->Initialize();

  // Get the pointer to the User Interface manager
  //
  G4UImanager * UI = G4UImanager::GetUIpointer();

  UI->ApplyCommand("/control/macroPath ../macro/");

  if(argc==1)  // Define (G)UI terminal for interactive mode
  {
    // G4UIterminal is a (dumb) terminal
    //
    G4UIsession * session = 0;
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);
#else
      session = new G4UIterminal();
#endif
    UI->ApplyCommand("/control/execute");
    session->SessionStart();
    delete session;
  }
  else   // Batch mode
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }

  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

//#ifdef G4VIS_USE
  delete visManager;
//#endif

  delete runManager;

  return 0;
}
