// Jupiter Auroral Simulation
// GEANT4-11.0.0
// Gabriel Bridges Mar, 2021

// Load GEANT Libraries
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4EventManager.hh"
#include "G4AnalysisManager.hh"
// Load User Libraries
#include "JupiterHit.hh"
#include "global.h"


G4Allocator<JupiterHit> JupiterHitAllocator;

// null constructor and destructor
JupiterHit::JupiterHit() {}
JupiterHit::~JupiterHit() {}

// new hit
JupiterHit::JupiterHit(const JupiterHit& right):
	G4VHit()
{
	eventID_		= right.eventID_;
	trackID_   		= right.trackID_;
	step_			= right.step_;
	particleID_ 	= right.particleID_;
	postCopyNb_ 	= right.postCopyNb_;
	kEnergy_   		= right.kEnergy_;
	eDep_ 			= right.eDep_;
    pos_       		= right.pos_;
	vertexPos_ 		= right.vertexPos_;
}

const JupiterHit& JupiterHit::operator=(const JupiterHit& right)
{
	eventID_		= right.eventID_;
	trackID_   		= right.trackID_;
	step_			= right.step_;
	particleID_ 	= right.particleID_;
	postCopyNb_ 	= right.postCopyNb_;
	kEnergy_   		= right.kEnergy_;
	eDep_ 			= right.eDep_;
    pos_       		= right.pos_;
	vertexPos_ 		= right.vertexPos_;
 	return *this;
}

G4int JupiterHit::operator==(const JupiterHit& right) const
{
	return (this == &right) ? 1 : 0;
}


void JupiterHit::Print()
{
	G4cout << "  trackID_: " << trackID_ << "  time_: " << "  particleID_: " << particleID_
		   << "  postCopyNb: " << postCopyNb_ << "  energy: " << kEnergy_ / keV
		   << "  e: " << G4BestUnit(eDep_,"Energy") << "  position: " << G4BestUnit(pos_,"Length") << G4endl;
}

void JupiterHit::fPrint()
{
	extern global_struct global;
    if (particleID_ == 22){ // if a photon was emitted make sure to keep it for visualization
        G4EventManager::GetEventManager() -> KeepTheCurrentEvent();
    }

	if ((postCopyNb_ == -1) && (particleID_ == 22)){ // if a photon escapes record info about the track
		auto analysisManager = G4AnalysisManager::Instance();

		analysisManager -> FillNtupleIColumn(1, 0, int(eventID_));
		analysisManager -> FillNtupleIColumn(1, 1, int(trackID_));
		analysisManager -> FillNtupleIColumn(1, 2, int(particleID_));
		analysisManager -> FillNtupleIColumn(1, 3, int(postCopyNb_));

		analysisManager -> FillNtupleDColumn(1, 4, double(kEnergy_/keV));
		analysisManager -> FillNtupleDColumn(1, 5, double(eDep_/keV));

		analysisManager -> FillNtupleDColumn(1, 6, double(pos_.getX()/km));
		analysisManager -> FillNtupleDColumn(1, 7, double(pos_.getY()/km));
		analysisManager -> FillNtupleDColumn(1, 8, double(pos_.getZ()/km));
		analysisManager -> FillNtupleDColumn(1, 9, double(vertexPos_.getX()/km));
		analysisManager -> FillNtupleDColumn(1, 10, double(vertexPos_.getY()/km));
		analysisManager -> FillNtupleDColumn(1, 11, double(vertexPos_.getZ()/km));

		analysisManager -> AddNtupleRow(1);
	}
	else if(parentID_ == 0 && (step_ == 1)){ // also record all electrons so we can match parent electrons to their X-Ray children
		auto analysisManager = G4AnalysisManager::Instance();
		analysisManager -> FillNtupleIColumn(0, 0, int(eventID_));
		analysisManager -> FillNtupleDColumn(0, 1, int(kEnergy_/keV));

		analysisManager -> AddNtupleRow(0);
	}
}
