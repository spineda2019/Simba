#include "moc_include/mainwindow.hpp"

#include <qmainwindow.h>
#include <qwidget.h>
#include <ui_mainwindow.h>

#include "moc_include/LicenseInfo.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{new Ui::MainWindow} {
    ui_->setupUi(this);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::OnLicenseInfoTriggered() {
    LicenseInfo license_info{this};
    license_info.show();
}
