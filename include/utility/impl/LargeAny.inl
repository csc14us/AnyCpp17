#pragma once

template<typename Object>
bool LargeAny::IsOfType(const LargeAny& any) {
  return (MakeObjectFunc<Object>() == any.objectFunc_);
}

template<typename Object>
LargeAny::LargeAny(Object object) {
  objectFunc_ = MakeObjectFunc<Object>();
  ownedObject_ = objectFunc_(&object, Operation::kCopy);
  sizeOfObject_ = sizeof(Object);
}

template<typename Object>
Object* LargeAny::as() {
  if (empty() || !IsOfType<Object>(*this)) {
    return nullptr;
  }

  return reinterpret_cast<Object*>(ownedObject_);
}

template<typename Object>
const Object* LargeAny::as() const {
  if (empty() || !IsOfType<Object>(*this)) {
    return nullptr;
  }

  return reinterpret_cast<const Object*>(ownedObject_);
}

template<typename Object>
constexpr LargeAny::ObjectFunc LargeAny::MakeObjectFunc() {
  return [](const void* objectAsVoid, const Operation operation) -> void* {
    if (operation == Operation::kCopy) {
      auto asObject = reinterpret_cast<const Object*>(objectAsVoid);
      return (asObject != nullptr ? new Object{*asObject} : nullptr);
    }

    if (operation == Operation::kDelete) {
      auto asObject = reinterpret_cast<const Object*>(objectAsVoid);
      delete asObject; // it's always safe to delete nullptr
    }

    return nullptr;
  };
}