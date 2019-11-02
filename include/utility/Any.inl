#pragma once

#include <algorithm>
#include <cassert>
#include <utility>

template<typename Object>
bool Utility::Any::IsOfType(const Any& any) {
  using namespace Impl;
  static_assert(sizeof(any) == 4 * sizeof(nullptr));

  auto smallAny = std::get_if<SmallAny>(&any.anyImpl_);
  if (smallAny != nullptr) {
    return SmallAny::IsOfType<Object>(*smallAny);
  } else {
    auto& largeAny = std::get<LargeAny>(any.anyImpl_);
    return LargeAny::IsOfType<Object>(largeAny);
  }
}

template<typename Object, typename... Args>
Utility::Any Utility::Any::Make(Args&&... args) {
  Object object{std::forward<Args...>(args...)};
  return Any{std::move(object)};
}

template<typename Object>
Utility::Any::Any(Object object) {
  if constexpr (sizeof(object) > SmallAny::MAX_OBJECT_SIZE) {
    anyImpl_ = LargeAny{std::move(object)};
  } else {
    anyImpl_ = SmallAny{std::move(object)};
  }
}

template<typename Object>
Object* Utility::Any::as() {
  const auto& smallAny = std::get_if<SmallAny>(&anyImpl_);
  if (smallAny != nullptr) {
    return smallAny->as<Object>();
  } else {
    return std::get<LargeAny>(anyImpl_).as<Object>();
  }
}

template<typename Object>
const Object* Utility::Any::as() const {
  return const_cast<Any*>(this)->as<Object>();
}