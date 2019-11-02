#pragma once

#include <cstddef>
#include <utility>

namespace Utility::Impl {

/**
 * Type that can store and retrieve a copy of the object it is constructed with.
 *
 * This version of the `Any` concept uses dynamic-memory allocation
 * to store a copy of the object passed into its constructor.
 */
class LargeAny final {
  public:
    template<typename Object>
    static bool IsOfType(const LargeAny& any);

    LargeAny();
    ~LargeAny();

    template<typename Object>
    explicit LargeAny(Object object);

    // LargeAny is both copyable and moveable
    LargeAny(const LargeAny&);
    LargeAny& operator=(const LargeAny&);
    LargeAny(LargeAny&&) noexcept;
    LargeAny& operator=(LargeAny&&) noexcept;

    bool empty() const;

    template<typename Object>
    Object* as();

    template<typename Object>
    const Object* as() const;

  private:
    // The operation to perform on an object with the `ObjectFunc`.
    enum class Operation {
      kCopy,
      kDelete
    };

    // Function that operates on the owned object stored as a void*.
    using ObjectFunc = void*(*)(const void* objectAsVoid, Operation operation);

    template<typename Object>
    static constexpr ObjectFunc MakeObjectFunc();

  private:
    void* ownedObject_;
    size_t sizeOfObject_;
    ObjectFunc objectFunc_;
};

#include "LargeAny.inl"

} // Utility::Impl