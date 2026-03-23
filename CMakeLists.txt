cmake_minimum_required(VERSION 3.14)
# project(wotcpp VERSION 1.0 LANGUAGES CXX)

# Allow building as part of a parent project
option(WOTCPP_BUILD_TESTS "Build tests for wotcpp" OFF)

add_subdirectory(Core)

# Include headers
# target_include_directories(${PROJECT_NAME}
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#         $<INSTALL_INTERFACE:include>
# )

# Set C++ standard
#target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
#target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -static-libasan)

# If building standalone, enable tests
if (MYLIB_BUILD_TESTS AND CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # enable_testing()
    # add_executable(${PROJECT_NAME}_test tests/test.cpp)
    # target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})
    # add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)
endif()

