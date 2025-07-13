#include "generator.hh"

PrimaryGenerator::PrimaryGenerator()
{
    // Cs-137 Gamma Source (0.662 MeV)
    fParticleSourceCs137 = new G4GeneralParticleSource();
    fParticleSourceCs137->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("gamma"));

    // Set position as a small cylinder of 2 cm diameter and 0.5 cm thickness
    fParticleSourceCs137->GetCurrentSource()->GetPosDist()->SetPosDisType("Volume");
    fParticleSourceCs137->GetCurrentSource()->GetPosDist()->SetPosDisShape("Cylinder");
    fParticleSourceCs137->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., 12 * cm));
    fParticleSourceCs137->GetCurrentSource()->GetPosDist()->SetRadius(1.0 * cm);
    fParticleSourceCs137->GetCurrentSource()->GetPosDist()->SetHalfZ(0.25 * cm);

    // Isotropic angular distribution
    fParticleSourceCs137->GetCurrentSource()->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
    fParticleSourceCs137->GetCurrentSource()->GetAngDist()->SetAngDistType("iso");

    // Energy distribution (Cs-137 emits 0.662 MeV gamma rays)
    fParticleSourceCs137->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");
    fParticleSourceCs137->GetCurrentSource()->GetEneDist()->SetMonoEnergy(0.662 * MeV);
}

PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleSourceCs137;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // Generate Cs-137 gamma ray
    fParticleSourceCs137->GeneratePrimaryVertex(anEvent);
}

