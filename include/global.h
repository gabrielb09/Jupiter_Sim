#ifndef __DEF__GLOBAL__
#define __DEF__GLOBAL__

#include <fstream>

using namespace std;

#include "G4Types.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
class G4LogicalVolume;
class G4VPhysicalVolume;

static constexpr double nT = (1e-9)*tesla;

struct global_struct{
	G4int SimulationType;
	G4int OutputType;
	G4int OutputFormat;
	G4int TrackType;
  G4int TrackEdep;
	G4int DetectorVisualization;
	G4int DetectorType;
	G4int GPS;
	ofstream output;
	ifstream input;

	vector<float> inputX,inputY,inputZ,inputPx,inputPy,inputPz,inputE;
	int nLine;
	int runnum;
	int ID1, ID2, ID3;
	int eventID, trackID, parentID, particleID, preCopyNb, postCopyNb;
	float time, energy, px, py, pz, x, y,z;
	G4String creatorProcess;
	//char creatorProcess[30];
	int CheckOverlap;
	char outfile[100];
	char outdir[100];
	char infile[100];
	char indir[100];
  G4int seed;
};

#endif
