#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4RadioactiveDecay.hh"
#include "G4DecayTable.hh"
#include "G4NuclideTable.hh"
#include "G4GenericMessenger.hh"
#include "G4PhysicalConstants.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4ProductionCuts.hh"
#include "G4ProductionCutsTable.hh"
#include "G4TransportationManager.hh"
#include"G4GeometryManager.hh"
#include "detector.hh"
#include "G4UserLimits.hh"
#include "G4ScoringManager.hh"
#include "G4VScoringMesh.hh"
#include "G4Box.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4LogicalBorderSurface.hh"
#include <cstdlib>
#include <ctime>
#include "G4VisAttributes.hh"


class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	~DetectorConstruction();
	
	virtual G4VPhysicalVolume *Construct();
	G4int GetNumberOfDetectors() const;
        

	
private:
	void DefineMaterials();
	void CreateWorld();
	void CreateScintillator();
        void ConstructSDandField();
        void CreateScintillatorGrid();
        G4bool createDetector, createDetectorGrid; 
	G4Material *worldMat, *NaI;
	G4Element *Na, *I; 
	G4double worldX, worldY, worldZ;
	G4Box *solidWorld, *solidScin;
	G4LogicalVolume *logicWorld, *logicScin;
	G4VPhysicalVolume *physWorld, *physScin;
	G4OpticalSurface *mirrorSurface;
	G4GenericMessenger *fMessenger;
};

#endif
