/**
 * \file PiZeroROI.h
 *
 * 
 * \brief This data product is to encapuslate the ROI surrounding a PiZero
 *
 * @author Joseph Zennamo, jaz8600@fnal.gov 
*/

#ifndef PIZEROROI_H
#define PIZEROROI_H

#include <string>


#include "TTree.h"
#include <vector>
#include <utility>

namespace ana{
  
  class PiZeroROI
  {
    
    
  public:
    
    /// Default constructor
    PiZeroROI();
    /// Default destructor
    virtual ~PiZeroROI(){};
    
    PiZeroROI(const std::vector < std::pair< int, int > > Wire,
	      const std::vector < std::pair< int, int > > Time);
    
    // Here are the Setters    
    void SetROI(const std::vector < std::pair< int, int > > Wire,
		const std::vector < std::pair< int, int > > Time);
    void SetVertex( const std::vector< std::pair <int, int> > vtx);

    // Here are the Getters
    std::vector < std::pair <int, int > > GetVertex();
    std::vector < std::pair< int, int > > GetWireROI();
    std::vector < std::pair< int, int > > GetTimeROI();
    

  private:
    
    std::string fAlgName;    
    std::vector < std::pair< int, int > > _wire_range; // size 3 planes, min then max
    std::vector < std::pair< int, int > > _t_range; // size 3 planes, min then max
    std::vector < std::pair <int, int > > _vtx; // size 3 plane, pair with tick, wire for vertex
    
  };
}
  
#endif
  
