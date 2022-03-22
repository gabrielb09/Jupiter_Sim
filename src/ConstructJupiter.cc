// Jupiter Auroral Simulation
// GEANT4-11.0.0
// Gabriel Bridges Mar, 2021

// Geometry construction

// Load C++ Libraries
#include <fstream>

// Load GEANT libraries
// generic
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
// geometry
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
// material
#include "G4Material.hh"
#include "G4NistManager.hh"
// managers
#include "G4GeometryManager.hh"
#include "G4SDManager.hh"

// Load User Libraries
#include "ConstructJupiter.hh"
#include "JupiterMessenger.hh"
#include "global.h"

// constructor
ConstructJupiter::ConstructJupiter()
:WorldSolid(0),WorldLogical(0),WorldPhysical(0) // initialize World volumes before execution
{
    DefineMaterials(); // call DefineMaterials
    jovianMessenger = new JupiterMessenger(); // create messenger 
}

// destructor
ConstructJupiter::~ConstructJupiter()
{
    delete jovianMessenger;
}

// Material definitions
void ConstructJupiter::DefineMaterials()
{
    // initialize NIST Manager
    nistManager = G4NistManager::Instance();

    // gather NIST materials
    Galactic = nistManager -> FindOrBuildMaterial("G4_Galactic");
    Hydrogen = nistManager -> FindOrBuildMaterial("G4_H");
    Helium = nistManager -> FindOrBuildMaterial("G4_He");
    Methane = nistManager -> FindOrBuildMaterial("G4_METHANE");

    // Inter planetary medium
    IPM = new G4Material("Vacuum", 2.376e-15*g/cm3, 1, kStateGas, 10*kelvin, 1.0e-8*bar);
    IPM -> AddMaterial(Galactic, 100*perCent);

    // Jovian Atmoshpere

    ifstream material_file;
    material_file.open("../src/Materials.txt");
    int i = -1;
    while (getline(material_file, line)){
        if (line[0] == '/'){
            continue;
        }
        else if (line[0] == '#'){
            i += 1;
            getline(material_file, density_line);
            getline(material_file, numEl_line);
            getline(material_file, temperature_line);
            getline(material_file, pressure_line);
            materials[i] = new G4Material("Atmoshpere" + to_string(i), stod(density_line.substr(density_line.find('=') + 1))*g/cm3,
                                                        stoi(numEl_line.substr(numEl_line.find('=') + 1)), kStateGas,
                                                        stod(temperature_line.substr(temperature_line.find('=') + 1))*kelvin,
                                                        stod(pressure_line.substr(pressure_line.find('=') + 1))*bar);
            for(int j = 0; j < stoi(numEl_line.substr(numEl_line.find('=') + 1)); j++){
                getline(material_file, line);
                if (line.substr(0, line.find('=') - 1).compare("Hydrogen") == 0){
                    if (stod(line.substr(line.find('=') + 1)) > 100.0){
                        materials[i] -> AddMaterial(Hydrogen, 1.);
                    }
                    else{
                        materials[i] -> AddMaterial(Hydrogen, stod(line.substr(line.find('=') + 1))*perCent);
                    }
                }
                else if (line.substr(0, line.find('=') - 1).compare("Helium") == 0){
                    materials[i] -> AddMaterial(Helium, stod(line.substr(line.find('=') + 1))*perCent);
                }
                else if (line.substr(0, line.find('=') - 1).compare("Methane") == 0){
                    materials[i] -> AddMaterial(Methane, stod(line.substr(line.find('=') + 1))*perCent);
                }
            }
        }
    }
    material_file.close();

    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

// Construction method, returns G4VPhysicalVolume
G4VPhysicalVolume* ConstructJupiter::Construct()
{
  G4GeometryManager::GetInstance() -> SetWorldMaximumExtent(320000*km);
  return JupiterConstruction();
}

G4VPhysicalVolume* ConstructJupiter::JupiterConstruction()
{
    extern global_struct global;
    overlap = global.CheckOverlap;

    //--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------

    //------------------------------------------------
    // World
    //------------------------------------------------

    WorldLength = 320000.0*km;
    WorldSolid = new G4Box("SolidWorld", WorldLength*0.5, WorldLength*0.5, WorldLength*0.5);
    WorldLogical = new G4LogicalVolume(WorldSolid, IPM, "World", 0, 0, 0);
    WorldPhysical = new G4PVPlacement(0, G4ThreeVector(), WorldLogical, "World", 0, false, -1, true);

    //------------------------------------------------
    // Atmoshpere
    //------------------------------------------------

    ifstream geometry_file;
    geometry_file.open("../src/Layers.txt");
    int i = -1;
    Rin = 0*km;

    while (getline(geometry_file, line)){
        i += 1;
        layers_solid[i] = new G4Sphere("Atmoshpere_Solid_" + to_string(i), Rin, Rin + stod(line)*km, 0*degree, 360*degree, 0*degree, 180*degree);
        layers_logical[i] = new G4LogicalVolume(layers_solid[i], materials[i], "Atmoshpere_Logical_" + to_string(i));
        layers_physical[i] = new G4PVPlacement(0, G4ThreeVector(), layers_logical[i], "Atmoshpere_Physical_" + to_string(i), WorldLogical, false, i);
        Rin += stod(line)*km;
    }
    geometry_file.close();

    //------------------------------------------------
    // Visualization attributes
    //------------------------------------------------

    TransparentVisAtt = new G4VisAttributes(G4Colour(1.00, 1.00, 1.00, 0.0));
    WorldLogical -> SetVisAttributes(TransparentVisAtt);

    ifstream visualization_file;
    visualization_file.open("../src/Visualization.txt");

    i = -1;

    while (getline(visualization_file, line)){
        i += 1;
        vis_atts[i] = new G4VisAttributes(G4Colour(207.0/255.0, 113.0/255.0, 66.0/255.0, stod(line)));
        layers_logical[i] -> SetVisAttributes(vis_atts[i]);
    }

    visualization_file.close();

    return WorldPhysical;
}

void ConstructJupiter::ConstructSDandField()
{
    DetectorManager = G4SDManager::GetSDMpointer(); // get Sensitive Detector Manager

    // create new sensitice detector
    SDetector = new JupiterSD("SensitiveAtmo");
    DetectorManager -> AddNewDetector(SDetector);
    // and assign it to each atmosphere volume
    for(int i = 0; i < num_layers; i++){
        layers_logical[i] -> SetSensitiveDetector(SDetector);
    }
}
