#ifndef SIMBA_SRC_MOC_INCLUDE_MAINWINDOW
#define SIMBA_SRC_MOC_INCLUDE_MAINWINDOW

#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qt6/QtCore/qobject.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include <Budget.hpp>
#include <array>

#include "Budget.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
    Q_OBJECT

 public:
    explicit MainWindow(QWidget* parent = nullptr);

    ~MainWindow();
    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

 private:
    void EnableOverview();
    void DisableOverview();

 private slots:
    void OnLicenseInfoTriggered();
    void OnOpenBudget();
    void OnCreateBudget();
    void OnShowSourceCode();

 private:
    QString current_budget_path_{};
    Ui::MainWindow* ui_;
    simba::Budget* open_budget_{nullptr};
};

#endif  // SIMBA_SRC_MOC_INCLUDE_MAINWINDOW
