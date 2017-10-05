project(tests)
cmake_minimum_required(VERSION 2.6)

set(SOURCES
    main.cpp
)

set(HEADERS
    catch.hpp
)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries( ${PROJECT_NAME} edicom )


enable_testing()
add_test(MyTest ${PROJECT_NAME})
