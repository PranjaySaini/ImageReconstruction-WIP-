#include "run.hh"
#include "construction.hh"
#include <iostream>
#include <fstream>

RunAction::RunAction()
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->SetNtupleMerging(true);
}

RunAction::~RunAction()
{}

G4int RunAction::GetNumberOfDetectors()
{
    // Assuming you have access to your DetectorConstruction or similar class:
    const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    return detectorConstruction->GetNumberOfDetectors(); // Implement this method in DetectorConstruction
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;
    
    man->OpenFile("output" + strRunID.str() + ".root");
    
    man->CreateNtuple("Photons", "Photons");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fWlen");
    man->FinishNtuple(0);
    
    man->CreateNtuple("Hits", "Hits");
    man->CreateNtupleIColumn("fEvent");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(1);
    
    G4int numDetectors = GetNumberOfDetectors(); // Add a function to get this value
  
    for (G4int i = 0; i < numDetectors; i++) {
        man->CreateNtuple("Detector" + std::to_string(i), "Detector " + std::to_string(i));
        man->CreateNtupleDColumn("fEdep");
        man->FinishNtuple(i+2);
    }

}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager *man = G4AnalysisManager::Instance();
  
  man->Write();
  man->CloseFile();
}


