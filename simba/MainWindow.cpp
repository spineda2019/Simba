// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

#include "views/MainWindow.hpp"

#include <FL/Enumerations.H>
#include <FL/Fl_Button.H>

#include "logic/filesystem.hpp"

namespace simba {

void MainWindow::Show() noexcept { window_.show(); }

void MainWindow::AddTransactionClicked() {}

void MainWindow::VisualizeClicked() {}

void MainWindow::AccountSummaryClicked() {}

void MainWindow::AccountEditorClicked() {}

void MainWindow::LoadConfigClicked() { fs::GetConfigFile(); }

}  // namespace simba
