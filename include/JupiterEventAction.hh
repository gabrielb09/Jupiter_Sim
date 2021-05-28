#ifndef JupiterEventAction_H
#define JupiterEventAction_H 1

#include "G4UserEventAction.hh"

class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class JupiterEventAction : public G4UserEventAction
{
  public:
    JupiterEventAction();
   ~JupiterEventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
};

#endif
