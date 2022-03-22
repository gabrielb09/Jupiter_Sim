// Jupiter Auroral Simulation
// GEANT4-11.0.0
// Gabriel Bridges Mar, 2021

// Sensitive Detector Scripts

// Load User Libraries
#include "JupiterSD.hh"

// Load GEANT libraries
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

JupiterSD::JupiterSD(G4String name)
:G4VSensitiveDetector(name)
{
    G4String HCname;
    collectionName.insert(HCname = name);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JupiterSD::~JupiterSD(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JupiterSD::Initialize(G4HCofThisEvent* hce){
    DetectorCollection = new JupiterHitsCollection(SensitiveDetectorName, collectionName[0]);
    auto hcID = G4SDManager::GetSDMpointer() -> GetCollectionID(collectionName[0]);
    hce -> AddHitsCollection( hcID, DetectorCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool JupiterSD::ProcessHits(G4Step* step,G4TouchableHistory*){
    JupiterHit* newHit = new JupiterHit();

    newHit -> SetEventID(G4RunManager::GetRunManager() -> GetCurrentEvent() -> GetEventID());
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

void JupiterSD::EndOfEvent(G4HCofThisEvent*){
    NbHits = DetectorCollection -> entries();
    for(G4int i=0; i<NbHits; i++ ){
        (*DetectorCollection)[i] -> fPrint();
    }

    if (verboseLevel > 1){
        G4cout << "\n-------->Hits Collection: in this event there are " << NbHits << G4endl;
        for (G4int i=0; i<NbHits; i++) (*DetectorCollection)[i]->Print();
    }
}
