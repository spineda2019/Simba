#include "moc_include/LicenseInfo.hpp"

#include <qdialog.h>
#include <qwidget.h>
#include <ui_license.h>

LicenseInfo::LicenseInfo(QWidget* parent)
    : QDialog{parent}, ui_{new Ui::LicenseInfo} {
    ui_->setupUi(this);
}

LicenseInfo::~LicenseInfo() { delete ui_; }
