# TODO
I need to build fltk using the zig build system. I will be parsing through
the cmake file tree and translating it to build script.

## CMake Files
These are the list of files that I need to go through. At a minimum, these all
need to be at least looked through:

* CMakeLists.txt
    * CMake/fl_debug_var.cmake
    * CMake/fl_debug_pkg.cmake
    * CMake/fl_add_library.cmake
    * CMake/setup.cmake
        * TestBigEndian (Sys)
        * GNUInstallDirs (Sys)
        * CheckTypeSize (Sys)
    * CMake/resources.cmake
        * CheckIncludeFiles (?)
        * CheckSymbolExists (?)
        * FindPkgConfig (?)
        * CheckFunctionExists (?)
    * CMake/options.cmake
        * FindX11 (Sys)
        * FindThreads (Sys)
    * CMake/gen_config.cmake
    * CMake/variables.cmake
    * CMake/export.cmake
    * CMakePackageConfigHelpers (?)
    * CMake/install.cmake
    * GenerateExportHeader (?)
    * FeatureSummary (?)
    * CMake/fl_summary.cmake

Files marked with (?) don't seem to be checked in to FLTK's source tree, and I
need to figure out what exactly their inclusion (literally with "include(...)"
means. Files marked with (Sys) seem to be included with an installation of
cmake.

## Targets
Simply looking through the files is not enough, what we really need is a list of
targets that can be built:

* TODO

## .in files
The scary configure pattern. These need to be quashed and replaced with the zig
build system:

* CMake/FLTKConfig.cmake.in
* CMake/MacOSXBundleInfo.plist.in
* CMake/cmake_uninstall.cmake.in
* CMake/install-symlinks.cmake.in
* CMake/macOS-bundle-wrapper.in
* config.h.in
* documentation/Doxyfile.in
* documentation/copyright.dox.in
* documentation/generated.dox.in
* documentation/make_pdf.in
* documentation/src/fltk-title.tex.in
* documentation/version.dox.in
* fl_config.h.in
* fltk-config.in
* fluid/documentation/Doxyfile.in
* fluid/documentation/copyright.dox.in
* fluid/documentation/generated.dox.in
* fluid/documentation/make_pdf.in
* fluid/documentation/src/fluid-title.tex.in
* fluid/documentation/version.dox.in

Most of these do not seem to be related to the main fltk library itself. It may
still be nice to port...
