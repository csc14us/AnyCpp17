#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <utility>

namespace Utility::Impl {

/**
 * Type that can store and retrieve a copy of the object it is constructed with.
 *
 * This version of the "any" concept can store objects of
 * max size given by SmallAny::MAX_OBJECT_SIZE.
 */
class SmallAny final {
  public:
    /** Max object size in bytes that can `SmallAny` can store. */
    static constexpr auto MAX_OBJECT_SIZE = 2 * sizeof(nullptr);

    template<typename Object>
    static bool IsOfType(const SmallAny& any);

    SmallAny();
    ~SmallAny();

    template<typename Object>
    explicit SmallAny(Object object);

    bool empty() const;

    template<typename Object>
    Object* as();

    template<typename Object>
    const Object* as() const;

    // SmallAny is both copyable and moveable
    SmallAny(const SmallAny&);
    SmallAny& operator=(const SmallAny&);
    SmallAny(SmallAny&&) noexcept;
    SmallAny& operator=(SmallAny&&) noexcept;

  private:
    using ObjectData = std::array<std::byte, MAX_OBJECT_SIZE>;

    // Function that operates on the owned object stored in the array.
    using ObjectFunc = void (*)(SmallAny& any, const void* copyFrom);

    template<typename Object>
    static constexpr ObjectFunc MakeObjectFunc();

  private:
    ObjectData objectData_;
    ObjectFunc objectFunc_;
};

#include "SmallAny.inl"

} // Utility::Impl