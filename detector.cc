#include "detector.hh"
#include "event.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
  quEff = new G4PhysicsFreeVector();
  
  std::ifstream datafile;
  datafile.open("eff.dat");
  
  while(1)
  {
      G4double wlen, queff;
      
      datafile >> wlen >> queff;
      
      if(datafile.eof())
        break;
      
      G4cout << wlen << " " << queff << G4endl;
      
      quEff->InsertValues(wlen, queff/100.);
  }
  
  datafile.close();

}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
  G4Track *track = aStep->GetTrack();
  //track->SetTrackStatus(fStopAndKill);
  
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0) return false;

  const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();
  G4int copyNo = touchable->GetCopyNumber();

    EventAction* eventAction = (EventAction*)G4RunManager::GetRunManager()->GetUserEventAction();
    eventAction->AddEdep(copyNo, edep);
    
    if (eventAction)
    {
        eventAction->AddEdep(copyNo, edep);
    }
  
  G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
  G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
  
  G4ThreeVector posPhoton = preStepPoint->GetPosition();
  G4ThreeVector momPhoton = preStepPoint->GetMomentum();
  
  G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;
  
  //G4cout << "Photon Position: " << posPhoton << G4endl;
  

  
  G4VPhysicalVolume *physVol = touchable->GetVolume();
  G4ThreeVector posDetector = physVol->GetTranslation();
  
  //G4cout <<"Detector Position: "<< posDetector << G4endl;
  
  G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  
  man->FillNtupleIColumn(0, 0, evt);
  man->FillNtupleDColumn(0, 1, posPhoton[0]);
  man->FillNtupleDColumn(0, 2, posPhoton[1]);
  man->FillNtupleDColumn(0, 3, posPhoton[2]);
  man->FillNtupleDColumn(0, 4, wlen);
  man->AddNtupleRow(0);
  
  if(G4UniformRand() < quEff->Value(wlen))
  {
    man->FillNtupleIColumn(1, 0, evt);
    man->FillNtupleDColumn(1, 1, posDetector[0]);
    man->FillNtupleDColumn(1, 2, posDetector[1]);
    man->FillNtupleDColumn(1, 3, posDetector[2]);
    man->AddNtupleRow(1);
  }
  
  return true;
}
