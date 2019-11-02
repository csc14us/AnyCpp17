#include "utility/Any.h"

namespace Utility {

bool Any::empty() const {
  auto smallAny = std::get_if<SmallAny>(&anyImpl_);
  if (smallAny != nullptr) {
    return smallAny->empty();
  } else {
    return std::get<LargeAny>(anyImpl_).empty();
  }
}

} // Utility
