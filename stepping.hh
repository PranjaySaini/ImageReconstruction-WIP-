#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <map>
#include <cmath>

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction();
    ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step) override;

    void ResetEventEnergy();                         // Reset all detectors' energy at the start of each event
    const std::map<int, double>& GetEventEnergy() const;  // Retrieve the map of energy deposits
   

private:
    std::map<int, double> fEventEnergy;  // Map of energy per detector
     G4double a, b, c;
};

#endif

