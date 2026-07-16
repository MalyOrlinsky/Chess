#pragma once
#include "../game_engine/PieceStatus.hpp"
#include "AnimConfig.hpp"
#include <string>
#include <map>

class PieceConfigManager {
public:
    explicit PieceConfigManager(const std::string& basePath);

    const AnimConfig& getConfig(const std::string& pieceCode,
                                const PieceStatus& state);

    AnimConfig parseConfig(const std::string& configPath);
    

private:
    std::string basePath;

    std::map<std::string, AnimConfig> cache;

    AnimConfig loadConfig(const std::string& pieceCode,
                          const PieceStatus& state);
};