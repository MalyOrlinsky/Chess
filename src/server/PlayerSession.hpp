#pragma once

#include <string>
#include <websocketpp/common/connection_hdl.hpp>
#include "../src/model/Piece.hpp"

struct PlayerSession
{
    websocketpp::connection_hdl hdl;

    Color color;

    std::string username;
};