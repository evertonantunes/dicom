project(dicom)
cmake_minimum_required(VERSION 2.6)

add_definitions(-std=c++14)
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(SAMPLES_DIR ${CMAKE_SOURCE_DIR}/samples)
set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/resource)

add_definitions(-DSAMPLES_DIR="${SAMPLES_DIR}")

INCLUDE_DIRECTORIES(${INCLUDE_DIR})

set(SOURCES
    ${SOURCE_DIR}/value_representation.cpp
    ${SOURCE_DIR}/dicom_dictionary.cpp
    ${SOURCE_DIR}/tag.cpp
    ${SOURCE_DIR}/utility.cpp
    ${SOURCE_DIR}/parser.cpp
    ${SOURCE_DIR}/sequence.cpp
)

set(HEADERS
    ${INCLUDE_DIR}/value_representation.hpp
    ${INCLUDE_DIR}/dicom_dictionary.hpp
    ${INCLUDE_DIR}/tag.hpp
    ${INCLUDE_DIR}/utility.hpp
    ${INCLUDE_DIR}/parser.hpp
    ${INCLUDE_DIR}/sequence.hpp
)

file(GLOB SAMPLES ${SAMPLES_DIR}/*)
file(GLOB RESOURCES ${RESOURCE_DIR}/*)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} ${RESOURCES} ${SAMPLES})
target_link_libraries(${PROJECT_NAME} ${Project_LIBRARIES} )

enable_testing()
add_subdirectory(tests)


