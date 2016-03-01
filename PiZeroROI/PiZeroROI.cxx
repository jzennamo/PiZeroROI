#ifndef TEST_USERANALYSIS_CXX
#define TEST_USERANALYSIS_CXX

#include "PiZeroROI.h"
#include <iostream>

namespace larlite{

  PiZeroROI::PiZeroROI()
    : fAlgName("PiZeroROI")
  {}
  
  void PiZeroROI::cleardata(){
    _t_range.clear();
    _wire_range.clear();
    _vtx.clear();
    
  }
  
  PiZeroROI::PiZeroROI(const std::vector < std::pair< int, int > > Wire,
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
  
  // Setter Function Definitions 
  void PiZeroROI::SetROI(const std::vector < std::pair< int, int > > Wire,
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
  
  void PiZeroROI::SetVertex( const std::vector< std::pair < int, int > > vtx){
    
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
  
  std::vector < std::pair <int, int > > PiZeroROI::GetVertex(){
    
    std::vector < std::pair <int, int > > output = _vtx;
    
    return output;
    
  }
  
  
  std::vector < std::pair< int, int > > PiZeroROI::GetWireROI(){
    
    std::vector < std::pair <int, int > > output = _wire_range;
    
    return output;
    
  }
  
  
  
  std::vector < std::pair< int, int > > PiZeroROI::GetTimeROI(){
    
    std::vector < std::pair <int, int > > output =  _t_range;
    
    return output;

  }
  
  
}

#endif
