/**
 * \file PiZeroROI.h
 *
 * 
 * \brief This data product is to encapuslate the ROI surrounding a PiZero
 *
 * @author Joseph Zennamo, jaz8600@fnal.gov 
*/

#ifndef TEST_USERANALYSIS_H
#define TEST_USERANALYSIS_H

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
    //virtual ~PiZeroROI(){};    
    PiZeroROI(const std::vector < std::pair< int, int > > Wire,
	      const std::vector < std::pair< int, int > > Time);

    PiZeroROI(const std::vector < std::pair< int, int > > Wire,
	      const std::vector < std::pair< int, int > > Time,
	      const std::vector < std::pair< int, int > > PiZeroWire,
	      const std::vector < std::pair< int, int > > PiZeroTime);
    
    
    // Here are the Setters    
    void SetROI(const std::vector < std::pair< int, int > > Wire,
		const std::vector < std::pair< int, int > > Time);
    
    void SetROI(const std::vector < std::pair< int, int > > Wire,
		const std::vector < std::pair< int, int > > Time,
		const std::vector < std::pair< int, int > > PiZeroWire,
		const std::vector < std::pair< int, int > > PiZeroTime);
    
    void SetPiZeroROI(const std::vector < std::pair< int, int > > PiZeroWire,
		      const std::vector < std::pair< int, int > > PiZeroTime);
    
    void SetVertex( const std::vector< std::pair <int, int> > vtx);

    // Here are the Getters
    std::vector < std::pair <int, int > > GetVertex() const;
    std::vector < std::pair< int, int > > GetWireROI() const;
    std::vector < std::pair< int, int > > GetTimeROI() const;
    std::vector < std::pair< int, int > > GetPiZeroWireROI() const;
    std::vector < std::pair< int, int > > GetPiZeroTimeROI() const;
    

  private:
    
    std::string fAlgName;    
    std::vector < std::pair< int, int > > _wire_range; // size 3 planes, min then max
    std::vector < std::pair< int, int > > _t_range; // size 3 planes, min then max
    std::vector < std::pair< int, int > > _pi0_wire_range; // size 3 planes, min then max
    std::vector < std::pair< int, int > > _pi0_t_range; // size 3 planes, min then max
    std::vector < std::pair <int, int > > _vtx; // size 3 plane, pair with tick, wire for vertex
    
  };
}
  
#endif
  
