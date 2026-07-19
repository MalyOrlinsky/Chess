cmake_minimum_required(VERSION 3.16)
project(KungFuChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV - same folder as the example project
set(OPENCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cpp/OpenCV_451")
set(OPENCV_INCLUDE_DIR "${OPENCV_DIR}/include")
set(OPENCV_LIB_DIR     "${OPENCV_DIR}/bin")

# All sources: game logic + renderer + Img library
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "cpp/src/img.cpp"
    "tests/*.cpp"
)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main_sfml.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_new.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OPENCV_INCLUDE_DIR}
    "cpp/src"       # so src/ can include img.hpp
    "src"
)

target_link_directories(${PROJECT_NAME} PRIVATE ${OPENCV_LIB_DIR})

target_link_libraries(${PROJECT_NAME}
    $<$<CONFIG:Debug>:${OPENCV_LIB_DIR}/opencv_world451d.lib>
    $<$<CONFIG:Release>:${OPENCV_LIB_DIR}/opencv_world451.lib>
    $<$<CONFIG:RelWithDebInfo>:${OPENCV_LIB_DIR}/opencv_world451.lib>
    $<$<CONFIG:MinSizeRel>:${OPENCV_LIB_DIR}/opencv_world451.lib>
)

# Copy OpenCV DLL next to the exe
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<$<CONFIG:Debug>:${OPENCV_LIB_DIR}/opencv_world451d.dll>
        $<$<CONFIG:Release>:${OPENCV_LIB_DIR}/opencv_world451.dll>
        $<$<CONFIG:RelWithDebInfo>:${OPENCV_LIB_DIR}/opencv_world451.dll>
        $<$<CONFIG:MinSizeRel>:${OPENCV_LIB_DIR}/opencv_world451.dll>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Werror
    )
endif()