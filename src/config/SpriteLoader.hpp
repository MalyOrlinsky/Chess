#pragma once
#include "../../cpp/src/img.hpp"
#include "../game_engine/PieceStatus.hpp"
#include "../config/AnimConfig.hpp"
#include "../config/PieceConfigManager.hpp"
#include "../model/Piece.hpp"
#include <string>
#include <vector>
#include <map>

class SpriteLoader {
public:

    explicit SpriteLoader(const std::string& basePath);

    PieceConfigManager configManager;

    const std::vector<Img>& getFrames(const std::string& pieceCode,
                                      const PieceStatus& state);

private:
    std::string basePath;

    struct Entry {
        std::vector<Img> frames;
        AnimConfig config;
    };

    std::map<std::string, Entry> cache;

    Entry load(const std::string& pieceCode, const PieceStatus& state);
};
