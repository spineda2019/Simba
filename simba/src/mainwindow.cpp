#include "moc_include/mainwindow.hpp"

#include <qmainwindow.h>
#include <qwidget.h>

#include <ui_mainwindow.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}, ui_{new Ui::MainWindow} {
    ui_->setupUi(this);
}

MainWindow::~MainWindow() { delete ui_; }
