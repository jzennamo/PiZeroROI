////////////////////////////////////////////////////////////////////////
// Class:       PiZeroFilter
// Module Type: filter
// File:        PiZeroFilter_module.cc
//
// Generated at Tue Mar  1 14:26:33 2016 by Matthew Toups using artmod
// from cetpkgsupport v1_10_01.
////////////////////////////////////////////////////////////////////////
#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/FindManyP.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include <memory>

#include "art/Framework/Services/Optional/TFileService.h"

#include "PiZeroROI/PiZeroROI.hh"
#include "RecoBase/PFParticle.h"
#include "RecoBase/Vertex.h"
#include "RecoBase/Track.h"
#include "RecoBase/Cluster.h"

class PiZeroFilter;

class PiZeroFilter : public art::EDFilter {
public:
  explicit PiZeroFilter(fhicl::ParameterSet const & p);
  // The destructor generated by the compiler is fine for classes
  // without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  PiZeroFilter(PiZeroFilter const &) = delete;
  PiZeroFilter(PiZeroFilter &&) = delete;
  PiZeroFilter & operator = (PiZeroFilter const &) = delete;
  PiZeroFilter & operator = (PiZeroFilter &&) = delete;

  // Required functions.
  bool filter(art::Event & e) override;

  // Selected optional functions.
  void reconfigure(fhicl::ParameterSet const & p) override;

private:

  // Declare member data here.
  std::string fPFPModuleLabel;
  std::string fVertexModuleLabel;
  std::string fClusterModuleLabel;
  std::string fTrackModuleLabel;
  //ShowerModuleLabel:          "showerrecopandora"

  std::string fPFPVertexAssnModuleLabel;
  std::string fPFPClusterAssnModuleLabel;
  std::string fPFPTrackAssnModuleLabel;

  float fMuonTrackLengthCut;
  float fTrackVertexProximityCut;
  float fShowerVertex2dProximityCut;
  float fShowerDetached2dProximityCut;
  float fMinMinDetachedShowersPerPlaneCut;
  float fMinMaxDetachedShowersPerPlaneCut;
  float fPadding;

  TTree* fmytree;
  int fnVtx;
  int fnShw;
  int fnNuMuCC;
  int fnTrk;
};


PiZeroFilter::PiZeroFilter(fhicl::ParameterSet const & p)
  : fnVtx(0),
    fnShw(0),
    fnNuMuCC(0),
    fnTrk(0)
// Initialize member data here.
{
  this->reconfigure(p);
  art::ServiceHandle<art::TFileService> tfs;

  fmytree = tfs->make<TTree>("mytree","mytree");
  fmytree->Branch("fnVtx",&fnVtx,"fnVtx/I");
  fmytree->Branch("fnShw",&fnShw,"fnShw/I");
  fmytree->Branch("fnTrk",&fnTrk,"fnTrk/I");
  fmytree->Branch("fnNuMuCC",&fnNuMuCC,"fnNuMuCC/I");

  // Call appropriate produces<>() functions here.
  produces<std::vector<ana::PiZeroROI> >();
}

