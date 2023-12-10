cmake_minimum_required(VERSION 3.22)
project(
    libmterm
    VERSION 0.1.0
    DESCRIPTION "A mini pseudo-terminal implementation library."
    HOMEPAGE_URL "https://github.com/zakiaatot/libmterm"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)

if(NOT DEFINED TARGET)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(TARGET "linux")
    elseif(ANDROID)
        set(TARGET "android")
    else()
        set(TARGET "unkonwn")
    endif()
endif()

if(${TARGET} STREQUAL "linux")
    add_definitions(-DTARGET_LINUX)
    include_directories(${PROJECT_SOURCE_DIR}/jni/linux)
    add_library(mterm SHARED ${SRC})
    set_target_properties(
        mterm
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/linux
    )
    add_library(mterm_static STATIC ${SRC})
    set_target_properties(
        mterm_static
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/linux
    )
elseif(${TARGET} STREQUAL "android")
    add_definitions(-DTARGET_ANDROID)
    include_directories(${PROJECT_SOURCE_DIR}/jni/android)
    add_library(mterm SHARED ${SRC})
    set_target_properties(
        mterm
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/android
    )
    add_library(mterm_static STATIC ${SRC})
    set_target_properties(
        mterm_static
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output/android
    )
else()
    message(FATAL_ERROR "Only support for Android and Linux now!")
endif()