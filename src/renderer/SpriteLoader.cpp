#include "SpriteLoader.hpp"
#include "../Constants.hpp"
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

const std::vector<Img>& SpriteLoader::getFrames(const std::string& pieceCode,
                                                  const std::string& state) {
    const std::string key = pieceCode + "/" + state;
    if (cache.find(key) == cache.end())
        cache[key] = load(pieceCode, state);
    return cache[key].frames;
}

const AnimConfig& SpriteLoader::getConfig(const std::string& pieceCode,
                                           const std::string& state) {
    const std::string key = pieceCode + "/" + state;
    if (cache.find(key) == cache.end())
        cache[key] = load(pieceCode, state);
    return cache[key].config;
}

SpriteLoader::Entry SpriteLoader::load(const std::string& pieceCode,
                                        const std::string& state) {
    std::string stateDir = basePath + "/" + pieceCode + "/states/" + state;
    Entry entry;
    entry.config = parseConfig(stateDir + "/config.json");

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

AnimConfig SpriteLoader::parseConfig(const std::string& configPath) {
    std::ifstream f(configPath);
    if (!f) throw std::runtime_error("Cannot open: " + configPath);

    // Minimal JSON parsing - no external library
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());

    AnimConfig cfg;

    auto extractStr = [&](const std::string& key) -> std::string {
        auto pos = content.find("\"" + key + "\"");
        if (pos == std::string::npos) return "";
        pos = content.find(":", pos);
        pos = content.find("\"", pos);
        auto end = content.find("\"", pos + 1);
        return content.substr(pos + 1, end - pos - 1);
    };
    auto extractInt = [&](const std::string& key) -> int {
        auto pos = content.find("\"" + key + "\"");
        if (pos == std::string::npos) return 0;
        pos = content.find(":", pos) + 1;
        while (pos < content.size() && !isdigit(content[pos])) ++pos;
        return std::stoi(content.substr(pos));
    };
    auto extractBool = [&](const std::string& key) -> bool {
        auto pos = content.find("\"" + key + "\"");
        if (pos == std::string::npos) return true;
        pos = content.find(":", pos) + 1;
        while (pos < content.size() && content[pos] == ' ') ++pos;
        return content.substr(pos, 4) == "true";
    };
    auto extractDouble = [&](const std::string& key) -> double {
        auto pos = content.find("\"" + key + "\"");
        if (pos == std::string::npos) return 0;
        
        pos = content.find(":", pos) + 1;
        
        while (pos < content.size() &&
               (content[pos] == ' ' || content[pos] == '\t'))
            ++pos;
        
        size_t end = pos;
        while (end < content.size() &&
               (isdigit(content[end]) || content[end] == '.'))
            ++end;
        
        return std::stod(content.substr(pos, end - pos));
    };

    cfg.fps       = extractInt("frames_per_sec");
    cfg.isLoop    = extractBool("is_loop");
    cfg.nextState = extractStr("next_state_when_finished");
    cfg.speed_m_per_sec = extractDouble("speed_m_per_sec");
    return cfg;
}
