#include "PieceConfigManager.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <cctype>


PieceConfigManager::PieceConfigManager(const std::string& basePath)
    : basePath(basePath)
{
}


const AnimConfig& PieceConfigManager::getConfig(
    const std::string& pieceCode,
    const std::string& state)
{
    std::string key = pieceCode + "/" + state;

    if (cache.find(key) == cache.end())
        cache[key] = loadConfig(pieceCode, state);

    return cache[key];
}


AnimConfig PieceConfigManager::loadConfig(
    const std::string& pieceCode,
    const std::string& state)
{
    std::string path =
        basePath + "/" +
        pieceCode +
        "/states/" +
        state +
        "/config.json";

    return parseConfig(path);
}