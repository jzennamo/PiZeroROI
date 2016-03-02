#include "art/Persistency/Common/Wrapper.h"
#include "PiZeroROI/PiZeroROI.hh"

template class art::Wrapper<std::vector<ana::PiZeroROI> >;
template class std::vector<ana::PiZeroROI>;

template class std::vector<std::pair<int,int> >;
template class std::pair<int,int>;


