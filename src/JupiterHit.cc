// Jupiter Auroral Simulation
// GEANT4-10.6.2
// Gabriel Bridges Aug, 2020

// Detector hit handler

// includes
//user header
#include "JupiterHit.hh"
#include "global.h"
#include "JupiterAnalysis.hh"
// Geant Visualization
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
// Geant units
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

G4Allocator<JupiterHit> JupiterHitAllocator;

// null constructor and destructor
JupiterHit::JupiterHit() {}
JupiterHit::~JupiterHit() {}

// new hit
JupiterHit::JupiterHit(const JupiterHit& right):
	G4VHit()
{
	trackID_   			= right.trackID_;
	particleID_ 		= right.particleID_;
	postCopyNb_ 		= right.postCopyNb_;
	kEnergy_   			= right.kEnergy_;
	eDep_ 					= right.eDep_;
  pos_       			= right.pos_;
	vertexPos_ 			= right.vertexPos_;
}

const JupiterHit& JupiterHit::operator=(const JupiterHit& right)
{
	trackID_   			= right.trackID_;
	particleID_ 		= right.particleID_;
	postCopyNb_ 		= right.postCopyNb_;
	kEnergy_   			= right.kEnergy_;
	eDep_ 					= right.eDep_;
  pos_       			= right.pos_;
	vertexPos_ 			= right.vertexPos_;
  return *this;
}

G4int JupiterHit::operator==(const JupiterHit& right) const
{
  return (this == &right) ? 1 : 0;
}

void JupiterHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos_);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void JupiterHit::Print()
{
  G4cout << "  trackID_: " << trackID_ << "  time_: "
				 << "  particleID_: " << particleID_
				 << "  postCopyNb: " << postCopyNb_
         << "  energy: " << kEnergy_ / keV
				 << "  e: " << G4BestUnit(eDep_,"Energy")
				 << "  position: " << G4BestUnit(pos_,"Length") << G4endl;
}

void JupiterHit::fPrint()
{
  extern global_struct global;

	auto analysisManager = G4AnalysisManager::Instance();

	analysisManager -> FillNtupleIColumn(0, int(trackID_));
	analysisManager -> FillNtupleIColumn(1, int(particleID_));
	analysisManager -> FillNtupleIColumn(2, int(postCopyNb_));

	analysisManager -> FillNtupleDColumn(3, double(kEnergy_/keV));
	analysisManager -> FillNtupleDColumn(4, double(eDep_/keV));

	analysisManager -> FillNtupleDColumn(5, double(pos_.getX()/km));
	analysisManager -> FillNtupleDColumn(6, double(pos_.getY()/km));
	analysisManager -> FillNtupleDColumn(7, double(pos_.getZ()/km));
	analysisManager -> FillNtupleDColumn(8, double(vertexPos_.getX()/km));
	analysisManager -> FillNtupleDColumn(9, double(vertexPos_.getY()/km));
	analysisManager -> FillNtupleDColumn(10, double(vertexPos_.getZ()/km));

  analysisManager->AddNtupleRow();
}
