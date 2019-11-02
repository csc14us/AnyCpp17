#include "utility/impl/SmallAny.h"

namespace Utility::Impl {

SmallAny::SmallAny()
  : objectData_()
  , objectFunc_(nullptr)
{
  static_assert(sizeof(ObjectFunc) == sizeof(nullptr));
  static_assert((MAX_OBJECT_SIZE + sizeof(ObjectFunc)) == sizeof(SmallAny));
}

SmallAny::~SmallAny() {
  if (objectFunc_ != nullptr) {
    objectFunc_(*this, nullptr);
  }
}

SmallAny::SmallAny(const SmallAny& other)
  : objectData_()
  , objectFunc_(other.objectFunc_)
{
  objectFunc_ = other.objectFunc_;
  if (objectFunc_ != nullptr) {
    objectFunc_(*this, other.objectData_.data());
  }
}

SmallAny& SmallAny::operator=(const SmallAny& other) {
  if (&other != this) {
    *this = SmallAny{other};
  }

  return (*this);
}

SmallAny::SmallAny(SmallAny&& other) noexcept {
  *this = std::move(other);
}

SmallAny& SmallAny::operator=(SmallAny&& other) noexcept {
  if (&other != this) {
    objectFunc_ = other.objectFunc_;
    objectData_ = other.objectData_;
    other.objectFunc_ = nullptr;
  }

  return (*this);
}

bool SmallAny::empty() const {
  return (objectFunc_ == nullptr);
}

} // Utility::Impl
