#include "JupiterRunAction.hh"
#include "JupiterAnalysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "g4root.hh"
#include "global.h"
#include <fstream>

using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JupiterRunAction::JupiterRunAction()
: G4UserRunAction()
{

  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager -> SetVerboseLevel(1);
  analysisManager -> SetNtupleMerging(true);

  analysisManager -> CreateNtuple("JupiterData", "Jupiter Simulation Data");

  analysisManager -> CreateNtupleIColumn("trackID");
  analysisManager -> CreateNtupleIColumn("particleID");
  analysisManager -> CreateNtupleIColumn("postCopyNb");

  analysisManager -> CreateNtupleDColumn("energy");
  analysisManager -> CreateNtupleDColumn("eDep");

  analysisManager -> CreateNtupleDColumn("x");
  analysisManager -> CreateNtupleDColumn("y");
  analysisManager -> CreateNtupleDColumn("z");
  analysisManager -> CreateNtupleDColumn("vertex_x");
  analysisManager -> CreateNtupleDColumn("vertex_y");
  analysisManager -> CreateNtupleDColumn("vertex_z");

  analysisManager->FinishNtuple();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JupiterRunAction::~JupiterRunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JupiterRunAction::BeginOfRunAction(const G4Run* aRun)
{
  extern global_struct global;
  char fname[100];

  ((G4Run *)(aRun))->SetRunID(global.runnum);
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  sprintf(fname, "%s/%s.root", global.outdir, global.outfile );
  G4cout << "Output file: " << fname << G4endl;

  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager -> OpenFile(fname);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JupiterRunAction::EndOfRunAction(const G4Run*)
{
	extern global_struct global;

	auto analysisManager = G4AnalysisManager::Instance();

  analysisManager -> Write();
  analysisManager -> CloseFile();

	G4cout << "Run end  " << G4endl;
}
