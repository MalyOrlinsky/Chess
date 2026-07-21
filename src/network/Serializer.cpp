#include "Serializer.hpp"

#include <nlohmann/json.hpp>
#include <stdexcept>

namespace Network
{

    static std::string messageTypeToString(MessageType type)
    {
        switch (type)
        {
        case MessageType::Login:
            return "Login";

        case MessageType::Command:
            return "Command";

        case MessageType::Play:
            return "Play";

        case MessageType::Snapshot:
            return "Snapshot";

        case MessageType::Text:
            return "Text";

        case MessageType::Error:
            return "Error";

        case MessageType::PlayerInfo:
            return "PlayerInfo";
        }

        return "Unknown";
    }

    static MessageType stringToMessageType(const std::string &type)
    {
        if (type == "Login")
            return MessageType::Login;

        if (type == "Command")
            return MessageType::Command;

        if (type == "Play")
            return MessageType::Play;

        if (type == "Snapshot")
            return MessageType::Snapshot;

        if (type == "Text")
            return MessageType::Text;

        if (type == "Error")
            return MessageType::Error;

        if (type == "PlayerInfo")
            return MessageType::PlayerInfo;

        throw std::runtime_error("Unknown message type");
    }

    std::string Serializer::serialize(const Message &message)
    {
        nlohmann::json json;

        json["type"] = messageTypeToString(message.type);
        json["payload"] = message.payload;

        return json.dump();
    }

    Message Serializer::deserialize(const std::string &data)
    {
        auto json = nlohmann::json::parse(data);
        Message message;

        message.type = stringToMessageType(json.at("type").get<std::string>());
        message.payload = json.at("payload").get<std::string>();

        return message;
    }

    std::string Serializer::serializeSnapshot(const GameSnapshot &snapshot)
    {
        nlohmann::json json;

        json["rows"] = snapshot.rows;
        json["cols"] = snapshot.cols;

        json["gameOver"] = snapshot.gameOver;
        json["winner"] = snapshot.winner;

        json["selected"] = nlohmann::json::array();

        for (const auto &[color, pos] : snapshot.selected)
        {
            json["selected"].push_back({{"color", static_cast<int>(color)},
                                        {"row", pos.first},
                                        {"col", pos.second}});
        }

        json["playerWhite"] = snapshot.playerWhite;
        json["playerBlack"] = snapshot.playerBlack;

        json["scoreWhite"] = snapshot.scoreWhite;
        json["scoreBlack"] = snapshot.scoreBlack;

        json["movesLogWhite"] = snapshot.movesLogWhite;
        json["movesLogBlack"] = snapshot.movesLogBlack;

        for (const auto &row : snapshot.cells)
        {
            nlohmann::json jsonRow;

            for (const auto &cell : row)
            {
                jsonRow.push_back({{"id", cell.id},
                                   {"type", cell.type},
                                   {"color", static_cast<int>(cell.color)},
                                   {"status", static_cast<int>(cell.status)}});
            }

            json["cells"].push_back(jsonRow);
        }

        return json.dump();
    }

    GameSnapshot Serializer::deserializeSnapshot(const std::string &data)
    {
        auto json = nlohmann::json::parse(data);

        GameSnapshot snapshot;

        snapshot.rows = json.at("rows").get<int>();
        snapshot.cols = json.at("cols").get<int>();

        snapshot.gameOver = json.at("gameOver").get<bool>();
        snapshot.winner = json.at("winner").get<std::string>();

        if (json.contains("selected"))
        {
            for (const auto &item : json["selected"])
            {
                Color color =
                    static_cast<Color>(item.at("color").get<int>());

                int row = item.at("row").get<int>();
                int col = item.at("col").get<int>();

                snapshot.selected[color] = {row, col};
            }
        }

        snapshot.playerWhite =
            json.at("playerWhite").get<std::string>();

        snapshot.playerBlack =
            json.at("playerBlack").get<std::string>();

        snapshot.scoreWhite =
            json.at("scoreWhite").get<int>();

        snapshot.scoreBlack =
            json.at("scoreBlack").get<int>();

        snapshot.movesLogWhite =
            json.at("movesLogWhite")
                .get<std::vector<std::string>>();

        snapshot.movesLogBlack =
            json.at("movesLogBlack")
                .get<std::vector<std::string>>();

        snapshot.cells.resize(snapshot.rows);

        const auto &jsonCells = json.at("cells");

        for (int r = 0; r < snapshot.rows; ++r)
        {
            snapshot.cells[r].resize(snapshot.cols);

            for (int c = 0; c < snapshot.cols; ++c)
            {
                const auto &jCell = jsonCells[r][c];

                CellSnapshot cell;

                cell.id =
                    jCell.at("id").get<int>();

                cell.type =
                    jCell.at("type").get<char>();

                cell.color =
                    static_cast<Color>(
                        jCell.at("color").get<int>());

                cell.status =
                    static_cast<PieceStatus>(
                        jCell.at("status").get<int>());

                snapshot.cells[r][c] = cell;
            }
        }

        return snapshot;
    }
}