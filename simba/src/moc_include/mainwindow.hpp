#ifndef SIMBA_SRC_MOC_INCLUDE_MAINWINDOW
#define SIMBA_SRC_MOC_INCLUDE_MAINWINDOW

#include <qmainwindow.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>

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

 private:
    Ui::MainWindow* ui_;
};

#endif  // SIMBA_SRC_MOC_INCLUDE_MAINWINDOW
