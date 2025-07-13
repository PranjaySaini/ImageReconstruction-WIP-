#include "event.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"

EventAction::EventAction(RunAction*) {}
EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*)
{
    auto steppingAction = const_cast<SteppingAction*>(static_cast<const SteppingAction*>(G4RunManager::GetRunManager()->GetUserSteppingAction()));
    steppingAction->ResetEventEnergy();
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    G4int eventID = event->GetEventID();
    G4int totalEvents = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
    fPrintModulo = totalEvents/20;
    
    if((eventID + 1) % fPrintModulo == 0 || (eventID+1) == totalEvents) {
    	G4double percent = 100.0 * (eventID + 1) / totalEvents;
    	G4cout << "Progress: " << (eventID + 1) << "/" << totalEvents << " events (" << percent << "% complete)" << G4endl;
    }
    auto analysisManager = G4AnalysisManager::Instance();
    auto steppingAction = static_cast<const SteppingAction*>(G4RunManager::GetRunManager()->GetUserSteppingAction());
    const auto& energyMap = steppingAction->GetEventEnergy();

    // Fill ROOT histograms for each detector
    for (const auto& entry : energyMap) {
        G4int copyNo = entry.first;
        G4double energyDeposit = entry.second;

        // Fill the ROOT ntuple corresponding to this detector
        analysisManager->FillNtupleDColumn(copyNo, 0, energyDeposit);
        analysisManager->AddNtupleRow(copyNo);
        analysisManager->FillH1(copyNo, energyDeposit);
    }

    // Write event data to the CSV file
    auto runAction = const_cast<RunAction*>(static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));

    if (runAction->csvFile.is_open()) {
    for (G4int i = 0; i < runAction->GetNumberOfDetectors(); i++) {
        G4double energyDeposit = 0.0;
        auto it = energyMap.find(i);
        if (it != energyMap.end()) {
            energyDeposit = it->second;
        }
        runAction->csvBuffer << (i == 0 ? "" : ",") << energyDeposit;  // Append to buffer
    }
    runAction->csvBuffer << "\n";  // End of event
}
}
