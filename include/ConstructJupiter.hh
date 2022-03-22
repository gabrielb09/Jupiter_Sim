// Jupiter Auroral Simulation
// GEANT4-11.0.0
// Gabriel Bridges Mar, 2021

// Geometry construction header

#ifndef ConstructJupiter_h
#define ConstructJupiter_h 1

// Load Geant Libraries
#include "G4VUserDetectorConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

// Load User Libraries
#include "JupiterSD.hh"
#include "globals.hh"

// basic class creation
class G4Box;
class G4Sphere;
class G4Material;
class JupiterMessenger;

// Jupiter Construction class
class ConstructJupiter : public G4VUserDetectorConstruction
{
    public:
        // constructor and destructor
        ConstructJupiter();
        ~ConstructJupiter();

        // material and updater methods
        void DefineMaterials();
        void ConstructSDandField();

        // GEANT geometry construction method
        G4VPhysicalVolume* Construct();

        // helper method to get length
        G4double GetWorldFullLength(){
            return WorldLength;
        };

    private:
        // actual geometry construction method
        G4VPhysicalVolume* JupiterConstruction();

        // constructor variables
        JupiterMessenger* jovianMessenger;
        G4int overlap;
        const static int num_layers = 80;

        // material variables
        G4NistManager* nistManager; // NIST manager
        G4Material* Galactic;
        G4Material* Hydrogen;
        G4Material* Helium;
        G4Material* Methane;

        G4Material* IPM; // User defined materials

        G4String line;
        G4String density_line;
        G4String temperature_line;
        G4String pressure_line;
        G4String numEl_line;
        G4Material *materials[num_layers];


        // geometry variables
        // World objects
        G4double WorldLength;
        G4Box* WorldSolid;
        G4LogicalVolume* WorldLogical;
        G4VPhysicalVolume* WorldPhysical;
        // Atmosphere objects
        G4double Rin;
        G4Sphere *layers_solid[num_layers];
        G4LogicalVolume *layers_logical[num_layers];
        G4VPhysicalVolume *layers_physical[num_layers];

        // Visible attributes
        G4VisAttributes* TransparentVisAtt;
        G4VisAttributes *vis_atts[num_layers];

        // Sensitive Detectors
        G4SDManager* DetectorManager;
        G4VSensitiveDetector* SDetector;
};

#endif
