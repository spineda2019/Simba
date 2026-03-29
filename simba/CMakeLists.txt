set(CPP_FILES
    "src/main.cpp"
    "src/MainWindow.cpp"
    "src/LicenseInfo.cpp"
    "src/Budget.cpp"
)
set(UI_FILES
    "src/ui/MainWindow.ui"
    "src/ui/license.ui")
set(MOC_HEADERS
    "src/moc_include/MainWindow.hpp"
    "src/moc_include/LicenseInfo.hpp"
)

qt_standard_project_setup()
qt_add_executable(simba ${MOC_HEADERS} ${CPP_FILES})
qt_add_ui(simba SOURCES ${UI_FILES})
target_include_directories(simba PRIVATE src/include)

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
target_link_libraries(simba PRIVATE Qt6::Core
                                    Qt6::Gui
                                    Qt6::Widgets)
target_link_libraries(simba PRIVATE nlohmann_json::nlohmann_json)

target_compile_definitions(simba PRIVATE JSON_USE_IMPLICIT_CONVERSIONS=0)


set_target_properties(simba PROPERTIES
                      WIN32_EXECUTABLE ON  # prevents console window on windows
                      MACOSX_BUNDLE ON     # Creates a full AppBundle on OSX
)

install(TARGETS simba
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin)

qt_generate_deploy_app_script(
    TARGET simba
    OUTPUT_SCRIPT deploy_script
)
install(SCRIPT ${deploy_script})
