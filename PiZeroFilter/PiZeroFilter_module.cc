////////////////////////////////////////////////////////////////////////
// Class:       PiZeroFilter
// Module Type: filter
// File:        PiZeroFilter_module.cc
//
// Version by Lorena and John 
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

#include "lardata/Utilities/AssociationUtil.h"

#include <memory>

#include "art/Framework/Services/Optional/TFileService.h"

#include "PiZeroROI/PiZeroROI.hh"
#include "lardata/RecoBase/PFParticle.h"
#include "lardata/RecoBase/Vertex.h"
#include "lardata/RecoBase/Track.h"
#include "lardata/RecoBase/Cluster.h"

#include "lardata/MCBase/MCShower.h"

//lorena - include larpandora
#include "larpandora/LArPandoraInterface/LArPandoraHelper.h"

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
  // ShowerModuleLabel:          "showerrecopandora"
  
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
  
  TTree* fallEventTree;
  int fnVtx;
  int fnShw;
  int fnTrk;
  int fnNuMuCC;
  
  TTree* fselectedEventTree;
  int fnTrkSelected;
  double fTrkLengths;
  double fdeltaVtx;
  int fnAllTrkSelected;
  int fnAllShowSelected;
  
  TTree* fMCEventTree;
  int fnTrkMC;
  int fnShwMC;
  int fnMCNuMuCC;
  int fMCPdg;

  bool NeutrinoHasAtLeastOneTrackAndTwoShowers(const art::Ptr<recob::PFParticle> particle, lar_pandora::PFParticleVector pfParticleList);
  int GetNCloseTracks(const art::Ptr<recob::PFParticle> particle, art::Ptr<recob::Vertex> nuVertex, lar_pandora::PFParticlesToVertices pfParticlesToVerticesMap, lar_pandora::PFParticleVector pfParticleList) const;
  bool IsThereALongTrack(const art::Ptr<recob::PFParticle> particle, lar_pandora::PFParticleVector pfParticleList, lar_pandora::PFParticlesToTracks pfParticleToTrackMap) const;  
  art::Ptr<recob::PFParticle> FindLongestTrack(const art::Ptr<recob::PFParticle> particle, lar_pandora::PFParticleVector pfParticleList, lar_pandora::PFParticlesToTracks pfParticleToTrackMap) const;
  bool BuildROI(const art::Ptr<recob::PFParticle> particle, lar_pandora::PFParticleVector pfParticleList, art::Ptr<recob::PFParticle> longestTrack, lar_pandora::PFParticlesToClusters pfParticleToClusterMap, lar_pandora::PFParticlesToVertices pfParticlesToVerticesMap, std::vector<std::pair<int,int> > & Vertex, std::vector<std::pair<int,int> > & WirePairs, std::vector<std::pair<int,int> > & TimePairs, std::vector<std::pair<int,int> > & PiZeroWirePairs, std::vector<std::pair<int,int> > & PiZeroTimePairs);
    
};


