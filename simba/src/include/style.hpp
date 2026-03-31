#ifndef SIMBA_SRC_INCLUDE_STYLE_HPP_
#define SIMBA_SRC_INCLUDE_STYLE_HPP_

#include <qobject.h>
#include <qstring.h>
#include <qstyle.h>

namespace simba::style::sheets {
using Qt::Literals::StringLiterals::operator""_s;

namespace buttons {
const inline QString overview_enabled{
    uR"(
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
)"_s};

const inline QString overview_disabled{
    uR"(
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
)"_s};
};  // namespace buttons

namespace labels {
const inline QString grand_total_unloaded{
    uR"(
QLabel {
    color: lightgray;                     
    font: 24px "Segoe UI", "Helvetica Neue", Arial, sans-serif;
    font-weight: 600;                   
    background: transparent;           
    padding: 12px 0;                    
    margin-bottom: 16px;                
}

QLabel[underline="true"] {
    border-bottom: 2px solid #2D9CDB;
}
)"_s};

const inline QString grand_total{
    uR"(
QLabel {
    color: #2D9CDB;                     
    font: 24px "Segoe UI", "Helvetica Neue", Arial, sans-serif;
    font-weight: 600;                   
    background: transparent;           
    padding: 12px 0;                    
    margin-bottom: 16px;                
}

QLabel[underline="true"] {
    border-bottom: 2px solid #2D9CDB;
}
)"_s};
};  // namespace labels

}  // namespace simba::style::sheets

#endif  // SIMBA_SRC_INCLUDE_STYLE_HPP_
