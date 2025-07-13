#include "stepping.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4VTouchable.hh"

SteppingAction::SteppingAction() {
	a = 0;
        b = 1.22835 * sqrt(keV);
        c = 1.44923e-03  * 1 / (keV);
}
SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    if (volume->GetName() == "logicScintillator") {
        G4double edep = step->GetTotalEnergyDeposit();
        if (edep > 0) {
        G4int copyNo = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
        G4double FWHM = a + b * sqrt(edep + c * edep * edep);
        //G4double FWHM = 0.07 * edep;
        G4double sigma = FWHM / 2.35; 
        G4double smearedEdep = G4RandGauss::shoot(edep, sigma);
            fEventEnergy[copyNo] += smearedEdep;  // Accumulate energy for this detector
        }
    }
}

void SteppingAction::ResetEventEnergy()
{
    fEventEnergy.clear(); 
}

const std::map<G4int, G4double>& SteppingAction::GetEventEnergy() const
{
    return fEventEnergy;
}

