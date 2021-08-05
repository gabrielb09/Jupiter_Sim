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

  analysisManager -> CreateNtuple("Primary_Electrons", "Primary Electron Data");

  analysisManager -> CreateNtupleIColumn(0, "eventID");
  analysisManager -> CreateNtupleDColumn(0, "energy");

  analysisManager -> FinishNtuple(0);

  analysisManager -> CreateNtuple("X_Rays", "Escaping X-Ray Data");

  analysisManager -> CreateNtupleIColumn(1, "eventID");
  analysisManager -> CreateNtupleIColumn(1, "trackID");
  analysisManager -> CreateNtupleIColumn(1, "particleID");
  analysisManager -> CreateNtupleIColumn(1, "postCopyNb");

  analysisManager -> CreateNtupleDColumn(1, "energy");
  analysisManager -> CreateNtupleDColumn(1, "eDep");

  analysisManager -> CreateNtupleDColumn(1, "x");
  analysisManager -> CreateNtupleDColumn(1, "y");
  analysisManager -> CreateNtupleDColumn(1, "z");
  analysisManager -> CreateNtupleDColumn(1, "vertex_x");
  analysisManager -> CreateNtupleDColumn(1, "vertex_y");
  analysisManager -> CreateNtupleDColumn(1, "vertex_z");

  analysisManager -> FinishNtuple(1);

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
