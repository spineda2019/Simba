#ifndef SIMBA_SRC_MOC_INCLUDE_LICENSEINFO_HPP_
#define SIMBA_SRC_MOC_INCLUDE_LICENSEINFO_HPP_

#include <qdialog.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include <qwindowdefs.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class LicenseInfo;
}
QT_END_NAMESPACE

class LicenseInfo final : public QDialog {
    Q_OBJECT

 public:
    explicit LicenseInfo(QWidget* parent = nullptr);

    ~LicenseInfo();
    LicenseInfo(const LicenseInfo&) = delete;
    LicenseInfo(LicenseInfo&&) = delete;
    LicenseInfo& operator=(const LicenseInfo&) = delete;
    LicenseInfo& operator=(LicenseInfo&&) = delete;

 private:
    Ui::LicenseInfo* ui_;
};

#endif  // SIMBA_SRC_MOC_INCLUDE_LICENSEINFO_HPP_
