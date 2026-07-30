cmake_minimum_required(VERSION 3.16)

project(KungFuChess)

if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PQXX REQUIRED libpqxx)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# =====================
# External
# =====================

if(WIN32)
    set(OPENCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/opencv2/OpenCV_451")
    set(OPENCV_INCLUDE_DIR "${OPENCV_DIR}/include")
    set(OPENCV_LIB_DIR "${OPENCV_DIR}/bin")
endif()

if(UNIX)
    find_package(OpenCV REQUIRED)
    set(OPENCV_INCLUDE_DIR ${OpenCV_INCLUDE_DIRS})
endif()

# =====================
# Shared source
# =====================

file(GLOB_RECURSE COMMON_SOURCES
    "src/config/*.cpp"
)

file(GLOB_RECURSE COMMON_HEADERS
    "src/config/*.hpp"
)



# =====================
# Client
# =====================

file(GLOB_RECURSE CLIENT_SOURCES
    "opencv2/src/img.cpp"
    "client/*.cpp"
    "src/renderer/*.cpp"
    "src/network/*.cpp"
)

file(GLOB_RECURSE CLIENT_HEADERS
    "opencv2/src/img.hpp"
    "client/*.hpp"
    "src/renderer/*.hpp"
    "src/network/*.hpp"
)


add_executable(KungFuChessClient
    ${CLIENT_SOURCES}
    ${CLIENT_HEADERS}
    ${COMMON_SOURCES}
    ${COMMON_HEADERS}
)


target_include_directories(KungFuChessClient PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/external/websocketpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/external"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/asio/asio/include"

    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/opencv2/src"

    "${OPENCV_INCLUDE_DIR}"
)


target_compile_definitions(KungFuChessClient PRIVATE
    ASIO_STANDALONE
    _WEBSOCKETPP_CPP11_STL_
)


target_link_directories(KungFuChessClient PRIVATE
    "${OPENCV_LIB_DIR}"
)


if(WIN32)
    target_link_libraries(KungFuChessClient
        $<$<CONFIG:Debug>:opencv_world451d.lib>
        $<$<CONFIG:Release>:opencv_world451.lib>
    )
endif()

if(UNIX)
    target_link_libraries(KungFuChessClient
        ${OpenCV_LIBS}
    )
endif()


# =====================
# Server
# =====================

file(GLOB_RECURSE SERVER_SOURCES
    "server/*.cpp"
    "src/model/*.cpp"
    "src/movement/*.cpp"
    "src/rule_engine/*.cpp"
    "src/arbiter/*.cpp"
    "src/game_engine/*.cpp"
    "src/network/*.cpp"
    "src/commands/*.cpp"
    "src/text_io/*.cpp"
    "src/audio/*.cpp"
    "src/controllerClick/*.cpp"
)


file(GLOB_RECURSE SERVER_HEADERS
    "server/*.hpp"
    "src/model/*.hpp"
    "src/movement/*.hpp"
    "src/rule_engine/*.hpp"
    "src/arbiter/*.hpp"
    "src/game_engine/*.hpp"
    "src/network/*.hpp"
    "src/commands/*.hpp"
    "src/text_io/*.hpp"
    "src/audio/*.hpp"
    "src/controllerClick/*.hpp"
)


add_executable(KungFuChessServer
    ${SERVER_SOURCES}
    ${SERVER_HEADERS}
    ${COMMON_SOURCES}
    ${COMMON_HEADERS}
)


target_include_directories(KungFuChessServer PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/external/websocketpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/external"
    "${CMAKE_CURRENT_SOURCE_DIR}/external/asio/asio/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/opencv2/src"
    "${OPENCV_INCLUDE_DIR}"
)

target_compile_definitions(KungFuChessServer PRIVATE
    ASIO_STANDALONE
    _WEBSOCKETPP_CPP11_STL_
)

if(UNIX)
    target_include_directories(KungFuChessServer PRIVATE ${PQXX_INCLUDE_DIRS})
    target_link_libraries(KungFuChessServer ${PQXX_LIBRARIES})
endif()

# =====================
# Warnings
# =====================

if(MSVC)

    target_compile_options(KungFuChessClient PRIVATE /W4)
    target_compile_options(KungFuChessServer PRIVATE /W4)

else()

    target_compile_options(KungFuChessClient PRIVATE
        -Wall
        -Wextra
    )

    target_compile_options(KungFuChessServer PRIVATE
        -Wall
        -Wextra
    )

endif()



# =====================
# Copy OpenCV DLL
# =====================

if(WIN32)

    add_custom_command(
        TARGET KungFuChessClient
        POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_if_different

        $<$<CONFIG:Debug>:${OPENCV_LIB_DIR}/opencv_world451d.dll>
        $<$<CONFIG:Release>:${OPENCV_LIB_DIR}/opencv_world451.dll>

        $<TARGET_FILE_DIR:KungFuChessClient>
    )

endif()