#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "physics.hh"
#include "construction.hh"
#include "action.hh"
//#include "G4ScoringManager.hh"

int main(int argc, char** argv){

G4MTRunManager *runManager = new G4MTRunManager();
runManager->SetUserInitialization(new PhysicsList());
runManager->SetUserInitialization(new DetectorConstruction());
runManager->SetUserInitialization(new ActionInitialization());

G4UIExecutive *UI = 0;

if(argc ==1){

	UI = new G4UIExecutive(argc, argv);

}

G4VisManager *visManager = new G4VisExecutive();
visManager->Initialize();

G4UImanager *UImanager = G4UImanager::GetUIpointer();

if(UI){
	UImanager->ApplyCommand("/control/execute vis.mac");
	UI->SessionStart();
}
else{
	G4String command = "/control/execute ";
  	G4String fileName = argv[1];
  	UImanager->ApplyCommand(command+fileName);
}


return 0;
}



