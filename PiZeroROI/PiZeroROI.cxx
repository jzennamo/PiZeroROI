#ifndef TEST_USERANALYSIS_CXX
#define TEST_USERANALYSIS_CXX

#include "PiZeroROI.hh"
#include <iostream>

ana::PiZeroROI::PiZeroROI()
  : fAlgName("PiZeroROI")
{}

//Initializer
ana::PiZeroROI::PiZeroROI(const std::vector < std::pair< int, int > > Wire,
			  const std::vector < std::pair< int, int > > Time){
  
  if( Wire.size() != Time.size()){
    try{
      throw Wire.size();
    }
    catch(int e){
      std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
    return;
  }

  if( Wire.size() > 3 || Time.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
  }
  
  _wire_range = Wire;
  _t_range = Time;

}

//Overloaded Initializer
ana::PiZeroROI::PiZeroROI(const std::vector < std::pair< int, int > > Wire,
			  const std::vector < std::pair< int, int > > Time, 
			  const std::vector < std::pair< int, int > > PiZeroWire,
			  const std::vector < std::pair< int, int > > PiZeroTime){
  
  if( Wire.size() != Time.size()){
    try{
      throw Wire.size();
    }
    catch(int e){
      std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
    return;
  }

  if( PiZeroWire.size() != PiZeroTime.size()){
    try{
      throw PiZeroWire.size();
    }
    catch(int e){
      std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
    return;
  }


  if( Wire.size() > 3 || Time.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
  }

  if( PiZeroWire.size() > 3 || PiZeroTime.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
  }

  
  _wire_range = Wire;
  _t_range = Time;
  _pi0_wire_range = PiZeroWire;
  _pi0_t_range = PiZeroTime;

}

// Setter Function Definitions 
void ana::PiZeroROI::SetROI(const std::vector < std::pair< int, int > > Wire,
			    const std::vector < std::pair< int, int > > Time){
  
  if( Wire.size() != Time.size()){
    try{
      throw Wire.size();
    }
    catch(int e){
      std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
    return;
  }

  if( Wire.size() > 3 || Time.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
  }
  
  _wire_range = Wire;
  _t_range = Time;

}

//Overloaded Setter
void ana::PiZeroROI::SetROI(const std::vector < std::pair< int, int > > Wire,
			    const std::vector < std::pair< int, int > > Time,
			    const std::vector < std::pair< int, int > > PiZeroWire,
			    const std::vector < std::pair< int, int > > PiZeroTime){
  
  if( Wire.size() != Time.size()){
    try{
      throw Wire.size();
    }
    catch(int e){
      std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
    return;
  }

  if( PiZeroWire.size() != PiZeroTime.size()){
    try{
      throw PiZeroWire.size();
    }
    catch(int e){
      std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
    return;
  }


  if( Wire.size() > 3 || Time.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
  }

  if( PiZeroWire.size() > 3 || PiZeroTime.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
  }
 
  _wire_range = Wire;
  _t_range = Time;
  _pi0_wire_range = PiZeroWire;
  _pi0_t_range = PiZeroTime;
}

void ana::PiZeroROI::SetPiZeroROI(const std::vector < std::pair< int, int > > PiZeroWire,
				  const std::vector < std::pair< int, int > > PiZeroTime){
  
  if( PiZeroWire.size() != PiZeroTime.size()){
    try{
      throw PiZeroWire.size();
    }
    catch(int e){
      std::cout << "ROI vector sizes do not match, size "<< e << std::endl;
    }
    return;
  }


  if( PiZeroWire.size() > 3 || PiZeroTime.size() > 3){
    std::cout << "Trying to define too many planes, only 3 in this search" << std::endl; 
    return;    
  }

  std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n \t\t :: OH HELLO THERE OLD FRIEND! \n\n "<< PiZeroWire.at(0).second    << "\n\n\n\n\n" << std::endl; 


  _pi0_wire_range = PiZeroWire;

  _pi0_t_range = PiZeroTime;
}


void ana::PiZeroROI::SetVertex( const std::vector< std::pair < int, int > > vtx)
{

  std::cout << " Setting the vertex! " << std::endl;
  
  if(vtx.size() > 3){
  try{ 
    throw vtx.size();
  }
  catch(int e){
    std::cout << "Too many vertices, " << e << std::endl; 
  }
}

_vtx = vtx;

}

void ana::PiZeroROI::SetMuonVertex( const std::vector< float > vtx)
{

  std::cout << "I am setting the muon vertex!" << std::endl;
  _muon_vtx = vtx;

}

void ana::PiZeroROI::SetNeutrinoVertex( const std::vector<float > vtx)
{
  std::cout << "I am setting the neutrino vertex!" << std::endl;
  _nu_vtx = vtx;

}

void ana::PiZeroROI::SetTrackEnd( const std::vector< std::pair < int, int > > trk_end){
  
  if(trk_end.size() > 3){
  try{ 
    throw trk_end.size();
  }
  catch(int e){
    std::cout << "Too many track-endsess, " << e << std::endl; 
  }
}

_trk_end = trk_end;

}


std::vector < float > ana::PiZeroROI::GetMuonVertex() const{

  //  TVector3 * output = _muon_vtx;
  std::cout << "Do you want the muon vertex?" << std::endl;

  std::vector < float > output = _muon_vtx;

  return output;

}

std::vector < float > ana::PiZeroROI::GetNeutrinoVertex() const{

  //  TVector3 * output = *_nu_vtx;
  std::cout << "Do you want the neutrino vertex?" << std::endl;

  std::vector < float > output = _nu_vtx;

  return output;

}

std::vector < std::pair <int, int > > ana::PiZeroROI::GetVertex() const{

  std::vector < std::pair <int, int > > output = _vtx;

  return output;

}

std::vector < std::pair <int, int > > ana::PiZeroROI::GetTrackEnd() const{

  std::vector < std::pair <int, int > > output = _trk_end;
  
  return output;
  
}

std::vector < std::pair< int, int > > ana::PiZeroROI::GetWireROI() const{

  std::vector < std::pair <int, int > > output = _wire_range;

  return output;
  
}

std::vector < std::pair< int, int > > ana::PiZeroROI::GetTimeROI() const {

  std::vector < std::pair <int, int > > output =  _t_range;

  return output;

}

std::vector < std::pair< int, int > > ana::PiZeroROI::GetPiZeroWireROI() const{

  std::vector < std::pair <int, int > > output = _pi0_wire_range;

  return output;
  
}

std::vector < std::pair< int, int > > ana::PiZeroROI::GetPiZeroTimeROI() const {

  std::vector < std::pair <int, int > > output =  _pi0_t_range;

  return output;

}




#endif
