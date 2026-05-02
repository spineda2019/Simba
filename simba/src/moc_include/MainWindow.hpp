#ifndef SIMBA_SRC_MOC_INCLUDE_MAINWINDOW
#define SIMBA_SRC_MOC_INCLUDE_MAINWINDOW

#include <qmainwindow.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include <Budget.hpp>
#include <meta.hpp>

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

 public:  // rule of five
    ~MainWindow();
    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

 private:
    void EnableOverview();
    void DisableOverview();
    void UpdateTotal(simba::meta::signed_native_word_t total);

 private slots:
    void OnLicenseInfoTriggered();
    void OnOpenBudget();
    void OnCreateBudget();
    void OnShowSourceCode();
    void OnNewTransaction();
    void OnNewCategory();
    void OnShowSummary();

 private:
    QString current_budget_path_{};
    Ui::MainWindow* ui_;
    simba::Budget* open_budget_{nullptr};
};

#endif  // SIMBA_SRC_MOC_INCLUDE_MAINWINDOW
