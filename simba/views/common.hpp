// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#ifndef SIMBA_VIEWS_COMMON_HPP_
#define SIMBA_VIEWS_COMMON_HPP_

#include <type_traits>

namespace simba::layout {

template <int Value>
struct RowCount {
    static inline constexpr int value{Value};
};

template <int Value>
struct ColumnCount {
    static inline constexpr int value{Value};
};

template <class Type, template <auto...> class Generic>
struct is_instance_of : std::false_type {};

template <template <auto...> class Generic, auto... NTTPs>
struct is_instance_of<Generic<NTTPs...>, Generic> : std::true_type {};

template <class Type, template <auto...> class Generic>
static inline constexpr bool is_instance_of_v =
    is_instance_of<Type, Generic>::value;

template <class T>
concept IsRowCountType = is_instance_of_v<T, RowCount>;

template <class T>
concept IsColumnCountType = is_instance_of_v<T, ColumnCount>;

template <class RowCount_t, class ColumnCount_t, int Width, int Height>
    requires IsRowCountType<RowCount_t> && IsColumnCountType<ColumnCount_t>
struct GridLayout {
    inline static constexpr int cell_width{Width / ColumnCount_t::value};
    inline static constexpr int cell_height{Height / RowCount_t::value};
};

template <int Row, int Column, class GridLayout_t>
struct GridPosition {
    inline static constexpr int row{Row};
    inline static constexpr int column{Column};
};
}  // namespace simba::layout

#endif  // SIMBA_VIEWS_COMMON_HPP_
