#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4VUserPhysicsList.hh"
#include "G4ProductionCuts.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmLivermorePhysics.hh"

class PhysicsList : public G4VModularPhysicsList
{
public :
    PhysicsList();
    ~PhysicsList();
protected:
    //void ConstructParticle() override;
    //void ConstructProcess() override;
    //void SetCuts() override;
};

#endif

