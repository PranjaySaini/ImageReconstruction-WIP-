#include "run.hh"
#include "messenger.hh"
RunAction::RunAction() : G4UserRunAction(), fNbins(100)
{
	fMessenger = new RunMessenger(this);
	timer = new G4Timer;

	
} 
RunAction::~RunAction() 
{
	delete fMessenger;
	delete timer;
	
}

void RunAction::BeginOfRunAction(const G4Run* run)
{  
    timer->Start();
    
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Reset();
    analysisManager->SetNtupleMerging(true);
    
    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;
    std::filesystem::path runDir = "Run_" + strRunID.str();
    std::filesystem::create_directory(runDir);
    analysisManager->OpenFile((runDir / ("output_run_" + strRunID.str() + ".root")).string());
    csvBuffer.str("");
    csvBuffer.clear();
    // Open the CSV file with run ID in the filename
    csvFile.open((runDir / ("energy_deposition_run_" + strRunID.str() + ".csv")).string());

    // Get the number of detectors
    const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4int numDetectors = detectorConstruction->GetNumberOfDetectors();

    for (G4int i = 0; i < numDetectors; i++) {
    if (i > 0) csvBuffer << ",";  // Add commas between column names
    csvBuffer << "Detector_" << i << "_Energy(MeV)";
}
csvBuffer << "\n";  // End of header

    // Create an ntuple for each detector to track energy deposition
    for (G4int i = 0; i < numDetectors; i++) {
        G4String ntupleName = "Detector_" + std::to_string(i);
        G4String ntupleTitle = "Energy Deposition in Detector " + std::to_string(i);

        analysisManager->CreateNtuple(ntupleName, ntupleTitle);
        analysisManager->CreateNtupleDColumn("fEdep");  // Energy deposited in this detector
        analysisManager->FinishNtuple(i);
        analysisManager->CreateH1(ntupleName, ntupleTitle, fNbins, 0.01, 2.);
    }
}

void RunAction::EndOfRunAction(const G4Run* run)
{  
    timer->Stop();
    G4cout << "=== Run time: " << timer->GetRealElapsed() << " s ===" << G4endl;

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile(false);
    
    if (csvFile.is_open()) {
    csvFile << csvBuffer.str();  // Write the entire buffer to the file in one operation
    csvFile.close();  // Close the file
    
}

    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;
	
if (G4Threading::IsMasterThread()) {
    system(("cd .. && python3 poop.py " + strRunID.str()).c_str());
}

}

G4int RunAction::GetNumberOfDetectors()
{
    const DetectorConstruction* detectorConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    return detectorConstruction->GetNumberOfDetectors();
}