bool PiZeroFilter::filter(art::Event & e)
{
  bool pass = false;

  std::cout << "Hello" << std::endl;
  // Implementation of required member function here.
  std::unique_ptr<std::vector<ana::PiZeroROI> > pizeroroiVector( new std::vector<ana::PiZeroROI> );
  //std::unique_ptr< art::Assns<recob::Vertex, ana::PiZeroROI::PiZeroROI > >  assnPiZeroROITagVertex( new art::Assns<recob::Vertex, anab::PiZeroROI>);

  art::ValidHandle<std::vector<recob::PFParticle> > Pfp_h = e.getValidHandle<std::vector<recob::PFParticle> >(fPFPModuleLabel);
  art::ValidHandle<std::vector<recob::Vertex> > Vtx_h = e.getValidHandle<std::vector<recob::Vertex> >(fVertexModuleLabel);
  art::ValidHandle<std::vector<recob::Cluster> > Cls_h = e.getValidHandle<std::vector<recob::Cluster> >(fClusterModuleLabel);
  art::ValidHandle<std::vector<recob::Track> > Trk_h = e.getValidHandle<std::vector<recob::Track> >(fTrackModuleLabel);

  if(!(Pfp_h.isValid() && Vtx_h.isValid() && Cls_h.isValid() && Trk_h.isValid())) 
    throw std::exception();

  const art::FindManyP<recob::Vertex> PfpVtx(Pfp_h, e, fPFPVertexAssnModuleLabel);
  const art::FindManyP<recob::Cluster> PfpCls(Pfp_h, e, fPFPClusterAssnModuleLabel);
  const art::FindManyP<recob::Track> PfpTrk(Pfp_h, e, fPFPTrackAssnModuleLabel);
  
  std::vector<recob::PFParticle> const& PfpVector(*Pfp_h);
  std::vector<recob::Vertex> const& VtxVector(*Vtx_h);
  std::vector<recob::Cluster> const& ClsVector(*Cls_h);
  std::vector<recob::Track> const& TrkVector(*Trk_h);

  std::cout << "PFPVector size: " << PfpVector.size() << std::endl;
  std::cout << "VtxVector size: " << VtxVector.size() << std::endl;
  std::cout << "ClsVector size: " << ClsVector.size() << std::endl;
  std::cout << "TrkVector size: " << TrkVector.size() << std::endl;
  
  std::cout << "Hello!" << std::endl;

  std::vector< int > roi_cand_v;  

  std::vector<ana::PiZeroROI> pizeroroi_v;
  std::vector<std::pair<int,int> > Vertex(3);
  std::vector<std::pair<int,int> > TimePairs(3);
  std::vector<std::pair<int,int> > WirePairs(3);

  std::map<int,double> nuMuonMaxTrackLength;
  std::map<int,int> nuMuonMaxTrackLengthIndex;
  std::map<int,std::vector<float> > nuMuonStartWire;
  std::map<int,std::vector<float> > nuMuonStartTick;
  std::map<int,std::vector<float> > startw;
  std::map<int,std::vector<float> > startt;
  std::map<int,std::vector<float> > endw;
  std::map<int,std::vector<float> > endt;
  std::map<int,std::vector<int> > nDetachedShowers;
  // iterate through all the PFParticles 
  for(auto const Pfp : PfpVector) {
    
    // Select only if Primary PFParticle is a neutrino
    if(Pfp.IsPrimary() && (Pfp.PdgCode()==12 || Pfp.PdgCode()==14 || Pfp.PdgCode()==-12 || Pfp.PdgCode()==-14)) { // Nu

      std::cout << "PFParticle Primary Neutrino with Pfp.PdgCode() " << Pfp.PdgCode() << std::endl;
      // Get neutrino vertex info
      auto const & v_ps = PfpVtx.at(Pfp.Self());
      // Save PFparticle vtx in v_ps should be size 1 because 
      // a PFparticle should only have one vertex
      if(v_ps.size() != 1) { std::cout << "PFParticle has more than one vtx?!" << std::endl; }
      
      //Store the neutrino vtx information in temporary container
      double xyz_p[3] = {0.,0.,0.};
      for(auto const & v_p : v_ps) {
	v_p->XYZ(xyz_p);
	std::cout << "xyz_p [" << xyz_p[0]<< ", " << xyz_p[1] << ", " << xyz_p[2] << "]" << std::endl;
      }

      //Check that there are at least 1 track and two showers
      //Should make this a flag
      int nmcc = 0; //use that instead of boolean
      int trk = 0; int show = 0;
      for(auto const idx : Pfp.Daughters()) {
	if(PfpVector.at(idx).PdgCode() == 13) trk++;
	if(PfpVector.at(idx).PdgCode() == 11) show++;
	if(trk >= 1 && show >= 2) {
	  nmcc = 1;
	}
      }
      fnShw = show;
      fnTrk = trk;
      fnNuMuCC = nmcc;
      fmytree->Fill();
	
      //If Pandora does not find 1 track and two showers skip it 
      if(nmcc != 1){
	std::cout << "Event skipped - not 1 track and 2 showers" << std::endl; 
	continue;
	};
      //End "Should make this a flag"

      if(nmcc == 1){std::cout << "There are  "<<trk << " tracks and " << show << " showers present." << std::endl;};
      
      // Iteration through all PFParticle daughters 
      for(auto const idx : Pfp.Daughters()) {

	// Want to find a muon, which is defined as the longest track daughter within tolerance
	if(PfpVector.at(idx).PdgCode() == 13) { // Track-like object

	  //Checking the number of vertices associated with the track object
	  auto const & v_ds = PfpVtx.at(PfpVector.at(idx).Self());

	  if(v_ds.size() == 0) { 
	    //Skip the track if it has no vertex
	    continue;
	  } else if (v_ds.size() > 1) {
	    //Complain if the track contains more than one vertex
	    std::cout << "Daughter Track has more than one vertex!? " << v_ds.size() << std::endl;
	  }

	  //Store the track vtx information in temporary container	  
	  double xyz_d[3] = {0.,0.,0.};
	  for(auto const & v_d : v_ds) {
	    v_d->XYZ(xyz_d);
	    std::cout << "Track vertex at [" << xyz_d[0] << ", " << xyz_d[1] << ", " << xyz_d[2] << "]" << std::endl;
	  }

	  //Calculate the distance between the neutrino and track vertices
	  float dist = std::sqrt(std::pow(xyz_p[0]-xyz_d[0],2)+std::pow(xyz_p[1]-xyz_d[1],2)+std::pow(xyz_p[2]-xyz_d[2],2));

	  // Passes the track-Nu proximity threshold
	  if(dist<fTrackVertexProximityCut) { 

	    // Loop over proximate tracks to find longest track
	    auto const & trk_ds = PfpTrk.at(PfpVector.at(idx).Self());
	    // Make sure you actually grabbed a track
	    if(trk_ds.size() == 0) {
	      std::cout << "Failed to find the longest track!?" << std::endl;
	      continue;
	    } else if (trk_ds.size() > 1) {
	      // Check that you only grabbed one track
	      std::cout << "Grabbed " << trk_ds.size() << " Tracks instead of the longest track?!" << std::endl;
	    }

	    //Study the selected track in detail
	    for(auto const & trk_d : trk_ds) {

	      //measure the end-to-end track length
	      float trkl = (trk_d->Vertex()-trk_d->End()).Mag();

	      //drop the track if it isn't long enough
	      if(trkl<fMuonTrackLengthCut){continue;}

	      //Store Event 
	      roi_cand_v.push_back(Pfp.Self());      

	      // Hold onto the longest track 
	      // Search through a vector and determine if it has been counted
	      // If it the longest track then push it into the muon max length vector
	      //	      if(nuMuonMaxTrackLength.find(idx) == nuMuonMaxTrackLength.end()) {
	      std::cout<< "Is it me?" << std::endl; 
	      if(nuMuonMaxTrackLength.find(Pfp.Self()) == nuMuonMaxTrackLength.end() ){
		nuMuonMaxTrackLength[Pfp.Self()] = trkl;
		nuMuonMaxTrackLengthIndex[Pfp.Self()] = PfpVector.at(idx).Self();
	      } else if (trkl > nuMuonMaxTrackLength[Pfp.Self()]) {
		nuMuonMaxTrackLength[Pfp.Self()] = trkl;
		nuMuonMaxTrackLengthIndex[Pfp.Self()] = PfpVector.at(idx).Self();		
	      }//End search through vector 
	    }//End Iteration through tracks
	    std::cout<< "Nope!" << std::endl; 
	    // Loop over tracks clusters to build potential ROIs
	    auto const & cls_ds = PfpCls.at(PfpVector.at(idx).Self());
	    std::cout<< "Got the clusters, sized " << cls_ds.size()  << std::endl; 
	    int cluster_num = 0;

	    if(startw.find(Pfp.Self()) == startw.end()) {
	      startw[Pfp.Self()] = std::vector<float>{8256.,8256.,8256.};
	      startt[Pfp.Self()] = std::vector<float>{9600.,9600.,9600.};
	      endw[Pfp.Self()] = std::vector<float>{0.,0.,0.};
	      endt[Pfp.Self()] = std::vector<float>{0.,0.,0.};
	      std::cout << "Setup an ROI for the PFParticle" << std::endl; 
	    }
	      
	    for(auto const & cls_d : cls_ds) {
	      cluster_num++;
	      std::cout<< "Looking at cluster " << cluster_num << std::endl; 
	      auto c_idx = cls_d->Plane().Plane;
	      std::cout<< "Looking at Plane " << c_idx << std::endl; 
	      
	      startw[Pfp.Self()][c_idx] = std::min(startw[Pfp.Self()][c_idx],std::min(cls_d->StartWire(),cls_d->EndWire()));
	      endw[Pfp.Self()][c_idx] = std::max(endw[Pfp.Self()][c_idx],std::max(cls_d->StartWire(),cls_d->EndWire()));
	      startt[Pfp.Self()][c_idx] = std::min(startt[Pfp.Self()][c_idx],std::min(cls_d->StartTick(),cls_d->EndTick()));
	      endt[Pfp.Self()][c_idx] = std::max(endt[Pfp.Self()][c_idx],std::max(cls_d->StartTick(),cls_d->EndTick()));
	      std::cout << "Setup an ROI for the Plane "<< c_idx << std::endl; 
	    }//Done building the ROI around the track
	    std::cout<< "Noper!" << std::endl; 
	  }//End cut on the length of the track
	}//End check on muon
	
	// I would add the ROI building around the showers here
	// Just ask for ALL the shower daughters of the PFParticle and build out the ROI 
	// Want to find a shower
	if(PfpVector.at(idx).PdgCode() == 11) { // Shower-like object
	  
	  if(startw.find(Pfp.Self()) == startw.end()) {
	    startw[Pfp.Self()] = std::vector<float>{8256.,8256.,8256.};
	    startt[Pfp.Self()] = std::vector<float>{9600.,9600.,9600.};
	    endw[Pfp.Self()] = std::vector<float>{0.,0.,0.};
	    endt[Pfp.Self()] = std::vector<float>{0.,0.,0.};
	  }
	  
	  std::cout<< "Nopest 1!" << std::endl; 
	  // Loop over tracks clusters to build potential ROIs
	  auto const & cls_ds = PfpCls.at(PfpVector.at(idx).Self());
	  std::cout<< "Nopest 2!" << std::endl; 
	  for(auto const & cls_d : cls_ds) {
	    auto c_idx = cls_d->Plane().Plane;
	    
	    startw[Pfp.Self()][c_idx] = std::min(startw[Pfp.Self()][c_idx],std::min(cls_d->StartWire(),cls_d->EndWire()));
	    endw[Pfp.Self()][c_idx] = std::max(endw[Pfp.Self()][c_idx],std::max(cls_d->StartWire(),cls_d->EndWire()));
	    startt[Pfp.Self()][c_idx] = std::min(startt[Pfp.Self()][c_idx],std::min(cls_d->StartTick(),cls_d->EndTick()));
	    endt[Pfp.Self()][c_idx] = std::max(endt[Pfp.Self()][c_idx],std::max(cls_d->StartTick(),cls_d->EndTick()));
	  }//Done building the ROI around the Showers
	  std::cout<< "Nopest!" << std::endl; 
	}//Done checking showers
	// 

      }//Done iterating through the PFParticle Daughters
    }//Done checking that the PFParticle is neutrino
  }//Done Iterating through all the PFParticles

  std::cout << "Done Building ROIs! Now to get them ready for prime time" << std::endl; 

  // Loop over longest mouns and save cluster start point
  std::cout << "Gonna sift through my " << nuMuonMaxTrackLengthIndex.size() << " tracks from " 
	    << roi_cand_v.size()<< " ROIs " << std::endl;  

  int check = 0;
  for(auto const & i : nuMuonMaxTrackLengthIndex) {
    check++;
    nuMuonStartWire[i.first] = std::vector<float>{0.,0.,0.};
    nuMuonStartTick[i.first] = std::vector<float>{0.,0.,0.};
    auto const & cls_ds = PfpCls.at(PfpVector.at(i.second).Self());
    std::cout << "I Got myself " << cls_ds.size() << " clusters! for my " <<  check << " out of " << nuMuonMaxTrackLengthIndex.size() << std::endl;  
    for(auto const & cls_d : cls_ds) {
      auto c_idx = cls_d->Plane().Plane;
      nuMuonStartWire[i.first][c_idx] = cls_d->StartWire();
      nuMuonStartTick[i.first][c_idx] = cls_d->StartTick();
    }
  }    

  // Loop over ROIs
  for(auto const & cand : roi_cand_v) {

    // If no ROI was built, does not pass even if there is a vertex candidate
    if(startw.find(cand)==startw.end() || startt.find(cand)==startt.end() || endw.find(cand)==endw.end() || endt.find(cand)==endt.end()){
      continue;}
    else{pass = true;}

    for(int i = 0; i<3; ++i) {
      Vertex[i] = std::make_pair(nuMuonStartTick[cand][i],nuMuonStartWire[cand][i]);
      // Need to define the upper limits on tick and wire number correctly
      TimePairs[i] = std::make_pair(std::max(0.,double(-1*fPadding)+startt[cand][i]),
				    std::min(9600.0,double(fPadding)+endt[cand][i]));
      WirePairs[i] = std::make_pair(std::max(0.,double(-1*fPadding)+startw[cand][i]),
				    std::min(8256.0,double(fPadding)+endw[cand][i]));
    }

    ana::PiZeroROI pizeroroi;
    pizeroroi.SetVertex( Vertex );
    pizeroroi.SetROI( WirePairs, TimePairs );
    pizeroroiVector->emplace_back(pizeroroi);
  
  } // loop over candidates

  std::cout << " Ending this shit " << std::endl; 
  e.put( std::move(pizeroroiVector) );

  return pass;
  
}

