#ifndef JupiterPrimaryGeneratorAction_H
#define JupiterPrimaryGeneratorAction_H 1

#include "G4VUserPrimaryGeneratorAction.hh"

class ConstructJupiter;
class G4GeneralParticleSource;
class G4SPSPosDistribution;
class G4SPSAngDistribution;
class G4SPSEneDistribution;
class G4ParticleGun;
class G4Event;

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
