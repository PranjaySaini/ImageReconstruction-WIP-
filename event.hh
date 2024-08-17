#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "run.hh"
#include "detector.hh"
#include <map>

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction*);
    ~EventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    void AddEdep(G4int, G4double);


    const std::map<G4int, G4double>& GetEdepMap() const { return fEdepMap; }
    
private:
    std::map<G4int, G4double> fEdepMap;

};

#endif


