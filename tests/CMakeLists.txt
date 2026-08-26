# tests/CMakeLists.txt

add_executable(loxpp_tests
    test_simple.cpp
)

target_link_libraries(loxpp_tests
    PRIVATE
        loxpp::core
        loxpp_warnings
        Catch2::Catch2WithMain
)

target_include_directories(loxpp_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

include(CTest)
include(Catch)
catch_discover_tests(loxpp_tests)