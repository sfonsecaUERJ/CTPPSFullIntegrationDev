import FWCore.ParameterSet.Config as cms

rechitProd = cms.EDProducer("CTPPSPixelRecHitProducer",
                                     label=cms.untracked.string("clusterProd"),
                                     RPixVerbosity = cms.int32(0)
)
