#ifndef SIMBA_SRC_INCLUDE_STYLE_HPP_
#define SIMBA_SRC_INCLUDE_STYLE_HPP_

#include <qobject.h>
#include <qstyle.h>

namespace simba::style::sheets {
namespace detail {
inline constexpr const char* modern_button_style = R"(
QPushButton {
    background-color: #2D9CDB;
    color: #FFFFFF;
    border: none;
    border-radius: 10px;
    padding: 8px 16px 8px 16px;
    margin: 16px 16px 16px 16px;
    font: 14px "Segoe UI", "Helvetica Neue", Arial, sans-serif;
    min-width: 80px;
    min-height: 32px;
}
QPushButton:hover {
    background-color: #3AA9FF;
}
QPushButton:pressed {
    background-color: #1A73E8;
    padding: 9px 17px;
}
QPushButton:disabled {
    background-color: #B0B0B0;
    color: #E0E0E0;
}
QPushButton:focus {
    outline: none;
}
)";
}  // namespace detail

inline const QString button{detail::modern_button_style};

}  // namespace simba::style::sheets

#endif  // SIMBA_SRC_INCLUDE_STYLE_HPP_
