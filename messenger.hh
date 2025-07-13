#ifndef MESSENGER_HH
#define MESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class RunAction;
class G4UIcmdWithAnInteger;

class RunMessenger : public G4UImessenger {
public:
	RunMessenger(RunAction*);
	~RunMessenger();
	
	virtual void SetNewValue(G4UIcommand*, G4String);
	
private:
	RunAction* fRunAction;
	G4UIcmdWithAnInteger* fSetNbinsCmd;
};

#endif
