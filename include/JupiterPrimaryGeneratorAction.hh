#ifndef JupiterPrimaryGeneratorAction_H
#define JupiterPrimaryGeneratorAction_H 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class JupiterPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        JupiterPrimaryGeneratorAction();
        ~JupiterPrimaryGeneratorAction();

    public:
        void GeneratePrimaries(G4Event*);

    private:
        G4GeneralParticleSource* GeneralParticleSource;
};
#endif
