
#ifndef JupiterMessenger_H
#define JupiterMessenger_H 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "global.h"

class G4UIcmdWithAString;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class JupiterMessenger: public G4UImessenger
{
  public:
    JupiterMessenger();
   ~JupiterMessenger();

    void SetNewValue(G4UIcommand* command, G4String newValue);

  private:
    G4UIcmdWithAString*         OutDirCmd;
    G4UIcmdWithAString*         OutFileCmd;
};

#endif
