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
        * if targeting windows and using MSVC, defines following macros
            * WIN32_LEAN_AND_MEAN
            * _CRT_SECURE_NO_WARNINGS
        * If MSVC_VERSION >= 1900, add /utf-8 flag
    * CMake/resources.cmake
        * CheckIncludeFiles (?)
        * CheckSymbolExists (?)
        * FindPkgConfig (?)
        * CheckFunctionExists (?)
    * CMake/options.cmake
        * FindX11 (Sys)
        * FindThreads (Sys)
        * zlib/CMakeLists.txt
            * only if FLTK_USE_BUNDLED_ZLIB is defined
        * png/CMakeLists.txt
            * only if FLTK_USE_BUNDLED_PNG is defined
        * jpeg/CMakeLists.txt
            * only if FLTK_USE_BUNDLED_JPEG is defined
        * documentation/CMakeLists.txt
            * only if FLTK_BUILD_HTML_DOCS or FLTK_BUILD_PDF_DOCS is defined
        * fluid/documentation/CMakeLists.txt
            * only if FLTK_BUILD_FLUID_DOCS is defined
        * if FLTK_OPTION_LARGE_FILE is set to true and MSVC is NOT being used, define following macros
            * _LARGEFILE_SOURCE
            * _LARGEFILE64_SOURCE
            * _FILE_OFFSET_BITS=64
    * CMake/variables.cmake
    * CMake/export.cmake
    * CMakePackageConfigHelpers (?)
    * CMake/install.cmake
    * GenerateExportHeader (?)
    * FeatureSummary (?)
    * CMake/fl_summary.cmake
    * cairo/CMakeLists.txt
    * src/CMakeLists.txt
    * fluid/CMakeLists.txt
    * fltk-options/CMakeLists.txt
    * test/CMakeLists.txt
    * examples/CMakeLists.txt

Files marked with (?) don't seem to be checked in to FLTK's source tree, and I
need to figure out what exactly their inclusion (literally with "include(...)"
means. Files marked with (Sys) seem to be included with an installation of
cmake.

## Targets
Simply looking through the files is not enough, what we really need is a list of
targets that can be built:

* call_main
    * Only when building shared libs via MSVC (DLLs)

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
