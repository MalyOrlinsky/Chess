#include "SpriteLoader.hpp"
#include "../Constants.hpp"
#include <filesystem>

namespace fs = std::filesystem;

SpriteLoader::SpriteLoader(const std::string& basePath) : 
        basePath(basePath), configManager(basePath) {}

const std::vector<Img>& SpriteLoader::getFrames(const std::string& pieceCode,
                                                  const PieceStatus& state) {
    const std::string key = pieceCode + "/" + PieceStatusToString(state);
    if (cache.find(key) == cache.end())
        cache[key] = load(pieceCode, state);
    return cache[key].frames;
}

SpriteLoader::Entry SpriteLoader::load(const std::string& pieceCode,
                                        const PieceStatus& state) {
    std::string stateDir = basePath + "/" + pieceCode + "/states/" + PieceStatusToString(state);
    Entry entry;
    entry.config = configManager.parseConfig(stateDir + "/config.json");

    std::string spritesDir = stateDir + "/sprites";
    std::vector<fs::path> paths;
    for (auto& p : fs::directory_iterator(spritesDir))
        if (p.path().extension() == ".png" || p.path().extension() == ".PNG")
            paths.push_back(p.path());

    std::sort(paths.begin(), paths.end());
    for (auto& p : paths) {
        Img img;
        img.read(p.string(), {CELL_SIZE_PX, CELL_SIZE_PX});
        entry.frames.push_back(std::move(img));
    }
    return entry;
}
