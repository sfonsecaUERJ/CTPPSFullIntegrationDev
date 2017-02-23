/**********************************************************************
 *
 * Author: F.Ferro - INFN Genova
 * December 2016
 *
 **********************************************************************/
#ifndef RecoCTPPS_CTPPSPixelLocal_CTPPSPixelRecHitProducer
#define RecoCTPPS_CTPPSPixelLocal_CTPPSPixelRecHitProducer

#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSet.h"
//#include "DataFormats/Common/interface/DetSetVectorNew.h"
//#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
 
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"


#include "DataFormats/CTPPSReco/interface/CTPPSPixelCluster.h"
#include "DataFormats/CTPPSReco/interface/CTPPSPixelRecHit.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "RecoCTPPS/CTPPSPixelLocal/interface/RPixCluster2Hit.h" 

#include "FWCore/Framework/interface/ESWatcher.h"
#include "Geometry/VeryForwardGeometryBuilder/interface/TotemRPGeometry.h"
#include "Geometry/VeryForwardRPTopology/interface/RPTopology.h"
#include "Geometry/Records/interface/VeryForwardRealGeometryRecord.h"


#include <vector>
#include <set>



class CTPPSPixelRecHitProducer : public edm::stream::EDProducer<>
{
public:
  explicit CTPPSPixelRecHitProducer(const edm::ParameterSet& param);
 
  ~CTPPSPixelRecHitProducer();

 virtual void produce(edm::Event&, const edm::EventSetup&) override;

private:
 edm::ParameterSet param_;
 int verbosity_;
 
 edm::InputTag src_;
 edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelCluster>> tokenCTPPSPixelCluster_;
  
 edm::ESWatcher<VeryForwardRealGeometryRecord> geometryWatcher;

 RPixCluster2Hit cluster2hit_;
  
 void run(const edm::DetSetVector<CTPPSPixelCluster> &input, edm::DetSetVector<CTPPSPixelRecHit> &output);
 
};



#endif
