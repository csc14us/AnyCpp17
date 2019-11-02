#pragma once

#include "impl/SmallAny.h"
#include "impl/LargeAny.h"

#include <functional>
#include <variant>

namespace Utility {

/**
 * Type that can store and retrieve a copy of the object it is constructed with.
 *
 * Note that the size of an `Any` object equals is four words.
 * The implementation uses type erasure with a small-object
 * optimization; objects up to two words in size are stored
 * without requiring new memory to be allocated.  This
 * includes the primitive types and std::string but not
 * std::vector.
 */
class Any final {
  public:
    /**
     * Is the `any` of the type of `Object`?
     *
     * If true, `as()` will not return nullptr.
     */
    template<typename Object>
    static bool IsOfType(const Any& any);

    /**
     * Make an `Any` object holding a type of the given object and constuctor argument list.
     *
     * @tparam Object The type of the object to store into this `Any`
     * object, which must be explicitly specified in the call to `Make`,
     * since type deduction is not possible (as for std::make_any).
     * @param args Arguments that will be forwarded to the object's constructor.
     * @return Newly constructed `Any` object that stores the given object.
     */
    template<typename Object, typename... Args>
    static Any Make(Args&&... args);

    /** Construct an `any` object for which `empty()` always returns true. */
    Any() = default;

    /**
     * Construct with an object, which must be copyable or moveable.
     *
     * @param object An object to store as a `Any`.
     * Note, sizeof(object) must be <= MAX_OBJECT_SIZE.
     */
    template<typename Object>
    explicit Any(Object object);

    /** True only if this `any` does not contain an object. */
    bool empty() const;

    /** Checked cast of the Any object to the given type. */
    template<typename Object>
    Object* as();

    /** Checked cast of the Any object to the given type. */
    template<typename Object>
    const Object* as() const;

  private:
    using SmallAny = Impl::SmallAny;
    using LargeAny = Impl::LargeAny;
    static_assert(sizeof(SmallAny) == sizeof(LargeAny));

  private:
    std::variant<SmallAny, LargeAny> anyImpl_;
};

} // Utility

#include "Any.inl"
