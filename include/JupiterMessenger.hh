
#ifndef JupiterMessenger_H
#define JupiterMessenger_H 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "global.h"

class ConstructJupiter;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class JupiterMessenger: public G4UImessenger
{
  public:
    JupiterMessenger(ConstructJupiter*);
   ~JupiterMessenger();

    void SetNewValue(G4UIcommand* command, G4String newValue);

  private:
    ConstructJupiter*           myDetector;
    G4UIcmdWithAString*         OutDirCmd;
    G4UIcmdWithAString*         OutFileCmd;
};

#endif
