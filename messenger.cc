#include "messenger.hh"
#include "run.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIdirectory.hh"

RunMessenger::RunMessenger(RunAction* runAction) : fRunAction(runAction)
{
	G4UIdirectory* runDir = new G4UIdirectory("/run/");
	runDir->SetGuidance("Run action controls");
	
	fSetNbinsCmd = new G4UIcmdWithAnInteger("/run/setNbins", this);
	fSetNbinsCmd->SetGuidance("Set number of histogram bins");
	fSetNbinsCmd->SetParameterName("Nbins", false);
	fSetNbinsCmd->SetDefaultValue(100);
}

RunMessenger::~RunMessenger()
{
	delete fSetNbinsCmd;
}

void RunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if (command == fSetNbinsCmd) {
		fRunAction->SetNbins(fSetNbinsCmd->GetNewIntValue(newValue));
	}
}
