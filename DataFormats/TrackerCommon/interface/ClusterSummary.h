// -*- C++ -*-
//
// Package:    ClusterSummary
// Class:      ClusterSummary
// 
/**\class ClusterSummary ClusterSummary.cc msegala/ClusterSummary/src/ClusterSummary.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Michael Segala
//         Created:  Wed Feb 23 17:36:23 CST 2011
//
//

#ifndef CLUSTERSUMMARY
#define CLUSTERSUMMARY

// system include files
#if !defined(__CINT__) && !defined(__MAKECINT__) && !defined(__REFLEX__)
#include <atomic>
#endif
#include <memory>
#include <string>
#include <map>
#include <vector>
#include<iostream>
#include <string.h>
#include <sstream>
#include "FWCore/Utilities/interface/Exception.h"

// user include files

#include "DataFormats/SiStripCluster/interface/SiStripCluster.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"


#include "DataFormats/SiStripDigi/interface/SiStripProcessedRawDigi.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/DetId/interface/DetId.h" 
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"

/*****************************************************************************************

How to use ClusterSummary class:

ClusterSummary provides summary inforation for a number of cluster dependent variables.
All the variables are stored within variables_
The modules selected are stored within modules_
The number of variables for each module is stored within iterator_

********************************************************************************************/


class ClusterSummary {
public:

  // Holds information for the standard module selections
  struct CMSTrackerSelection{
    std::string name;
    std::vector<std::string> selection;
  };
  
  ClusterSummary();
  //nSelections is the number of selections that you want to have
  //It should be highest enum + 1
  ClusterSummary(const int nSelections);
  ~ClusterSummary() {}
  // copy ctor
  ClusterSummary(const ClusterSummary& src);
  // copy assingment operator
  ClusterSummary& operator=(const ClusterSummary& rhs);
#if !defined(__CINT__) && !defined(__MAKECINT__) && !defined(__REFLEX__)
  ClusterSummary(ClusterSummary&& other);
#endif

  // Enum for each partition within Tracker
  enum CMSTracker {STRIP = 0, TIB = 1, TOB = 2, TID = 3, TEC = 4, PIXEL = 5, FPIX = 6, BPIX = 7, NDEFAULTENUMS = 8, NTRACKERENUMS = 100};

  // Enum which describes the ordering of the summary variables inside vector variables_
  enum VariablePlacement{NCLUSTERS, CLUSTERSIZE, CLUSTERCHARGE, NVARIABLES};

  //Functions to return the standard subdetector selections
  static std::vector<CMSTrackerSelection> getStandardSelections(); //returns the standard selection info
  static const CMSTrackerSelection& getStandardSelection(const CMSTracker subdet) {
    static std::vector<CMSTrackerSelection> selVec = getStandardSelections();
    return selVec.at(subdet);
  }
  static const std::string& getSubDetName(const CMSTracker subdet) {return getStandardSelection(subdet).name;}
  static const std::vector<std::string>& getSubDetSelection(const CMSTracker subdet) {return getStandardSelection(subdet).selection;}
  static CMSTracker getSubDetEnum(const std::string name);

  static std::string getVarName(const VariablePlacement var);

  //===================+++++++++++++========================
  //
  //                 Main methods to fill 
  //                      Variables
  //
  //===================+++++++++++++========================
  
  //These functions are broken into two categories. The standard versions take the enums as input and find the locations in the vector.
  //The ones labeled "byIndex" take the vector location as input
 public:
  int   getNClusByIndex     (const int mod) const {return nClus     .at(mod);}
  int   getClusSizeByIndex  (const int mod) const {return clusSize  .at(mod);}
  float getClusChargeByIndex(const int mod) const {return clusCharge.at(mod);}

  int   getNClus     (const CMSTracker mod) const {int pos = getModuleLocation(mod); return pos < 0 ? 0. : nClus     [pos];}
  int   getClusSize  (const CMSTracker mod) const {int pos = getModuleLocation(mod); return pos < 0 ? 0. : clusSize  [pos];}
  float getClusCharge(const CMSTracker mod) const {int pos = getModuleLocation(mod); return pos < 0 ? 0. : clusCharge[pos];}

  const std::vector<int>  & getNClusVector()      const {return nClus;}
  const std::vector<int>  & getClusSizeVector()   const {return clusSize;}
  const std::vector<float>& getClusChargeVector() const {return clusCharge;}

  std::vector<int>  & getNClusVector()      {return nClus;}
  std::vector<int>  & getClusSizeVector()   {return clusSize;}
  std::vector<float>& getClusChargeVector() {return clusCharge;}

  void addNClusByIndex     (const int mod, const int   val) {nClus     .at(mod)+=val;}
  void addClusSizeByIndex  (const int mod, const int   val) {clusSize  .at(mod)+=val;}
  void addClusChargeByIndex(const int mod, const float val) {clusCharge.at(mod)+=val;}

  void addNClus     (const CMSTracker mod, const int   val) {nClus     .at(getModuleLocation(mod))+=val;}
  void addClusSize  (const CMSTracker mod, const int   val) {clusSize  .at(getModuleLocation(mod))+=val;}
  void addClusCharge(const CMSTracker mod, const float val) {clusCharge.at(getModuleLocation(mod))+=val;}

  const std::vector<int>& getModules() const { return modules;  }
  std::vector<int>& getModules() { return modules;  }
  // Return the location of desired module within modules_. If warn is set to true, a warnign will be outputed in case no module was found
  int getModuleLocation ( int mod, bool warn = true ) const;
  unsigned int getNumberOfModules() const {return modules.size();}
  int getModule(const int index) const { return modules[index];}

  //Resets the source values (set values to 0)
  //and copies over only non-zero entries into the current one
  void cleanAndReset(ClusterSummary& src);

 private:
  std::vector<int>   modules   ;    // <Module1, Module2 ...>
  std::vector<int>   nClus     ;
  std::vector<int>   clusSize  ;
  std::vector<float> clusCharge;
};


#endif







