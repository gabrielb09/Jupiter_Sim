#ifndef JupiterRunAction_H
#define JupiterRunAction_H 1

#include "G4UserRunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

class JupiterRunAction : public G4UserRunAction
{
    public:
        JupiterRunAction();
        ~JupiterRunAction();

    public:
        void BeginOfRunAction(const G4Run*);
        void EndOfRunAction(const G4Run*);
};

#endif
