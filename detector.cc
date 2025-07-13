#include "detector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomTools.hh" // For G4RandGauss

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
	a = 0;
        b = 1.22835 * sqrt(keV);
        c = 1.44923e-03  * 1 / (keV);
}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep > 0) {
        G4double FWHM = a + b * sqrt(edep + c * edep * edep);
        //G4double FWHM = edep * 0.07;
        G4double sigma = FWHM / 2.35; 
        G4double smearedEdep = G4RandGauss::shoot(edep, sigma);
    }
    return true;
}

