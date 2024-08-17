#include "generator.hh"

PrimaryGenerator::PrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
        fParticleGun->SetParticleDefinition(particle);
        
        srand(time(NULL));       
}

PrimaryGenerator::~PrimaryGenerator()
{
	delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleDefinition *particle = fParticleGun->GetParticleDefinition();

	if (particle == G4Geantino::Geantino())
	{
		G4int Z = 27;
		G4int A = 60;

		G4double charge = 0. * eplus;
		G4double energy = 0. * keV;

		G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);

		fParticleGun->SetParticleDefinition(ion);
		fParticleGun->SetParticleCharge(charge);
		fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 12*cm));
		fParticleGun->SetParticleMomentum(0.*keV);
	        fParticleGun->GeneratePrimaryVertex(anEvent);
	        

	}
	
                fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 12*cm));
                
                G4double par_x = (pow(-1, rand() % 2));
                G4double par_y = (pow(-1, rand() % 2));
                G4double par_z = (pow(-1, rand() % 2));
                
                G4double x_mom = par_x*(rand()/10e+9);
                G4double y_mom = par_y*(rand()/10e+9);
                G4double z_mom = par_z*(rand()/10e+9);
                
                G4ThreeVector mom(x_mom, y_mom, z_mom);
                fParticleGun->SetParticleMomentumDirection(mom);
                fParticleGun->SetParticleMomentum(1.* MeV);
	        fParticleGun->GeneratePrimaryVertex(anEvent);
      
}

