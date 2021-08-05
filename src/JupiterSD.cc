 // Jupiter Auroral Simulation
// GEANT4-10.6.2
// Gabriel Bridges Aug, 2020

// Sensitive Detector Scripts

//includes
// user headers
#include "JupiterSD.hh"
#include "global.h"
// geant headers
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"

JupiterSD::JupiterSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
	collectionName.insert(HCname = name);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JupiterSD::~JupiterSD(){ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JupiterSD::Initialize(G4HCofThisEvent* HCE)
{
  DetectorCollection = new JupiterHitsCollection(SensitiveDetectorName,collectionName[0]);
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection( hcID, DetectorCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool JupiterSD::ProcessHits(G4Step* step,G4TouchableHistory*)
{
  extern global_struct global;

  JupiterHit* newHit = new JupiterHit();

  newHit -> setEventID(G4RunManager::GetRunManager() -> GetCurrentEvent() -> GetEventID());
  newHit -> SetTrackID(step -> GetTrack() -> GetTrackID());
  newHit -> SetParentID(step -> GetTrack() -> GetParentID());
  newHit -> SetStep(step -> GetTrack() -> GetCurrentStepNumber());
  newHit -> SetParticleID(step -> GetTrack() -> GetDynamicParticle() -> GetDefinition() -> GetPDGEncoding());
  newHit -> SetPostCopyNb(step -> GetPostStepPoint() -> GetTouchableHandle() -> GetCopyNumber());
  newHit -> SetEnergy(step -> GetPreStepPoint() -> GetKineticEnergy());
  newHit -> SetEdep(step -> GetTotalEnergyDeposit());
  newHit -> SetPos(step -> GetPostStepPoint() -> GetPosition());
  newHit -> SetVertexPos(step -> GetTrack() -> GetVertexPosition());

  DetectorCollection -> insert(newHit);
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JupiterSD::EndOfEvent(G4HCofThisEvent*)
{
	extern global_struct global;
  NbHits = DetectorCollection -> entries();
  for(G4int i=0; i<NbHits; i++ )
  {
    (*DetectorCollection)[i] -> fPrint();
  }

  if (verboseLevel > 1)
  {
    G4cout << "\n-------->Hits Collection: in this event there are " << NbHits << G4endl;
    for (G4int i=0; i<NbHits; i++) (*DetectorCollection)[i]->Print();
  }
}
