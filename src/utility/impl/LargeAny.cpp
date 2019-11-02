#include "utility/impl/LargeAny.h"

#include <utility>

namespace Utility::Impl {

LargeAny::LargeAny()
  : ownedObject_(nullptr)
  , sizeOfObject_(0)
  , objectFunc_(nullptr)
{ }

LargeAny::~LargeAny() {
  if (objectFunc_ != nullptr) {
    objectFunc_(ownedObject_, Operation::kDelete);
  }
}

LargeAny::LargeAny(const LargeAny& other)
  : ownedObject_(nullptr)
  , objectFunc_(other.objectFunc_)
  , sizeOfObject_(other.sizeOfObject_)
{
  if (objectFunc_ != nullptr) {
    ownedObject_ = objectFunc_(other.ownedObject_, Operation::kCopy);
  }
}

LargeAny& LargeAny::operator=(const LargeAny& other) {
  if (&other != this) {
    LargeAny copy{other};
    *this = std::move(copy);
  }

  return (*this);
}

LargeAny::LargeAny(LargeAny&& other) noexcept
    : ownedObject_(nullptr)
      , objectFunc_(nullptr)
      , sizeOfObject_(0) {
  *this = std::move(other);
}

LargeAny& LargeAny::operator=(LargeAny&& other) noexcept {
  if (&other != this) {
    ownedObject_ = std::exchange(other.ownedObject_, nullptr);
    sizeOfObject_ = std::exchange(other.sizeOfObject_, 0);
    objectFunc_ = std::exchange(other.objectFunc_, nullptr);
  }

  return (*this);
}

bool LargeAny::empty() const {
  return (ownedObject_ == nullptr);
}

} // Utility::Impl
