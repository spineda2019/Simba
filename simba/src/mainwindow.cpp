#include "moc_include/mainwindow.hpp"

#include <qaction.h>
#include <qdesktopservices.h>
#include <qfiledialog.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qurl.h>
#include <qwidget.h>
#include <ui_mainwindow.h>

#include "moc_include/LicenseInfo.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{new Ui::MainWindow} {
    ui_->setupUi(this);
    connect(this->ui_->action_license_info, &QAction::triggered, this,
            &MainWindow::OnLicenseInfoTriggered);
    connect(this->ui_->action_open_budget, &QAction::triggered, this,
            &MainWindow::OnOpenBudget);
    connect(this->ui_->action_create_budget, &QAction::triggered, this,
            &MainWindow::OnCreateBudget);
    connect(this->ui_->action_see_the_source_code, &QAction::triggered, this,
            &MainWindow::OnShowSourceCode);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::OnLicenseInfoTriggered() {
    LicenseInfo license_info{this};
    license_info.exec();
}

void MainWindow::OnOpenBudget() {
    constexpr const char* caption{"Open Simba Budget"};
    constexpr const char* filter{"Simba Files (*.simba)"};

    const QString to_open{
        QFileDialog::getOpenFileName(this, tr(caption), {}, tr(filter))};
    // TODO(SEP): implement
    {
        QMessageBox debug{this};
        if (to_open.isEmpty()) {
            debug.setText("No file picked...");

        } else {
            debug.setText(to_open);
        }
        (void)debug.exec();
    }
}

void MainWindow::OnCreateBudget() {
    constexpr const char* caption{"Open Simba Budget"};
    constexpr const char* filter{"Simba Files (*.simba)"};

    const QString to_create{
        QFileDialog::getSaveFileName(this, tr(caption), {}, tr(filter))};

    // TODO(SEP): implement
    {
        QMessageBox debug{this};
        if (to_create.isEmpty()) {
            debug.setText("No file picked...");

        } else {
            debug.setText(to_create);
        }
        (void)debug.exec();
    }
}

void MainWindow::OnShowSourceCode() const {
    const static QUrl url{"https://github.com/spineda2019/Simba"};
    QDesktopServices::openUrl(url);
};
