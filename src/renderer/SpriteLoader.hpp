#pragma once
#include "../../cpp/src/img.hpp"
#include "../arbiter/Motion.hpp"
#include "AnimConfig.hpp"
#include <string>
#include <vector>
#include <map>

class SpriteLoader {
public:
    explicit SpriteLoader(const std::string& basePath) : basePath(basePath) {}

    // Returns frames for pieceCode+state (loads on first call, cached after)
    const std::vector<Img>& getFrames(const std::string& pieceCode,
                                      const PieceStatus& state);

    // Returns config for pieceCode+state
    const AnimConfig& getConfig(const std::string& pieceCode,
                                const PieceStatus& state);

private:
    std::string basePath;  // path to pieces2/

    struct Entry {
        std::vector<Img> frames;
        AnimConfig config;
    };

    std::map<std::string, Entry> cache;  // key: "PW/idle"

    Entry load(const std::string& pieceCode, const PieceStatus& state);
    AnimConfig parseConfig(const std::string& configPath);
};
