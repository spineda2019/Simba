include(GoogleTest)

set(TEST_FILES test_budget.cpp
               "${CMAKE_SOURCE_DIR}/simba/src/Budget.cpp")

add_executable(simba_tests ${TEST_FILES})

target_include_directories(simba_tests PRIVATE
    "${CMAKE_SOURCE_DIR}/simba/src/include"
)

target_compile_features(simba_tests PUBLIC cxx_std_23)
target_compile_definitions(simba_tests PRIVATE JSON_USE_IMPLICIT_CONVERSIONS=0)

target_link_libraries(simba_tests PRIVATE
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)

gtest_discover_tests(simba_tests)
