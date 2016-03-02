////////////////////////////////////////////////////////////////////////
// Class:       PiZeroAna
// Module Type: analyzer
// File:        PiZeroAna_module.cc
//
// Generated at Wed Mar  2 09:44:33 2016 by Matthew Toups using artmod
// from cetpkgsupport v1_10_01.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "PiZeroROI/PiZeroROI.hh"

class PiZeroAna;

class PiZeroAna : public art::EDAnalyzer {
public:
  explicit PiZeroAna(fhicl::ParameterSet const & p);
  // The destructor generated by the compiler is fine for classes
  // without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  PiZeroAna(PiZeroAna const &) = delete;
  PiZeroAna(PiZeroAna &&) = delete;
  PiZeroAna & operator = (PiZeroAna const &) = delete;
  PiZeroAna & operator = (PiZeroAna &&) = delete;

  // Required functions.
  void analyze(art::Event const & e) override;
  void reconfigure(fhicl::ParameterSet const &p) override;

private:

  std::string fROIModuleLabel;
  // Declare member data here.

};

void PiZeroAna::reconfigure(fhicl::ParameterSet const &p) {
  fROIModuleLabel = p.get<std::string>("ROIModuleLabel","");
}

PiZeroAna::PiZeroAna(fhicl::ParameterSet const & p)
  :
  EDAnalyzer(p)  // ,
 // More initializers here.
{
  this->reconfigure(p);
}

void PiZeroAna::analyze(art::Event const & e)
{
  // Implementation of required member function here.
  std::cout << "Hello3" << std::endl;

  art::Handle<std::vector<ana::PiZeroROI> > Roi_h;
  e.getByLabel( fROIModuleLabel, Roi_h );

  if(!Roi_h.isValid()) throw std::exception();
  std::vector<ana::PiZeroROI> const& PiZeroROIVector(*Roi_h);
  std::cout << PiZeroROIVector.size() << std::endl;

  std::cout << "Hello4!" << std::endl;
  int fnROI = 0;
  for(auto Roi : PiZeroROIVector) {
    std::vector<std::pair<int, int> > vs = Roi.GetVertex();
    std::vector<std::pair<int, int> > ts = Roi.GetTimeROI();
    std::vector<std::pair<int, int> > ws = Roi.GetWireROI();
   
    std::cout << "=================================" << fnROI << std::endl;
    std::cout << "ROI number: " << fnROI << std::endl;
    for(size_t i = 0; i<vs.size(); ++i) {
      std::cout << "Plane: " << i << std::endl;
      std::cout << "Vertex: " << vs[i].first << " --> " << vs[i].second << std::endl;
      std::cout << "Time: " << ts[i].first << " --> " << ts[i].second << std::endl;
      std::cout << "Wire: " << ws[i].first << " --> " << ws[i].second << std::endl;
    }

    ++fnROI;

  }

}

DEFINE_ART_MODULE(PiZeroAna)
