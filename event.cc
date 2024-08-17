#include "event.hh"

EventAction::EventAction(RunAction*)
{
    fEdepMap.clear();
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{ 
}

void EventAction::AddEdep(G4int copyNo, G4double edep)
{
    if (fEdepMap.find(copyNo) == fEdepMap.end()) {
        fEdepMap[copyNo] = 0.0;
    }
    fEdepMap[copyNo] += edep;
}

void EventAction::EndOfEventAction(const G4Event*)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    for (const auto& pair : fEdepMap) {
        G4int copyNo = pair.first;
        G4double edep = pair.second;
        man->FillNtupleDColumn(2+copyNo, 0, static_cast<double>(edep));
        man->AddNtupleRow(2+copyNo);
    }
    fEdepMap.clear(); // Clear the map for the next event
}






