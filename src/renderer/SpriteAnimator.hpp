#pragma once
#include "../config/SpriteLoader.hpp"
#include <string>

class SpriteAnimator {
public:
    SpriteAnimator(const std::string& pieceCode, SpriteLoader& loader);

    void tick(int dt);
    void setState(const PieceStatus& newState);
    const PieceStatus& getState() const;
    const Img& currentImg() const;

private:
    std::string pieceCode;
    SpriteLoader& loader;
    PieceStatus state;
    int currentFrame = 0;
    int msAccum = 0;

    void enterState(const PieceStatus& newState);
};
