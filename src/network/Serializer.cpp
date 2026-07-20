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

        case MessageType::Click:
            return "Click";

        case MessageType::Play:
            return "Play";

        case MessageType::Snapshot:
            return "Snapshot";

        case MessageType::Text:
            return "Text";

        case MessageType::Error:
            return "Error";
        }

        return "Unknown";
    }

    static MessageType stringToMessageType(const std::string &type)
    {
        if (type == "Login")
            return MessageType::Login;

        if (type == "Click")
            return MessageType::Click;

        if (type == "Play")
            return MessageType::Play;

        if (type == "Snapshot")
            return MessageType::Snapshot;

        if (type == "Text")
            return MessageType::Text;

        if (type == "Error")
            return MessageType::Error;

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

        message.type =
            stringToMessageType(json.at("type").get<std::string>());

        message.payload =
            json.at("payload").get<std::string>();

        return message;
    }

}