#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "run.hh"
#include "detector.hh"
#include "stepping.hh"
#include <map>
#include <vector>
#include <utility> // for std::pair
#include "G4THitsMap.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction*);
    ~EventAction();
    
    virtual void BeginOfEventAction(const G4Event*) override; 
    virtual void EndOfEventAction(const G4Event*) override;
    
private:
	G4int fPrintModulo;
};

#endif

