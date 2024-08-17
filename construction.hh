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
#include "detector.hh"
#include <cstdlib>
#include <ctime>

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	~DetectorConstruction();
	
	G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}
	virtual G4VPhysicalVolume *Construct();
	void CreateDetectorGrid();
	void CreateDetector();
	G4int GetNumberOfDetectors() const;
	
private:
        G4LogicalVolume *fScoringVolume;
        G4GenericMessenger *fMessenger;
        G4bool createHole, createTarget, createTube, createDetector, createDetectorGrid, createShield;
	virtual void ConstructSDandField();
	void DefineMaterials();
	void CreateWorld();
	void CreateShield();
	void CreateTarget();
	void CreateShieldWithHole();

	void CreateScintillator();
	void CreateScintillatorGrid();
	void CreateXRayTube();
	G4SubtractionSolid* finalShield;
	G4Material *worldMat, *shieldMat, *targetMat, *tubeMat, *NaI, *Pb;
	G4Element *Na, *I; //*Pb
	G4double worldX, worldY, worldZ, shieldThickness;
	G4Box *solidWorld, *solidShield, *solidTarget, *emptyHole, *solidDetector, *solidTube, *solidScin;
	G4LogicalVolume *logicWorld, *logicShield, *logicTarget, *logicDetector, *logicTube, *logicScin;
	G4VPhysicalVolume *physWorld, *physShield, *physTarget, *physDetector, *physTube, *physScin;
	G4OpticalSurface *mirrorSurface;
};

#endif
