#include "construction.hh"

DetectorConstruction::DetectorConstruction()
{
  fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");
  fMessenger->DeclareProperty("createHole", createHole, "Toggle hole in shield");
  //fMessenger->DeclareProperty("createTarget", createTarget, "Toggle tungsten target");
  //fMessenger->DeclareProperty("createTube", createTube, "Toggle X-Ray Tube");
  fMessenger->DeclareProperty("createShield", createShield, "Toggle lead shield");
  fMessenger->DeclareProperty("createSingleDetector", createDetector, "Use Single Detector");
  fMessenger->DeclareProperty("createDetectorGrid", createDetectorGrid, "Use Detector Grid");
  
  DefineMaterials();
  
  createHole = false;
  //createTarget = true;
  //createTube = true;
  createShield = false;
  createDetector = false;
  createDetectorGrid = true;
}

DetectorConstruction::~DetectorConstruction()
{}

G4int DetectorConstruction::GetNumberOfDetectors() const
{
    if (createDetectorGrid) {
        return 16; // 4x4 grid
    } else if (createDetector) {
        return 1; // Single detector
    } else {
        return 0; // No detectors
    }
}


G4VPhysicalVolume *DetectorConstruction::Construct()
{

  CreateWorld();
  
  
  if(createShield){
  CreateShield();
  }
  else if(createHole){
  CreateShieldWithHole();
  }
  
  if(createDetector)
  {
  CreateDetector();
  //CreateScintillator();
  }
  else if(createDetectorGrid)
  {
  CreateDetectorGrid();
  //CreateScintillatorGrid();
  }
  
  fScoringVolume = logicDetector;
  return physWorld;
}


void DetectorConstruction::DefineMaterials()
{
  G4NistManager *nist;
  nist = G4NistManager::Instance();
  worldMat = nist->FindOrBuildMaterial("G4_AIR");
  shieldMat = nist->FindOrBuildMaterial("G4_Pb");
  targetMat = nist->FindOrBuildMaterial("G4_W");
  
  G4double energy[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
  G4double rindexWorld[2] = {1.0,1.0};
  G4double rindexNaI[2] = {1.78,1.78};
  G4double reflectivity[2] = {1.0,1.0};
  
  G4double fraction[2] = {1.0, 1.0};
	
  Na = nist->FindOrBuildElement("Na");
  I = nist->FindOrBuildElement("I");
  Pb = nist->FindOrBuildMaterial("G4_Pb");
  NaI = new G4Material("NaI", 3.67*g/cm3, 2);
  NaI-> AddElement(Na, 1);
  NaI-> AddElement(I, 1);
  
  G4MaterialPropertiesTable *mptNaI = new G4MaterialPropertiesTable();
  mptNaI->AddProperty("RINDEX", energy, rindexNaI, 2);
  mptNaI->AddProperty("SCINTILLATIONCOMPONENT1", energy, fraction, 2);
  mptNaI->AddConstProperty("SCINTILLATIONYIELD", 38./keV);
  mptNaI->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mptNaI->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 250*ns);
  mptNaI->AddConstProperty("SCINTILLATIONYIELD1", 1.);
	
  NaI->SetMaterialPropertiesTable(mptNaI);
  
  G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
  mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
  
  worldMat->SetMaterialPropertiesTable(mptWorld);
  
  G4double atomicNumber = 1.;
  G4double massOfMole = 1.01 * g/mole;
  G4double density = universe_mean_density; // Very low density
  G4double temperature = 2.73 * kelvin;
  G4double pressure = 3.e-18 * pascal;
   
  tubeMat = new G4Material("tubeMat", atomicNumber, massOfMole, density, kStateGas, temperature, pressure);
  
  mirrorSurface = new G4OpticalSurface("mirrorSurface");
        
  mirrorSurface->SetType(dielectric_metal);
  mirrorSurface->SetFinish(ground);
  mirrorSurface->SetModel(unified);
        
  G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
        
  mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity, 2);
        
  mirrorSurface->SetMaterialPropertiesTable(mptMirror);
                                           
}

void DetectorConstruction::CreateWorld()
{ 
  worldX = 35*cm;
  worldY = 35*cm;
  worldZ = 35*cm;
  
  solidWorld = new G4Box("solidWorld", worldX*0.5, worldY*0.5, worldZ*0.5);
  logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld,"physWorld", 0, false, 0, true);
}

void DetectorConstruction::CreateShield()
{
  shieldThickness = 5*cm;
  
  solidShield = new G4Box("soildShield", worldX*0.5, worldY*0.5, shieldThickness*0.5);
  logicShield = new G4LogicalVolume(solidShield, shieldMat, "logicShield");
  //skin1 = new G4LogicalSkinSurface("skin1", logicShield, mirrorSurface);
  physShield = new G4PVPlacement(0, G4ThreeVector(0., 0., 7.5*cm), logicShield, "physShield", logicWorld, false, 0, true);
  
  G4Region* leadRegion = new G4Region("LeadRegion");
  logicShield->SetRegion(leadRegion);
  leadRegion->AddRootLogicalVolume(logicShield);
}

