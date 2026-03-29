#ifndef SIMBA_SRC_INCLUDE_META_HPP_
#define SIMBA_SRC_INCLUDE_META_HPP_

#include <cstdint>
#include <type_traits>

namespace simba::meta {

template <bool Predicate, class T, class F>
struct If final {
    static_assert(false, "Could not evaluate predicate at compile-time");
    using type = void;
};

template <class T, class F>
struct If<true, T, F> final {
    using type = T;
};

template <class T, class F>
struct If<false, T, F> final {
    using type = F;
};

using signed_native_word_t = If<sizeof(void*) == 8,              // if
                                std::int64_t,                    // then
                                typename If<sizeof(void*) == 4,  // else-if
                                            std::int32_t,        // then
                                            void                 //
                                            >::type>::type;

static_assert(!std::is_same_v<signed_native_word_t, void>,
              "Unsupported architecture size");

}  // namespace simba::meta

#endif  // SIMBA_SRC_INCLUDE_META_HPP_
