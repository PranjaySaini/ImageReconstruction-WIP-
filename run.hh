#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4UImanager.hh"
#include "detector.hh"
#include "construction.hh"
#include "G4ScoringManager.hh"
#include "G4VScoringMesh.hh"
#include "G4Threading.hh"
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include "G4Timer.hh"

class RunMessenger;

class RunAction : public G4UserRunAction
{
public:
    RunAction();  // Default to one detector for single setup
    ~RunAction();

    virtual void BeginOfRunAction(const G4Run* run) override;
    virtual void EndOfRunAction(const G4Run* run) override;
    std::ofstream csvFile;
    G4int GetNumberOfDetectors();
    std::ostringstream csvBuffer;
    
    void SetNbins(G4int n){ fNbins = n; }
    G4int GetNbins() const {return fNbins;}
    
private:
 	G4int fNbins;
 	RunMessenger* fMessenger;  
 	G4Timer* timer;


};

#endif




