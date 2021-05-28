#ifndef JupiterSD_H
#define JupiterSD_H 1


#include "G4VSensitiveDetector.hh"
#include "JupiterHit.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class JupiterSD : public G4VSensitiveDetector
{
public:
    JupiterSD(G4String);
    ~JupiterSD();

    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);

private:
    JupiterHitsCollection* DetectorCollection;
    G4int NbHits;
};

#endif
