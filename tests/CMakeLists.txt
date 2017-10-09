project(tests)
cmake_minimum_required(VERSION 2.6)

INCLUDE_DIRECTORIES(include/)

#add project files
file(GLOB_RECURSE SOURCES source/*.cpp)
file(GLOB_RECURSE HEADERS include/*.hpp)

include_directories(${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries( ${PROJECT_NAME} dicom)

enable_testing()
add_test(MyTest ${PROJECT_NAME})
