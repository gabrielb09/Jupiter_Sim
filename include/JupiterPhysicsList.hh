#ifndef JupiterPhysicsList_H
#define JupiterPhysicsList_H 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class JupiterPhysicsList: public G4VModularPhysicsList
{
    public:
        JupiterPhysicsList();
        virtual ~JupiterPhysicsList();

    public:
        virtual void SetCuts();
};

#endif
