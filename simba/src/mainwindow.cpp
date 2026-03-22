#include "moc_include/mainwindow.hpp"

#include <qaction.h>
#include <qmainwindow.h>
#include <qwidget.h>
#include <ui_mainwindow.h>

#include "moc_include/LicenseInfo.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{new Ui::MainWindow} {
    ui_->setupUi(this);
    connect(this->ui_->actionLicense_info, &QAction::triggered, this,
            &MainWindow::OnLicenseInfoTriggered);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::OnLicenseInfoTriggered() {
    LicenseInfo license_info{this};
    license_info.exec();
}
