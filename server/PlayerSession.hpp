#pragma once

#include <string>
#include <chrono>
#include <websocketpp/common/connection_hdl.hpp>
#include "../src/model/Piece.hpp"

struct PlayerSession
{
    int id;
    std::string username;

    websocketpp::connection_hdl hdl;
    Color color = Color::None;

    int rating = 1200;
    bool loggedIn = false;
    int gameId = -1;

    bool searchingGame = false;
    std::chrono::steady_clock::time_point searchStartTime;

    bool disconnected = false;
    std::chrono::steady_clock::time_point disconnectTime;
};