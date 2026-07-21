#pragma once

namespace Network
{

    enum class MessageType
    {
        Login,
        Command,
        Play,
        Snapshot,
        Text,
        Error,
        PlayerInfo
    };

}