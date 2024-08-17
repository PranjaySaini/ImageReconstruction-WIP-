#include "physics.hh"

PhysicsList::PhysicsList()
{
  RegisterPhysics (new G4EmStandardPhysics_option4());
  RegisterPhysics (new G4OpticalPhysics());
  RegisterPhysics (new G4DecayPhysics());
  RegisterPhysics (new G4RadioactiveDecayPhysics());
}

PhysicsList::~PhysicsList()
{}

//void PhysicsList::ConstructParticle()
//{}
  
//void PhysicsList::ConstructProcess()
//{
  //AddTransportation();
//}
  

  
  

