cmake_minimum_required(VERSION 3.16)

project(KungFuChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =====================
# OpenCV
# =====================

set(OPENCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/img/OpenCV_451")
set(OPENCV_INCLUDE_DIR "${OPENCV_DIR}/include")
set(OPENCV_LIB_DIR "${OPENCV_DIR}/bin")


# =====================
# Common sources
# =====================

file(GLOB_RECURSE COMMON_SOURCES
    "src/config/JsonHelper.cpp"
    "src/config/PieceConfigManager.cpp"
)


file(GLOB_RECURSE COMMON_HEADERS
    "src/config/JsonHelper.hpp"
    "src/config/PieceConfigManager.hpp"
)

# =====================
# Client sources
# =====================

file(GLOB_RECURSE CLIENT_SOURCES
    "src/renderer/*.cpp"
    "src/controllerClick/*.cpp"
    "src/network/*.cpp"
    "img/src/img.cpp"
    "src/config/SpriteLoader.cpp"
)

file(GLOB_RECURSE CLIENT_HEADERS
    "src/renderer/*.hpp"
    "src/controllerClick/*.hpp"
    "src/network/*.hpp"
    "img/src/img.hpp"
    "src/config/SpriteLoader.hpp"
)

# =====================
# Server sources
# =====================

file(GLOB_RECURSE SERVER_SOURCES
    "audio/*.cpp"
    "audios/*.cpp"
    "src/buffer/*.cpp"
    "src/model/*.cpp"
    "src/movement/*.cpp"
    "src/rule_engine/*.cpp"
    "src/arbiter/*.cpp"
    "src/config/AnimationConfigProvider.cpp"
    "src/game_engine/*.cpp"
    "src/network/Serializer.cpp"
    "src/commands/*.cpp"
    "src/text_io/*.cpp"
    "src/controllerClick/*.cpp"
    "server/*.cpp"
)

file(GLOB_RECURSE SERVER_GAME_HEADERS
    "audio/*.hpp"
    "audios/*.hpp"
    "src/buffer/*.hpp"
    "src/model/*.hpp"
    "src/movement/*.hpp"
    "src/rule_engine/*.hpp"
    "src/arbiter/*.hpp"
    "src/config/AnimationConfigProvider.hpp"
    "src/game_engine/*.hpp"
    "src/network/Serializer.hpp"
    "src/commands/*.hpp"
    "src/text_io/*.hpp"
    "src/controllerClick/*.hpp"
    "server/*.hpp"
)



# =====================
# Client executable
# =====================

add_executable(KungFuChessClient
    main.cpp
    ${CLIENT_SOURCES}
    ${CLIENT_HEADERS}
    ${COMMON_SOURCES}
    ${COMMON_HEADERS}
)

target_include_directories(KungFuChessClient PRIVATE
    # websocket
    "${CMAKE_CURRENT_SOURCE_DIR}/external/websocketpp"
    # json
    "${CMAKE_CURRENT_SOURCE_DIR}/external"
    # asio
    "${CMAKE_CURRENT_SOURCE_DIR}/external/asio/asio/include"
    # opencv
    "${OPENCV_INCLUDE_DIR}"
    # project
    "${CMAKE_CURRENT_SOURCE_DIR}/img/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(KungFuChessClient PRIVATE
    ASIO_STANDALONE
    _WEBSOCKETPP_CPP11_STL_
)

target_link_directories(KungFuChessClient PRIVATE
    ${OPENCV_LIB_DIR}
)

target_link_libraries(KungFuChessClient
    $<$<CONFIG:Debug>:${OPENCV_LIB_DIR}/opencv_world451d.lib>
    $<$<CONFIG:Release>:${OPENCV_LIB_DIR}/opencv_world451.lib>
)

# =====================
# Server executable
# =====================

add_executable(KungFuChessServer
    server/main.cpp
    ${SERVER_SOURCES}
    ${SERVER_GAME_SOURCES}
    ${SERVER_GAME_HEADERS}
    ${COMMON_SOURCES}
    ${COMMON_HEADERS}
)

target_include_directories(KungFuChessServer PRIVATE
    # websocket
    "${CMAKE_CURRENT_SOURCE_DIR}/external/websocketpp"
    # json
    "${CMAKE_CURRENT_SOURCE_DIR}/external"
    # asio
    "${CMAKE_CURRENT_SOURCE_DIR}/external/asio/asio/include"
    # project
    "${CMAKE_CURRENT_SOURCE_DIR}/img/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(KungFuChessServer PRIVATE
    ASIO_STANDALONE
    _WEBSOCKETPP_CPP11_STL_
)

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