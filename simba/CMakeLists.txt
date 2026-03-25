set(CPP_FILES
    "src/main.cpp"
    "src/MainWindow.cpp"
    "src/LicenseInfo.cpp"
)
set(UI_FILES
    "ui/MainWindow.ui"
    "ui/license.ui")
set(MOC_HEADERS
    "src/moc_include/MainWindow.hpp"
    "src/moc_include/LicenseInfo.hpp"
)

qt_standard_project_setup()
qt_add_executable(simba ${MOC_HEADERS} ${CPP_FILES})
qt_add_ui(simba SOURCES ${UI_FILES})

if(MSVC)
    target_compile_options(simba PRIVATE
        /W4
        /WX
        /permissive-
    )
else()
    target_compile_options(simba PRIVATE -Wall
                                        -Wextra
                                        -Wpedantic
                                        -Wshadow
                                        -Wconversion
                                        -Werror)
endif()

target_compile_features(simba PUBLIC cxx_std_23)
target_link_libraries(simba PRIVATE
                                    Qt6::Core
                                    Qt6::Gui
                                    Qt6::Widgets)

set_target_properties(simba PROPERTIES
                      WIN32_EXECUTABLE ON  # prevents console window on windows
                      MACOSX_BUNDLE ON     # Creates a full AppBundle on OSX
)

install(TARGETS simba
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin)
