#pragma once

template<typename Object>
bool SmallAny::IsOfType(const SmallAny& any) {
  return (MakeObjectFunc<Object>() == any.objectFunc_);
}

template<typename Object>
SmallAny::SmallAny(Object object)
  : objectData_()
{
  static_assert(sizeof(object) <= MAX_OBJECT_SIZE);

  objectFunc_ = MakeObjectFunc<Object>();
  objectFunc_(*this, &object);
}

template<typename Object>
Object* SmallAny::as() {
  if (empty() || !IsOfType<Object>(*this)) {
    return nullptr;
  }

  return reinterpret_cast<Object*>(objectData_.data());
}

template<typename Object>
const Object* SmallAny::as() const {
  if (empty() || !IsOfType<Object>(*this)) {
    return nullptr;
  }

  return reinterpret_cast<const Object*>(objectData_.data());
}

template<typename Object>
SmallAny::ObjectFunc SmallAny::MakeObjectFunc() {
  return [](SmallAny& any, const void* copyFrom) {
    ObjectData& objectData = any.objectData_;

    // Copy the bytes representing the object into the byte array
    if (copyFrom != nullptr) {
      auto pObject = reinterpret_cast<const Object*>(copyFrom);
      new(&objectData) Object{*pObject};
      return;
    }

    // Call the destructor of the object stored in the byte array
    if (!any.empty()) {
      auto pObject = reinterpret_cast<Object*>(objectData.data());
      assert(pObject != nullptr);
      pObject->~Object();
      any.objectFunc_ = nullptr;
      return;
    }
  };
}