// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#ifndef SIMBA_VIEWS_COMMON_HPP_
#define SIMBA_VIEWS_COMMON_HPP_

namespace simba {

template <int ParentWidth, int ParentHeight, int RowCount, int ColumnCount>
struct GridLayout {
    struct CellWidth {
        inline static constexpr int value{ParentWidth / ColumnCount};
    };

    struct CellHeight {
        inline static constexpr int value{ParentHeight / RowCount};
    };

    template <int Cell>
    struct XPosition {
        static_assert(Cell < ColumnCount,
                      "Requested Cell coordinate exceeds ColumnCount");
        inline static constexpr int value{Cell * CellWidth::value};
    };

    template <int Cell>
    struct YPosition {
        static_assert(Cell < RowCount,
                      "Requested Cell coordinate exceeds RowCount");
        inline static constexpr int value{Cell * CellHeight::value};
    };
};
}  // namespace simba

#endif  // SIMBA_VIEWS_COMMON_HPP_
