project(dicom)
cmake_minimum_required(VERSION 2.6)

add_definitions(-std=c++14)
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(SAMPLES_DIR ${CMAKE_SOURCE_DIR}/samples)
set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/resource)

add_definitions(-DSAMPLES_DIR="${SAMPLES_DIR}")
add_definitions(-DRESOURCE_DIR="${RESOURCE_DIR}")


INCLUDE_DIRECTORIES(${INCLUDE_DIR})

#add project files
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*)
file(GLOB_RECURSE HEADERS ${INCLUDE_DIR}/*)

#add other project files
file(GLOB_RECURSE SAMPLES ${SAMPLES_DIR}/*)
file(GLOB_RECURSE RESOURCES ${RESOURCE_DIR}/*)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RESOURCES} ${SAMPLES})
target_link_libraries(${PROJECT_NAME} ${Project_LIBRARIES} )

enable_testing()
add_subdirectory(tests)