PiZeroFilter::PiZeroFilter(fhicl::ParameterSet const & p)
  : fnVtx(0),
    fnShw(0),
    fnNuMuCC(0),
    fnTrkSelected(0),
    fdeltaVtx(0.0),
    fnAllTrkSelected(0),
    fnAllShowSelected(0)
    
    
    // Initialize member data here.
{
  this->reconfigure(p);
  art::ServiceHandle<art::TFileService> tfs;
  
  fallEventTree = tfs->make<TTree>("allEvents","allEvents");
  fallEventTree->Branch("fnVtx",&fnVtx,"fnVtx/I");
  fallEventTree->Branch("fnShw",&fnShw,"fnShw/I");
  fallEventTree->Branch("fnTrk",&fnTrk,"fnTrk/I");
  fallEventTree->Branch("fnNuMuCC",&fnNuMuCC,"fnNuMuCC/I");
  
  fselectedEventTree = tfs->make<TTree>("selectedEvents","selectedEvents");
  fselectedEventTree->Branch("fdeltaVtx",&fdeltaVtx,"fdeltaVtx/D");
  fselectedEventTree->Branch("fTrkLengths",&fTrkLengths,"fTrkLengths/D");
  fselectedEventTree->Branch("fnTrkSelected",&fnTrkSelected,"fnTrkSelected/I");
  fselectedEventTree->Branch("fnAllTrkSelected",&fnAllTrkSelected,"fnAllTrkSelected/I");
  fselectedEventTree->Branch("fnAllShowSelected",&fnAllShowSelected,"fnAllShowSelected/I");
  
  fMCEventTree = tfs->make<TTree>("MCEvents","MCEvents");
  fMCEventTree->Branch("fnTrkMC",&fnTrkMC,"fnTrkMC/I");
  fMCEventTree->Branch("fnShwMC",&fnShwMC,"fnShwMC/I");
  fMCEventTree->Branch("fnMCNuMuCC",&fnMCNuMuCC,"fnMCNuMuCC/I");
  fMCEventTree->Branch("fMCPdg",&fMCPdg,"fMCPdg/I");
  
  // Call appropriate produces<>() functions here.
  produces<std::vector<ana::PiZeroROI> >();
  produces<art::Assns<recob::PFParticle, ana::PiZeroROI> >();
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool PiZeroFilter::filter(art::Event & e)
{
  bool pass = false;
  
  // Implementation of required member function here.
  std::unique_ptr<std::vector<ana::PiZeroROI> > pizeroroiVector( new std::vector<ana::PiZeroROI> );
  //std::unique_ptr< art::Assns<recob::Vertex, ana::PiZeroROI::PiZeroROI > >  assnPiZeroROITagVertex( new art::Assns<recob::Vertex, anab::PiZeroROI>);
  
  //declaration of variables to be used to store ROIss
  std::vector< int > roi_cand_v;  
  std::vector< double > trk_lengths; 
    
  std::map<int,double> nuMuonMaxTrackLength;
  std::map<int,int> nuMuonMaxTrackLengthIndex;
  std::map<int,std::vector<float> > nuMuonStartWire;
  std::map<int,std::vector<float> > nuMuonStartTick;
  std::map<int,std::vector<int> > nDetachedShowers;
  //lorena - does these need to be maps? dont we want just one ROI per event?
  std::vector<ana::PiZeroROI> pizeroroi_v;
  std::vector<std::pair<int,int> > Vertex(3);
  std::vector<std::pair<int,int> > TimePairs(3);
  std::vector<std::pair<int,int> > WirePairs(3);
  std::vector<std::pair<int,int> > PiZeroTimePairs(3);
  std::vector<std::pair<int,int> > PiZeroWirePairs(3);


  //Attempt at making an association between PFParticle and ROI
  std::unique_ptr<art::Assns<recob::PFParticle, ana::PiZeroROI> > ROI_PFP_Assn(new art::Assns<recob::PFParticle, ana::PiZeroROI>); 
    
  // * PFParticles
  //lorena - these vectors and maps will contain the associations between pfparticles and vertices, tracks, clusters, etc
  //these vectors and maps are filled up inside LArPandoraHelper, should have the right associations
  
  std::cout << "Getting the PFParticles... "<< std::endl;	
  lar_pandora::PFParticleVector pfParticleList; //vector of PFParticles
  lar_pandora::VertexVector vertexVector; //vector of vertices
  lar_pandora::TrackVector allPfParticleTracks; //all PFParticle tracks
  lar_pandora::ShowerVector allPfParticleShowers; //all PFParticle showers
  lar_pandora::PFParticlesToClusters pfParticleToClusterMap; //PFParticle-to-cluster map
  lar_pandora::PFParticlesToVertices pfParticlesToVerticesMap; //PFParticle-to-vertex map
  lar_pandora::PFParticlesToTracks pfParticleToTrackMap; //PFParticle-to-track map
  lar_pandora::PFParticlesToShowers pfParticleToShowerMap; //PFParticle-to-shower map
  
  lar_pandora::LArPandoraHelper::CollectPFParticles(e, fPFPModuleLabel, pfParticleList, pfParticleToClusterMap); //collect PFParticles and map to clusters
  lar_pandora::LArPandoraHelper::CollectVertices(e, fPFPModuleLabel, vertexVector, pfParticlesToVerticesMap); //map PFParticles-to-vertex
  lar_pandora::LArPandoraHelper::CollectTracks(e, fPFPModuleLabel, allPfParticleTracks, pfParticleToTrackMap);//map PFParticles-to-tracks
  lar_pandora::LArPandoraHelper::CollectShowers(e, fPFPModuleLabel, allPfParticleShowers, pfParticleToShowerMap);//map PfParticles-to-showers
  //lorena - question, do we need more than one module label?	  
  
  //lorena - a check... 
  std::cout << "Number of PFParticles = "<< pfParticleList.size() << std::endl;
  
  short nprim = 0;
  for (const art::Ptr<recob::PFParticle> particle : pfParticleList)
    {
      if (particle->IsPrimary())
	nprim++;
    }
  
  //lorena - big loop to find neutrino PFParticle here	  
  for (unsigned int n = 0; n < pfParticleList.size(); ++n)
    {
      const art::Ptr<recob::PFParticle> particle = pfParticleList.at(n);	  
      
      if (particle->IsPrimary() && lar_pandora::LArPandoraHelper::IsNeutrino(particle))
        { 
	  //Select only if Primary PFParticle is a neutrino
	  //get neutrino vertex
	  lar_pandora::PFParticlesToVertices::const_iterator vIter = pfParticlesToVerticesMap.find(particle);
	  if (pfParticlesToVerticesMap.end() != vIter)
            {
	      const lar_pandora::VertexVector &vertexVector = vIter->second;
	      
	      if (!vertexVector.empty())
                {
		  if (vertexVector.size() == 1)
                    {
		      //if there is one vertex, store it
		      art::Ptr<recob::Vertex> nuVertex = vertexVector.front();
		      
		      if (!this->NeutrinoHasAtLeastOneTrackAndTwoShowers(particle,pfParticleList))
			continue;
		      
		      const int n_close_tracks = this->GetNCloseTracks(particle, nuVertex, pfParticlesToVerticesMap,pfParticleList);
                      
		      if (fnShw > 2 || n_close_tracks > 2)
			continue;//lorena - why this cut?  
		      
		      if (!this->IsThereALongTrack(particle,pfParticleList, pfParticleToTrackMap))
			continue;
		      
		      //Lorena: IMPORTANT! - This needs checks - ShowerDetachedProximityCut not used in latest version? 
		      art::Ptr<recob::PFParticle> longestTrack = this->FindLongestTrack(particle,pfParticleList, pfParticleToTrackMap);
		      if (!this->BuildROI(particle, pfParticleList, longestTrack, pfParticleToClusterMap, pfParticlesToVerticesMap,Vertex,WirePairs,TimePairs,PiZeroWirePairs,PiZeroTimePairs))
			continue;
		      
		      //store the ROI found
		      ana::PiZeroROI pizeroroi;
		      pizeroroi.SetVertex( Vertex );
		      pizeroroi.SetROI( WirePairs, TimePairs );
		      pizeroroi.SetPiZeroROI(PiZeroWirePairs,PiZeroTimePairs);
		      pizeroroiVector->emplace_back(pizeroroi);
		      

		      if (!util::CreateAssn(*this, e, *pizeroroiVector, particle, *ROI_PFP_Assn))
			{
			  throw art::Exception(art::errors::InsertFailure)
			    << "Can't associate";
			}

		      pass = true;
                    }//if there is one vertex
                }//
            }//vertex ok        
        }//particle is prinary and neutrino
    }//loop particle list
  
  e.put( std::move(pizeroroiVector) );
  e.put( std::move(ROI_PFP_Assn) );

  return pass;
}//end filter 

//------------------------------------------------------------------------------------------------------------------------------------------

//lorena - function to check neutrino has at least one track and two showers
//this could be more generic CheckNTracksAndShowers and minimum could be parameter in .xml
bool PiZeroFilter::NeutrinoHasAtLeastOneTrackAndTwoShowers(const art::Ptr<recob::PFParticle> particle, lar_pandora::PFParticleVector pfParticleList)
{
  unsigned int n_tracks = 0, n_showers = 0;
  const std::vector<size_t> &daughterIDs = particle->Daughters();
  
  for (size_t j = 0; j < daughterIDs.size(); ++j)
    {
      if (lar_pandora::LArPandoraHelper::IsTrack(pfParticleList.at(daughterIDs[j]))) ++n_tracks;
      else if (lar_pandora::LArPandoraHelper::IsShower(pfParticleList.at(daughterIDs[j]))) ++n_showers;
    }
  
  //lorena - add this here, not sure the best place though
  fnShw = n_showers;
  fnTrk = n_tracks;
  fnNuMuCC = (n_tracks>=1)&&(n_showers>=2);
  fallEventTree->Fill();
  
  return ((n_tracks>=1) && (n_showers>=2)); //lorena - min can be another parameter in .xml
}

//------------------------------------------------------------------------------------------------------------------------------------------

//lorena - get number of close tracks
int PiZeroFilter::GetNCloseTracks(const art::Ptr<recob::PFParticle> particle, art::Ptr<recob::Vertex> nuVertex, lar_pandora::PFParticlesToVertices pfParticlesToVerticesMap, lar_pandora::PFParticleVector pfParticleList) const
{ 
  int close_tracks = 0;
  double xyz_p[3] = {0.0, 0.0, 0.0} ;
  nuVertex->XYZ(xyz_p);//vertex of the neutrino primary particle in XYZ form
  
  const std::vector<size_t> &daughterIDs = particle->Daughters();
  
  for (size_t j = 0; j < daughterIDs.size(); ++j) // loop over neutrino daughters
    {
      const art::Ptr<recob::PFParticle> daughter(pfParticleList.at(daughterIDs[j]));
      if(lar_pandora::LArPandoraHelper::IsTrack(daughter))
        {
	  lar_pandora::PFParticlesToVertices::const_iterator vertexMapIter = pfParticlesToVerticesMap.find(daughter);
	  if (vertexMapIter != pfParticlesToVerticesMap.end())
            {
	      const lar_pandora::VertexVector &daughterVertices = vertexMapIter->second;
	      //vertices
	      if (daughterVertices.empty())
		std::cerr << "Error! daughter particle with ID " << pfParticleList.at(daughterIDs[j]) << " has no vertex!" << std::endl;
	      
	      if (daughterVertices.size() > 1)
		std::cerr << "Warning: more than one vertex found for neutrino daughter with ID " << pfParticleList.at(daughterIDs[j]) << std::endl;
	      
	      if (daughterVertices.size() > 0)
                {
		  art::Ptr<recob::Vertex> daughterVertex = daughterVertices.front();
		  double xyz_d[3] = {0.,0.,0.};
		  daughterVertex->XYZ(xyz_d);
		  
		  const float dist = std::sqrt(std::pow(xyz_p[0]-xyz_d[0],2)+std::pow(xyz_p[1]-xyz_d[1],2)+std::pow(xyz_p[2]-xyz_d[2],2));// std::pow deprecated
		  if(dist<fTrackVertexProximityCut)
		    ++close_tracks;
                }
	    }
        }
    }
  
  return close_tracks;	
}

//------------------------------------------------------------------------------------------------------------------------------------------

//lorena -find whether there is at least one long track 
bool PiZeroFilter::IsThereALongTrack(const art::Ptr<recob::PFParticle> particle, lar_pandora::PFParticleVector pfParticleList, lar_pandora::PFParticlesToTracks pfParticleToTrackMap) const
{
  const std::vector<size_t> &daughterIDs = particle->Daughters();
  
  for (size_t j = 0; j < daughterIDs.size(); ++j) // loop over neutrino daughters                                                                         
    {
      const art::Ptr<recob::PFParticle> daughter(pfParticleList.at(daughterIDs[j]));
      
      if (lar_pandora::LArPandoraHelper::IsTrack(daughter)) // loop over tracks
        {
	  const lar_pandora::TrackVector &pfParticleTracks = pfParticleToTrackMap.at(daughter);
	  
	  if (pfParticleTracks.size() > 1)
	    std::cerr << "Warning: there was more than one track found for daughter particle with ID " << pfParticleList.at(daughterIDs[j]) << std::endl;
	  
	  if (pfParticleTracks.size() > 0)
            {
	      art::Ptr<recob::Track> daughterTrack = pfParticleTracks.front();
	      const float trkl = (daughterTrack->Vertex()-daughterTrack->End()).Mag();
	      
	      if(trkl>fMuonTrackLengthCut)
		return true;
            }
        }
    }
  return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------

//lorena - find the longest track
art::Ptr<recob::PFParticle> PiZeroFilter::FindLongestTrack(const art::Ptr<recob::PFParticle> particle, lar_pandora::PFParticleVector pfParticleList, lar_pandora::PFParticlesToTracks pfParticleToTrackMap) const
{
  float max_trkl = -std::numeric_limits<float>::max(); // double-check and, if good, re-use in other places!
  size_t longest_trk_idx = -std::numeric_limits<size_t>::max();

  const std::vector<size_t> &daughterIDs = particle->Daughters();
  for (size_t j = 0; j < daughterIDs.size(); ++j) // loop over neutrino daughters                                                                         
    {
      const art::Ptr<recob::PFParticle> daughter(pfParticleList.at(daughterIDs[j]));
      
      if (lar_pandora::LArPandoraHelper::IsTrack(daughter)) // loop over tracks
        {
	  const lar_pandora::TrackVector &pfParticleTracks = pfParticleToTrackMap.at(daughter);
	  
	  if (pfParticleTracks.size() > 1)
	    std::cerr << "Warning: there was more than one track found for daughter particle with ID " << pfParticleList.at(daughterIDs[j]) << std::endl;
	  
	  if (pfParticleTracks.size() > 0)
            {
	      art::Ptr<recob::Track> daughterTrack = pfParticleTracks.front();
	      const float trkl = (daughterTrack->Vertex()-daughterTrack->End()).Mag();
	      
	      if((trkl>fMuonTrackLengthCut)&& (trkl>max_trkl)){
		//if longest so far                                                                                                                       
		max_trkl = trkl;
		longest_trk_idx = pfParticleList.at(daughterIDs[j])->Self(); // lorena: Check this!
	      }
	      
            }
        }
    }
  return pfParticleList.at(longest_trk_idx);
}


//------------------------------------------------------------------------------------------------------------------------------------------

//lorena - build the ROI
bool PiZeroFilter::BuildROI(const art::Ptr<recob::PFParticle> particle, lar_pandora::PFParticleVector pfParticleList,art::Ptr<recob::PFParticle> longestTrack, lar_pandora::PFParticlesToClusters pfParticleToClusterMap, lar_pandora::PFParticlesToVertices pfParticlesToVerticesMap, std::vector<std::pair<int,int> > & Vertex, std::vector<std::pair<int,int> > & WirePairs, std::vector<std::pair<int,int> > & TimePairs, std::vector<std::pair<int,int> > & PiZeroWirePairs, std::vector<std::pair<int,int> > & PiZeroTimePairs) 
{
  //lorena - this is only used here
  std::vector<float>  startw  = std::vector<float>{8256.,8256.,8256.};
  std::vector<float>  startt = std::vector<float>{9600.,9600.,9600.};
  std::vector<float>  endw = std::vector<float>{0.,0.,0.};
  std::vector<float>  endt = std::vector<float>{0.,0.,0.};

  std::vector<float>  pi0startw  = std::vector<float>{8256.,8256.,8256.};
  std::vector<float>  pi0startt = std::vector<float>{9600.,9600.,9600.};
  std::vector<float>  pi0endw = std::vector<float>{0.,0.,0.};
  std::vector<float>  pi0endt = std::vector<float>{0.,0.,0.};


  
  // 1 - fill with longest track info
  double xyz_track[3] = {0.,0.,0.};
  lar_pandora::PFParticlesToVertices::const_iterator trackVertexMapIter = pfParticlesToVerticesMap.find(longestTrack);
  if (trackVertexMapIter == pfParticlesToVerticesMap.end())
    std::cerr << "Warning: longest track has no vertex!" << std::endl;
  lar_pandora::VertexVector trackVertices = trackVertexMapIter->second;
  if (trackVertices.size() > 1)
    std::cerr << "Warning: more than one vertex found for the longest track!" << std::endl;

  art::Ptr<recob::Vertex> trackVertex = trackVertices.front();
  trackVertex->XYZ(xyz_track);
  
  lar_pandora::PFParticlesToClusters::const_iterator clusterMapIter = pfParticleToClusterMap.find(longestTrack); 
  if (clusterMapIter != pfParticleToClusterMap.end()) {
    lar_pandora::ClusterVector trackClusters = clusterMapIter->second;
    for(unsigned int i = 0; i < trackClusters.size(); ++i)
      {
	auto c_idx = trackClusters[i]->Plane().Plane;
	Vertex[c_idx] = std::make_pair(trackClusters[i]->StartTick(),trackClusters[i]->StartWire()); //                             
	startw[c_idx] = std::min(startw[c_idx],std::min(trackClusters[i]->StartWire(),trackClusters[i]->EndWire()));
	endw[c_idx] = std::max(endw[c_idx],std::max(trackClusters[i]->StartWire(),trackClusters[i]->EndWire()));
	startt[c_idx] = std::min(startt[c_idx],std::min(trackClusters[i]->StartTick(),trackClusters[i]->EndTick()));
	endt[c_idx] = std::max(endt[c_idx],std::max(trackClusters[i]->StartTick(),trackClusters[i]->EndTick()));
      }
  }
  
  // 2 - find detached but close showers
  int n_detached_showers = 0;  
  const std::vector<size_t> &daughterIDs = particle->Daughters();
  for (size_t j = 0; j < daughterIDs.size(); ++j) // loop over neutrino daughters                                                                
    {
      const art::Ptr<recob::PFParticle> daughter(pfParticleList.at(daughterIDs[j]));
      if(lar_pandora::LArPandoraHelper::IsShower(daughter)) // loop over showers
        {
          lar_pandora::PFParticlesToVertices::const_iterator vertexMapIter = pfParticlesToVerticesMap.find(daughter); //get vertex                             
          if (vertexMapIter == pfParticlesToVerticesMap.end()) continue;
          lar_pandora::VertexVector showerVertices = vertexMapIter->second;
          if (showerVertices.size() > 1) 
	    continue;
          art::Ptr<recob::Vertex> showerVertex = showerVertices.front();
          double xyz_shower[3] = {0.,0.,0.};
          showerVertex->XYZ(xyz_shower);
          //IMPORTANT! detached clusters proximity cut, using vertices?
          float dist = std::sqrt(std::pow(xyz_track[0]-xyz_shower[0],2)+std::pow(xyz_track[1]-xyz_shower[1],2)+std::pow(xyz_track[2]-xyz_shower[2],2));//distance shower-track
          if(dist<fShowerDetached2dProximityCut){
            ++n_detached_showers;
	    
            lar_pandora::PFParticlesToClusters::const_iterator clusterMapIter = pfParticleToClusterMap.find(daughter);//find clusters                        
            if (clusterMapIter != pfParticleToClusterMap.end()) {
              lar_pandora::ClusterVector showerClusters = clusterMapIter->second;
              for(int i = 0; i<3; ++i) {
		auto c_idx = showerClusters[i]->Plane().Plane;
		startw[c_idx] = std::min(startw[c_idx],std::min(showerClusters[i]->StartWire(),showerClusters[i]->EndWire()));
		endw[c_idx] = std::max(endw[c_idx],std::max(showerClusters[i]->StartWire(),showerClusters[i]->EndWire()));
		startt[c_idx] = std::min(startt[c_idx],std::min(showerClusters[i]->StartTick(),showerClusters[i]->EndTick()));
		endt[c_idx] = std::max(endt[c_idx],std::max(showerClusters[i]->StartTick(),showerClusters[i]->EndTick()));
		pi0startw[c_idx] = std::min(pi0startw[c_idx],std::min(showerClusters[i]->StartWire(),showerClusters[i]->EndWire()));
		pi0endw[c_idx] = std::max(pi0endw[c_idx],std::max(showerClusters[i]->StartWire(),showerClusters[i]->EndWire()));
		pi0startt[c_idx] = std::min(pi0startt[c_idx],std::min(showerClusters[i]->StartTick(),showerClusters[i]->EndTick()));
		pi0endt[c_idx] = std::max(pi0endt[c_idx],std::max(showerClusters[i]->StartTick(),showerClusters[i]->EndTick()));
              }
            }
          }
        }
    }
  
  //IMPORTANT! number detached showers cut, disappeared?  
  if (n_detached_showers<fMinMinDetachedShowersPerPlaneCut)
    return false;
  
  for(int i = 0; i<3; ++i) {
    TimePairs[i] = std::make_pair(std::max(0.,double(-1*fPadding)+startt[i]),
				  std::min(9600.0,double(fPadding)+endt[i]));
    WirePairs[i] = std::make_pair(std::max(0.,double(-1*fPadding)+startw[i]),
				  std::min(8256.0,double(fPadding)+endw[i]));
    PiZeroTimePairs[i] = std::make_pair(std::max(0.,double(-1*fPadding)+pi0startt[i]),
					std::min(9600.0,double(fPadding)+pi0endt[i]));
    PiZeroWirePairs[i] = std::make_pair(std::max(0.,double(-1*fPadding)+pi0startw[i]),
					std::min(8256.0,double(fPadding)+pi0endw[i]));


  }
  
  return true;
}
  
//------------------------------------------------------------------------------------------------------------------------------------------

// reconfigure 
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





	  



