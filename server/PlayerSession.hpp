#pragma once

#include <string>
#include <chrono>
#include <websocketpp/common/connection_hdl.hpp>
#include "../src/model/Piece.hpp"

struct PlayerSession
{
    websocketpp::connection_hdl hdl;

    std::string username;
    Color color = Color::None;

    int rating = 1200;
    bool loggedIn = false;
    int gameId = -1;

    bool searchingGame = false;
    std::chrono::steady_clock::time_point searchStartTime;
};