void DetectorConstruction::CreateTarget()
{
  solidTarget = new G4Box("soildTarget", 2.5*cm, 2.5*cm, 0.05*cm);
  logicTarget = new G4LogicalVolume(solidTarget, targetMat, "logicTarget");
  physTarget = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicTarget, "physTarget", logicWorld, false, 0, true);
}

void DetectorConstruction::CreateShieldWithHole()
{
  shieldThickness = 5*cm;
  
  solidShield = new G4Box("soildShield", worldX*0.5, worldY*0.5, shieldThickness*0.5);
  emptyHole = new G4Box("emptyHole", 5.*cm, 5.*cm, shieldThickness*0.5);
  finalShield = new G4SubtractionSolid("finalShield", solidShield, emptyHole);
  logicShield = new G4LogicalVolume(finalShield, Pb, "logicShield");
  //skin2 = new G4LogicalSkinSurface("skin2", logicShield, mirrorSurface);
  physShield = new G4PVPlacement(0, G4ThreeVector(0., 0., 7.5*cm), logicShield, "physShield", logicWorld, false, 0, true);
  
  G4Region* leadRegion = new G4Region("LeadRegion");
  logicShield->SetRegion(leadRegion);
  leadRegion->AddRootLogicalVolume(logicShield);
}

void DetectorConstruction::CreateDetectorGrid()
{       
	
	solidDetector = new G4Box("solidDetector", 4.3375*cm, 4.3375*cm, 0.5*cm);
	logicDetector = new G4LogicalVolume(solidDetector, NaI, "logicDetector");
	G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirrorSurface);
	for (G4int i = 0; i<4; i++)
	{
	    for(G4int j = 0; j<4;j++)
	    {
	        physDetector = new G4PVPlacement(0, G4ThreeVector(-17.5*cm + (4.3375*cm + i*8.775*cm), -17.5*cm+(j*8.775*cm+4.3375*cm), 17*cm), logicDetector, "physDetector", logicWorld, false, j+i*4, true);
	    }
	}
}

void DetectorConstruction::CreateDetector()
{       	
	solidDetector = new G4Box("solidDetector", 17.5*cm, 17.5*cm, 0.5*cm);
	logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");
	physDetector = new G4PVPlacement(0, G4ThreeVector(0, 0, 17.*cm), logicDetector, "physDetector", logicWorld, false, 0, true);

}


void DetectorConstruction::CreateXRayTube()
{  
   
   solidTube = new G4Box("solidTube", 2.5*cm, 2.5*cm, 0.5*cm);
   logicTube = new G4LogicalVolume(solidTube, tubeMat, "logicTube");
   physTube = new G4PVPlacement(0, G4ThreeVector(0., 0., -0.5*cm), logicTube, "physTube", logicWorld, false, 0, true);
   
  solidTarget = new G4Box("soildTarget", 2.5*cm, 2.5*cm, 0.05*cm);
  logicTarget = new G4LogicalVolume(solidTarget, targetMat, "logicTarget");
  physTarget = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicTarget, "physTarget", logicWorld, false, 0, true);
}

void DetectorConstruction::CreateScintillatorGrid()
{
  solidScin = new G4Box("solidScin", 4*cm, 4*cm, 1*cm);
  logicScin = new G4LogicalVolume(solidScin, NaI, "logicScin");
  
  
  for (G4int i = 0; i<4; i++)
	{
	    for(G4int j = 0; j<4;j++)
	    {
	        physScin = new G4PVPlacement(0, G4ThreeVector(-17.5*cm + (4.3375*cm + i*8.775*cm), -17.5*cm+(j*8.775*cm+4.3375*cm), 15.5*cm), logicScin, "physScin", logicWorld, false, j+i*4, true);
	    }
	}
}

void DetectorConstruction::CreateScintillator()
{
  solidScin = new G4Box("solidScin", 17.5*cm, 17.5*cm, 1.*cm);
  logicScin = new G4LogicalVolume(solidScin, NaI, "logicScin");
  G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirrorSurface);
  
  physScin = new G4PVPlacement(0, G4ThreeVector(0, 0, 15.5*cm), logicScin, "physScin", logicWorld, false, 0, true);
}
	    
	


void DetectorConstruction::ConstructSDandField()
{
  SensitiveDetector *sensDet = new SensitiveDetector("SensitiveDetector");
  
  logicDetector->SetSensitiveDetector(sensDet);
}



