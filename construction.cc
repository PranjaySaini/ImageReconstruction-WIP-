#include "construction.hh"

DetectorConstruction::DetectorConstruction() 
{
  
  DefineMaterials();
  createDetector = false;
  createDetectorGrid = true;
}

DetectorConstruction::~DetectorConstruction()
{}


G4VPhysicalVolume *DetectorConstruction::Construct()
{

  CreateWorld();
   if (createDetectorGrid) {
        CreateScintillatorGrid();  // Build the grid if the flag is set
    } else if (createDetector) {
        CreateScintillator();  // Build a single detector if the flag is set
    }
  return physWorld;
}

G4int DetectorConstruction::GetNumberOfDetectors() const
{
    if (createDetectorGrid) {
        return 16;  // 4x4 grid
    } else if (createDetector) {
        return 1;   // Single detector
    } else {
        return 0;   // No detectors
    }
}

void DetectorConstruction::DefineMaterials()
{
  G4NistManager *nist;
  nist = G4NistManager::Instance();
  worldMat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4double energy[2] = {1.239841939*eV/0.9,1.239841939*eV/0.2};
  G4double rindexWorld[2] = {1.0,1.0};
  G4double rindexNaI[2] = {1.78,1.78};
  G4double reflectivity[2] = {1.0,1.0};
  
  G4double fraction[2] = {1.0, 1.0};
	
  Na = nist->FindOrBuildElement("Na");
  I = nist->FindOrBuildElement("I");
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
  
  G4double birksConstant = 0.126 * mm/MeV; // Example value for NaI
  NaI->GetIonisation()->SetBirksConstant(birksConstant);
  NaI->SetMaterialPropertiesTable(mptNaI);
  
  G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
  mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
  
  worldMat->SetMaterialPropertiesTable(mptWorld);
  
  G4double atomicNumber = 1.;
  G4double massOfMole = 1.01 * g/mole;
  G4double density = universe_mean_density; // Very low density
  G4double temperature = 2.73 * kelvin;
  G4double pressure = 3.e-18 * pascal;
  
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
  worldX = 1*m;
  worldY = 1*m;
  worldZ = 1*m;
  
  solidWorld = new G4Box("solidWorld", worldX*0.5, worldY*0.5, worldZ*0.5);
  logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld,"physWorld", 0, false, 0, true);  
}



void DetectorConstruction::CreateScintillator()
{
    solidScin = new G4Box("solidScintillator", 25*cm, 25*cm, 0.5*cm);
    logicScin = new G4LogicalVolume(solidScin, NaI, "logicScintillator");
    G4double maxStep = 1.0*mm;
    logicScin->SetUserLimits(new G4UserLimits(maxStep));

    // Physical placement of scintillator
    physScin = new G4PVPlacement(0, G4ThreeVector(0, 0, 39.5*cm), logicScin, "physScintillator", logicWorld, false, 0, true);

    // Define the skin surface for optical handling
    new G4LogicalSkinSurface("ScintillatorSkin", logicScin, mirrorSurface);

    // Define the border surface between scintillator and world volume
    new G4LogicalBorderSurface("ScintillatorWorldBorder", physScin, physWorld, mirrorSurface);
}

void DetectorConstruction::CreateScintillatorGrid()
{
    solidScin = new G4Box("solidScintillator", 2*cm, 2*cm, 0.5*cm);
    logicScin = new G4LogicalVolume(solidScin, NaI, "logicScintillator");
    G4double maxStep = 1.0*mm;
    logicScin->SetUserLimits(new G4UserLimits(maxStep));

    // Define the skin surface for each scintillator
    new G4LogicalSkinSurface("ScintillatorSkin", logicScin, mirrorSurface);

    // Place the scintillators in a grid
    for (G4int i = 0; i < 4; i++) {
        for (G4int j = 0; j < 4; j++) {
            physScin = new G4PVPlacement(0, G4ThreeVector((-5.25 + j*4.5)*cm, (5.25 - i*4.5)*cm, 29.5*cm),
                                         logicScin, "physScintillator", logicWorld, false, 4*i + j, true);

            // Define a border surface between each scintillator and the world
            new G4LogicalBorderSurface("ScintillatorWorldBorder_" + std::to_string(4*i + j), physScin, physWorld, mirrorSurface);
        }
    }
    G4VisAttributes* visScintillator = new G4VisAttributes(G4Colour(0., 0.1, 0., 0.3));
    visScintillator->SetVisibility(true);
    visScintillator->SetForceWireframe(false);
    visScintillator->SetForceSolid(true);
    logicScin->SetVisAttributes(visScintillator);
}



void DetectorConstruction::ConstructSDandField()
{
    // Create the MultiFunctionalDetector for scoring
    auto scintillatorDetector = new G4MultiFunctionalDetector("ScintillatorDetector");
    G4SDManager::GetSDMpointer()->AddNewDetector(scintillatorDetector);
    
    // Define an energy deposit scorer and register it to the detector
    auto energyDepositScorer = new G4PSEnergyDeposit("Edep");
    scintillatorDetector->RegisterPrimitive(energyDepositScorer);
    
    // Attach the MFD to the scintillator logical volume
    SetSensitiveDetector("logicScintillator", scintillatorDetector);
}
