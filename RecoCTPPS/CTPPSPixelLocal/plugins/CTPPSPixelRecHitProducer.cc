
#include "RecoCTPPS/CTPPSPixelLocal/interface/CTPPSPixelRecHitProducer.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


//needed for the geometry:
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"


#include <vector>
#include <memory>
#include <string>
#include <iostream>
//#include <cstdlib> // I need it for random numbers

CTPPSPixelRecHitProducer::CTPPSPixelRecHitProducer(const edm::ParameterSet& conf) :
  param_(conf), cluster2hit_(conf)
{
  
  src_ = conf.getUntrackedParameter<std::string>("label");
  verbosity_ = conf.getParameter<int> ("RPixVerbosity");
	 
  tokenCTPPSPixelCluster_ = consumes<edm::DetSetVector<CTPPSPixelCluster> >(edm::InputTag(src_));
 
  produces<edm::DetSetVector<CTPPSPixelRecHit> > ();
}

CTPPSPixelRecHitProducer::~CTPPSPixelRecHitProducer() {

}


// ------------ method called to produce the data  ------------
void CTPPSPixelRecHitProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
   
  std::cout << "  ****************************************************** " << std::endl;
  std::cout << "  ****        Inside CTPPSPixelRecHitProducer        ***** " << std::endl;
  std::cout << "  ****************************************************** " << std::endl;

// Step A: get inputs

	edm::Handle<edm::DetSetVector<CTPPSPixelCluster> > rpCl;
	iEvent.getByToken(tokenCTPPSPixelCluster_, rpCl);

 // get geometry
 //----------------------------------

	edm::ESHandle<TotemRPGeometry> geometry;
	iSetup.get<VeryForwardMeasuredGeometryRecord>().get(geometry);

	geometryWatcher.check(iSetup);


// dry checks to be removed
	unsigned int rpId = 2031091712;//  1997537280;
//	double z0 = geometry->GetRPDevice(rpId)->translation().z();
	CLHEP::Hep3Vector localV(-4.43825,2.05224,0.115);
	CLHEP::Hep3Vector globalV = geometry->LocalToGlobal(rpId,localV);

//	std::cout << " z0 = " << z0 <<std::endl;
	std::cout << "id: "<< rpId <<"   local " << localV <<"   to global "<<globalV<< std::endl;

//---------------------------------------------



	edm::DetSetVector<CTPPSPixelRecHit>  output;

// run reconstruction
	if (rpCl->size())
	  run(*rpCl, output);

        // Step D: write output to file
       
	iEvent.put(std::make_unique<edm::DetSetVector<CTPPSPixelRecHit> >(output));

}

void CTPPSPixelRecHitProducer::run(const edm::DetSetVector<CTPPSPixelCluster> &input, edm::DetSetVector<CTPPSPixelRecHit> &output){

  for (const auto &ds_cluster : input)
    {
      edm::DetSet<CTPPSPixelRecHit> &ds_rechit = output.find_or_insert(ds_cluster.id);


// call here an RPix CPE to calculate the cluster parameters and, maybe, to convert it into a rechit 
         cluster2hit_.buildHits(ds_cluster.id, ds_cluster.data, ds_rechit.data);



	
//-----------------------------------
      unsigned int rechitN=0;
      for(std::vector<CTPPSPixelRecHit>::iterator iit = ds_rechit.data.begin(); iit != ds_rechit.data.end(); iit++){
	
	if(verbosity_)	std::cout << "Rechit " << ++rechitN <<" x " << (*iit).getPoint().x()<< " y " << (*iit).getPoint().y()<< " z " <<  (*iit).getPoint().z() << std::endl;





      }

      if(verbosity_)	std::cout << "Rechit Number = " << rechitN << std::endl; 

//-----------------------------------
    }
}

DEFINE_FWK_MODULE( CTPPSPixelRecHitProducer);
