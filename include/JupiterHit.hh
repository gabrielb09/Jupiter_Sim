// Jupiter Auroral Simulation
// GEANT4-11.0.0
// Gabriel Bridges Mar, 2021

// Hit manager header
#ifndef JupiterHit_H
#define JupiterHit_H 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "G4VProcess.hh"

class JupiterHit : public G4VHit
{
    public:
        JupiterHit();
        ~JupiterHit();
        JupiterHit(const JupiterHit&);
        const JupiterHit& operator=(const JupiterHit&);
        G4int operator==(const JupiterHit&) const;

        inline void* operator new(size_t);
        inline void  operator delete(void*);

        void Print();
    	void fPrint();
    public:
        void SetEventID     (G4int eventID)     { eventID_ = eventID; };
      	void SetTrackID  	(G4int track)      	{ trackID_ = track; };
        void SetParentID    (G4int parent)      { parentID_ = parent; };
        void SetStep        (G4int step)        { step_ = step; };
        void SetParticleID  (G4int PDGid)      	{ particleID_ = PDGid; }
        void SetPostCopyNb	(G4int PostCopy)    { postCopyNb_ = PostCopy; };
        void SetEnergy   	(G4double kinE)     { kEnergy_ = kinE; };
        void SetEdep   		(G4double eDep)     { eDep_ = eDep; };
        void SetPos         (G4ThreeVector xyz)	{ pos_ = xyz; };
        void SetVertexPos	(G4ThreeVector xyz)	{ vertexPos_ = xyz; };

    private:
        G4int eventID_;
        G4int trackID_;
        G4int parentID_;
        G4int step_;
        G4int particleID_;
        G4int postCopyNb_;
        G4double kEnergy_;
        G4double eDep_;
        G4ThreeVector pos_;
    	G4ThreeVector vertexPos_;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<JupiterHit> JupiterHitsCollection;

extern G4Allocator<JupiterHit> JupiterHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* JupiterHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) JupiterHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void JupiterHit::operator delete(void *aHit)
{
  JupiterHitAllocator.FreeSingle((JupiterHit*) aHit);
}

#endif
