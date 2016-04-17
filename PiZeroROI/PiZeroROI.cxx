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

  
  _pi0_wire_range = PiZeroWire;
  _pi0_t_range = PiZeroTime;
}


void ana::PiZeroROI::SetVertex( const std::vector< std::pair < int, int > > vtx){
  
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

std::vector < std::pair <int, int > > ana::PiZeroROI::GetVertex() const{

  std::vector < std::pair <int, int > > output = _vtx;

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