void PiZeroFilter::reconfigure(fhicl::ParameterSet const & p)
{
  fPFPModuleLabel = p.get<std::string>("PFPModuleLabel");
  fVertexModuleLabel = p.get<std::string>("VertexModuleLabel");
  fClusterModuleLabel = p.get<std::string>("ClusterModuleLabel");
  fTrackModuleLabel = p.get<std::string>("TrackModuleLabel");
  //ShowerModuleLabel:          "showerrecopandora"

  fPFPVertexAssnModuleLabel = p.get<std::string>("PFPVertexAssnModuleLabel");
  fPFPClusterAssnModuleLabel = p.get<std::string>("PFPClusterAssnModuleLabel");
  fPFPTrackAssnModuleLabel = p.get<std::string>("PFPTrackAssnModuleLabel");
  // Implementation of optional member function here.

  fMuonTrackLengthCut = p.get<float>("MuonTrackLengthCut");
  fTrackVertexProximityCut = p.get<float>("TrackVertexProximityCut");
  fShowerVertex2dProximityCut = p.get<float>("ShowerVertex2dProximityCut");
  fShowerDetached2dProximityCut = p.get<float>("ShowerDetached2dProximityCut");
  fMinMinDetachedShowersPerPlaneCut = p.get<int>("MinMinDetachedShowersPerPlaneCut");
  fMinMaxDetachedShowersPerPlaneCut = p.get<int>("MinMaxDetachedShowersPerPlaneCut");
  fPadding = p.get<float>("Padding");
}

DEFINE_ART_MODULE(PiZeroFilter)

















  



