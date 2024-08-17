#include "stepping.hh"

SteppingAction::SteppingAction(EventAction *eventAction)
{
  fEventAction = eventAction;
}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    const DetectorConstruction *detectorConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();

    G4double edep = step->GetTotalEnergyDeposit();
    if (volume != fScoringVolume || edep == 0) return;

    const G4VTouchable *touchable = step->GetPreStepPoint()->GetTouchable();
    G4int copyNo = touchable->GetCopyNumber();

    fEventAction->AddEdep(copyNo, edep);
}


