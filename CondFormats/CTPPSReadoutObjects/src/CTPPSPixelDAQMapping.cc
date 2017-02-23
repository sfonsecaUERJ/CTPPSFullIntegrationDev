/****************************************************************************
*
* 
* Authors: 
* F.Ferro ferro@ge.infn.it
*
****************************************************************************/

#include "FWCore/Utilities/interface/typelookup.h"

#include "CondFormats/CTPPSReadoutObjects/interface/CTPPSPixelDAQMapping.h"

using namespace std;

//----------------------------------------------------------------------------------------------------

std::ostream& operator << (std::ostream& s, const CTPPSPixelROCInfo &vi)
{
  s << "ID="<< vi.iD << "  ROC=" << vi.roc;

  return s;
}

//----------------------------------------------------------------------------------------------------

void CTPPSPixelDAQMapping::insert(const CTPPSPixelFramePosition &fp, const CTPPSPixelROCInfo &vi)
{
  auto it = ROCMapping.find(fp);  
  if (it != ROCMapping.end())
  {
    cerr << "WARNING in DAQMapping::insert > Overwriting entry at " << fp << ". Previous: " << endl 
      << "    " << ROCMapping[fp] << "," << endl << "  new: " << endl << "    " << vi << ". " << endl;
  }

  ROCMapping[fp] = vi;
}

//----------------------------------------------------------------------------------------------------

TYPELOOKUP_DATA_REG(CTPPSPixelDAQMapping);
