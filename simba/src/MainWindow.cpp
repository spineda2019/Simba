
#include "moc_include/MainWindow.hpp"

#include <qaction.h>
#include <qdesktopservices.h>
#include <qfiledialog.h>
#include <qmainwindow.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qurl.h>
#include <qwidget.h>
#include <ui_MainWindow.h>

#include <array>
#include <string>

#include "include/Budget.hpp"
#include "include/style.hpp"
#include "moc_include/LicenseInfo.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent},
      ui_{new Ui::MainWindow},
      current_budget_path_{},
      open_budget_{nullptr} {
    ui_->setupUi(this);
    connect(this->ui_->action_license_info, &QAction::triggered, this,
            &MainWindow::OnLicenseInfoTriggered);
    connect(this->ui_->action_open_budget, &QAction::triggered, this,
            &MainWindow::OnOpenBudget);
    connect(this->ui_->action_create_budget, &QAction::triggered, this,
            &MainWindow::OnCreateBudget);
    connect(this->ui_->action_see_the_source_code, &QAction::triggered, this,
            &MainWindow::OnShowSourceCode);

    const std::array<QPushButton*, 3> buttons{
        ui_->button_add_transaction,
        ui_->button_add_category,
        ui_->button_show_summary,
    };

    for (QPushButton* button : buttons) {
        button->setStyleSheet(simba::style::sheets::button);
    }
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
    if (to_open.isEmpty()) {
        (void)QMessageBox::warning(this, tr("Simba"),
                                   tr("No file selected. Budget not loaded"));
    } else {
        // TODO(SEP): implement JSON parsing
        (void)QMessageBox::information(this, tr("Simba"), tr("TODO"));
        const std::string path{to_open.toStdString()};
    }
}

void MainWindow::OnCreateBudget() {
    constexpr const char* caption{"Open Simba Budget"};
    constexpr const char* filter{"Simba Files (*.simba)"};

    const QString to_create{
        QFileDialog::getSaveFileName(this, tr(caption), {}, tr(filter))};

    // TODO(SEP): implement
    if (to_create.isEmpty()) {
        (void)QMessageBox::warning(this, tr("Simba"),
                                   tr("No file selected. Budget not created"));

    } else {
        // TODO(SEP): reuse memory perhaps?
        if (open_budget_) {
            open_budget_->SaveToFile(current_budget_path_.toStdString());
            delete open_budget_;
        }
        current_budget_path_ = to_create;
        open_budget_ = new simba::Budget;
    }
}

void MainWindow::OnShowSourceCode() const {
    const static QUrl url{"https://github.com/spineda2019/Simba"};
    QDesktopServices::openUrl(url);
};
