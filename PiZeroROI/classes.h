#include "art/Persistency/Common/PtrVector.h"
#include "art/Persistency/Common/Wrapper.h"
#include "art/Persistency/Common/Assns.h"

#include "lardata/RecoBase/PFParticle.h"
#include "PiZeroROI/PiZeroROI.hh"

template class art::Wrapper<std::vector<ana::PiZeroROI> >;
template class std::vector<ana::PiZeroROI>;

template class art::Assns<recob::PFParticle, ana::PiZeroROI, void>;
template class art::Wrapper<art::Assns<recob::PFParticle, ana::PiZeroROI, void> >;
template class art::Assns<ana::PiZeroROI, recob::PFParticle, void>;
template class art::Wrapper<art::Assns<ana::PiZeroROI, recob::PFParticle, void> >;

template class std::vector<std::pair<int,int> >;
template class std::pair<int,int>;


