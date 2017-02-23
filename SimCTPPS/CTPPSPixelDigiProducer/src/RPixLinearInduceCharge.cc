#include "SimCTPPS/CTPPSPixelDigiProducer/interface/RPixLinearInduceCharge.h"
#include <iostream>
#include "TFile.h"
#include "TH2D.h"

RPixLinearInduceCharge::RPixLinearInduceCharge(const edm::ParameterSet &params, uint32_t det_id)
 : det_id_(det_id), theRPixDetTopology_(params), sqrt_2(sqrt(2.0))
{
  verbosity_ = params.getParameter<int>("RPixVerbosity");
  signalCoupling_.clear();
  ChargeMapFile_ = params.getParameter<string>("ChargeMapFile");
// di default =1
  double coupling_constant_ = params.getParameter<double>("RPixCoupling");
  signalCoupling_.push_back(coupling_constant_);
  signalCoupling_.push_back( (1.0-coupling_constant_)/2 );
  
  no_of_pixels_ = theRPixDetTopology_.DetPixelNo();
 fChargeMap = new TFile(ChargeMapFile_.c_str());
 hChargeMap = (TH2D*)fChargeMap->Get("gChargeMap_Angle_0");
}

std::map<unsigned short, double, std::less<unsigned short> >  RPixLinearInduceCharge::Induce(
    const std::vector<RPixSignalPoint> &charge_map)
{
  thePixelChargeMap.clear();
  if(verbosity_)
    std::cout<<"RPixLinearInduceCharge "<<det_id_<<" : Clouds to be induced= "<<charge_map.size()<<std::endl;
  for(std::vector<RPixSignalPoint>::const_iterator i=charge_map.begin(); 
      i!=charge_map.end(); ++i)
  {
    double hit_pos_x,hit_pos_y;
// Used to avoid the obort due to hits out of detector 

    if (((*i).X()+16.6/2)<0||((*i).X()+16.6/2)>16.6) {
      cout << "**** Atention ((*i).X()+simX_width_/2.)<0||((*i).X()+simX_width_/2.)>simX_width  " << endl;
      cout << "(*i).X() = " << (*i).X() << endl;
      continue;
    }
    if (((*i).Y()+24.4/2.)<0||((*i).Y()+24.4/2.)>24.4) {
      cout << "**** Atention ((*i).Y()+simY_width_/2.)<0||((*i).Y()+simY_width_/2.)>simY_width  " << endl;
      cout << "(*i).Y() = " << (*i).Y() << endl;
      continue;
    }

    std::vector<pixel_info> relevant_pixels = theRPixDetTopology_.GetPixelsInvolved(
    	(*i).X(), (*i).Y(), (*i).Sigma(), hit_pos_x, hit_pos_y);
    if(verbosity_)
    {
      std::cout<<"RPixLinearInduceCharge "<<det_id_<<" : relevant_pixels = "
        <<relevant_pixels.size()<<std::endl;
    }
    for(std::vector<pixel_info>::const_iterator j = relevant_pixels.begin(); 
        j!=relevant_pixels.end(); ++j)
    {
/* UNUSED VARIABLES
      double pixel_begin_x = (*j).LowerSimXBorder();
      double pixel_end_x = (*j).HigherSimXBorder();
      double pixel_begin_y = (*j).LowerSimYBorder();
      double pixel_end_y = (*j).HigherSimYBorder();
      double sigma = (*i).Sigma();
*/
      double effic = (*j).EffFactor();
 
      unsigned short pixel_no = (*j).PixelIndex();
      
      double charge_in_pixel =  (*i).Charge()*effic;
      if(verbosity_)
        std::cout<<"Efficiency in detector "<<det_id_<< " and pixel no " << pixel_no << "  : " <<effic<<std::endl;

//        QUI SI POTREBBE INTRODURRE IL CHARGE SHARING TRA I PIXELS ..................................       
      if (signalCoupling_[0]==0.){
          thePixelChargeMap[pixel_no] += charge_in_pixel;
      } else {
       int pixel_row = (*j).PixelRowNo();
       int pixel_col = (*j).PixelColNo();
       double pixel_lower_x=0;
       double pixel_lower_y=0;
       double pixel_upper_x=0;
       double pixel_upper_y=0;
       double pixel_center_x=0;
       double pixel_center_y=0;

       theRPixDetTopology_.PixelRange(pixel_row,pixel_col,pixel_lower_x,pixel_upper_x,pixel_lower_y,pixel_upper_y);
       pixel_center_x = pixel_lower_x + (pixel_upper_x - pixel_lower_x)/2.;
       pixel_center_y = pixel_lower_y + (pixel_upper_y - pixel_lower_y)/2.;

        double hit2neighbour[8];
        double collect_prob = hChargeMap->GetBinContent(hChargeMap->FindBin(((*i).Y()-pixel_center_y)*1.e3,((*i).X()-pixel_center_x)*1.e3));
       thePixelChargeMap[pixel_no] += charge_in_pixel*collect_prob;
        unsigned short neighbour_no[8];
        unsigned short m=0;
        double closer_neighbour=0;
	unsigned short closer_no=0;
//      Considering the 8 neighbours to share charge
        for (int k = pixel_row - 1; k <= pixel_row + 1; k++){
          for (int l = pixel_col - 1; l <= pixel_col + 1; l++){
           if ((k<0)||k>159||l<0||l>155) continue;
           if ((k==pixel_row) && (l==pixel_col)) continue;
           double neighbour_pixel_lower_x=0;
           double neighbour_pixel_lower_y=0;
           double neighbour_pixel_upper_x=0;
           double neighbour_pixel_upper_y=0;
           double neighbour_pixel_center_x=0;
           double neighbour_pixel_center_y=0;
//         Check the hit approach to the neighbours
           theRPixDetTopology_.PixelRange(k,l,neighbour_pixel_lower_x,neighbour_pixel_upper_x,neighbour_pixel_lower_y,neighbour_pixel_upper_y);
           neighbour_pixel_center_x = neighbour_pixel_lower_x + 
                 (neighbour_pixel_upper_x - neighbour_pixel_lower_x)/2.;
           neighbour_pixel_center_y = neighbour_pixel_lower_y + 
                 (neighbour_pixel_upper_y - neighbour_pixel_lower_y)/2.;
           hit2neighbour[m] = 1/sqrt(pow((*i).X()-neighbour_pixel_center_x,2.)+pow((*i).Y()-neighbour_pixel_center_y,2.));
           neighbour_no[m]=l*160+k;
           if (hit2neighbour[m]>closer_neighbour){
		closer_neighbour=hit2neighbour[m];
		closer_no = neighbour_no[m];
	   }
            m++;
          }
        }
          double chargetransfereff = (1-collect_prob)*signalCoupling_[0];
          thePixelChargeMap[closer_no] += charge_in_pixel*chargetransfereff;
      }
    }
  }
  return thePixelChargeMap;
}
