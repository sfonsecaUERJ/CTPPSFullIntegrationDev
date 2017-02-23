#ifndef CTPPS_TRACKER_DIGI_H
#define CTPPS_TRACKER_DIGI_H


#include <utility>
#include <algorithm>
#include "DataFormats/CTPPSDetId/interface/RPixPacking.h"    // for packing algorithm

/**
 * Persistent digi for the Pixels.
 */

class CTPPSPixelDigi {
public:

  typedef unsigned int PackedDigiType;
  typedef unsigned int ChannelType;

  CTPPSPixelDigi( int packed_value) : theData(packed_value) {}

  CTPPSPixelDigi( int row, int col, int adc) {
    init( row, col, adc);
  }

  CTPPSPixelDigi( int chan, int adc) {
    std::pair<int,int> rc = channelToPixel(chan);
    init( rc.first, rc.second, adc);
  }

  CTPPSPixelDigi() : theData(0)  {}

  void init( int row, int col, int adc) {

  // Set adc to max_adc in case of overflow
  adc = (adc > RPixPacking::thePacking.max_adc) ? RPixPacking::thePacking.max_adc : std::max(adc,0);

  theData = (row << RPixPacking::thePacking.row_shift) |
    (col << RPixPacking::thePacking.column_shift) |
    (adc << RPixPacking::thePacking.adc_shift);

  }

  // Access to digi information
  int row() const     {return (theData >> RPixPacking::thePacking.row_shift) & RPixPacking::thePacking.row_mask;}
  int column() const  {return (theData >> RPixPacking::thePacking.column_shift) & RPixPacking::thePacking.column_mask;} 
  unsigned short adc() const  {return (theData >> RPixPacking::thePacking.adc_shift) & RPixPacking::thePacking.adc_mask;}
  PackedDigiType packedData() const {return theData;}

  static std::pair<int,int> channelToPixel( int ch) {
    int row = ( ch >> RPixPacking::thePacking.column_width) & RPixPacking::thePacking.row_mask;
    int col = ch & RPixPacking::thePacking.column_mask;
    return std::pair<int,int>(row,col);
  }

  static int pixelToChannel( int row, int col) {
    return (row << RPixPacking::thePacking.column_width) | col;
  }

  int channel() const {return RPixPacking::pixelToChannel( row(), column());}

 private:
  PackedDigiType theData;
};  

// Comparison operators

//inline bool operator<( const CTPPSPixelDigi& one, const CTPPSPixelDigi& other) {
//  return one.channel() < other.channel();
//}

inline bool operator<( const CTPPSPixelDigi& one, const CTPPSPixelDigi& other) {
  return (one.packedData()&RPixPacking::thePacking.rowcol_mask) < (other.packedData()&RPixPacking::thePacking.rowcol_mask);
}

#include<iostream>
inline std::ostream & operator<<(std::ostream & o, const CTPPSPixelDigi& digi) {
  return o << " " << digi.channel()
	   << " " << digi.adc();
}

#endif
