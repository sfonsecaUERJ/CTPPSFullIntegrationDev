import FWCore.ParameterSet.Config as cms

RPixDetDigitizer = cms.EDProducer("CTPPSPixelDigiProducer",

    # all distances in [mm]
    # RPDigiProducer
    ROUList = cms.vstring('CTPPSPixelHits'),
    RPixVerbosity = cms.int32(0),
    CTPPSPixelDigiSimHitRelationsPersistence = cms.bool(False), # save links betweend digi, clusters and OSCAR/Geant4 hits

    # RPDetDigitizer
  RPixEquivalentNoiseCharge = cms.double(1000.0),
  RPixNoNoise = cms.bool(False),

    # RPDisplacementGenerator
  #  RPDisplacementOn = cms.bool(False),

    # RPLinearChargeCollectionDrifter
    RPixGeVPerElectron = cms.double(3.61e-09),
    RPixInterSmearing = cms.vdouble(0.011),

    # RPLinearChargeDivider
   RPixLandauFluctuations = cms.bool(True),
#   RPChargeDivisionsPerStrip = cms.int32(15),
#   RPChargeDivisionsPerThickness = cms.int32(5),
   RPixChargeDivisions = cms.int32(20),
   RPixDeltaProductionCut = cms.double(0.120425),    # [MeV]

    # RPixLinearInduceCharge
#    ChargeMapFile = cms.string("/afs/cern.ch/user/n/nogima/CMSSW_7_5_0/src/CNM_CT_PPS_10_4_ChargeMap_Angle_0_norm.root"),
    ChargeMapFile = cms.string("CNM_CT_PPS_10_4_ChargeMap_Angle_0_norm.root"),
    RPixCoupling = cms.double(0.135), # fraction of the remaining charge going to the closer neighbour pixel. Value = 0.135, Value = 0.0 bypass the charge map and the charge sharing approach 

    # RPixDummyROCSimulator
  
#  RPixDummyROCThreshold = cms.double(2500.0),
   RPixDummyROCThreshold = cms.double(1900.0),
   RPixDummyROCElectronPerADC = cms.double(210.0),   # 210.0 to be verified 
   RPixDeadPixelProbability = cms.double(0.001),
   RPixDeadPixelSimulationOn = cms.bool(False),

    # CTPPSPixelSimTopology
 #   RPSharingSigmas = cms.double(5.0), # how many sigmas taken into account for the edges and inter strips
  #  RPTopEdgeSmearing = cms.double(0.011),
  #  RPBottomEdgeSmearing = cms.double(0.011),
    RPixActiveEdgeSmearing = cms.double(0.020),
    RPixActiveEdgePosition = cms.double(0.150),   # from the physical edge
 #   RPTopEdgePosition = cms.double(1.5),
  #  RPBottomEdgePosition = cms.double(1.5)
)
