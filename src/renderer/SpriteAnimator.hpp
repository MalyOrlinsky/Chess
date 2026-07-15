#pragma once
#include "SpriteLoader.hpp"
#include <string>

class SpriteAnimator {
public:
    SpriteAnimator(const std::string& pieceCode, SpriteLoader& loader);

    void tick(int dt);                          // advance animation by dt ms
    void setState(const PieceStatus& newState); // called by renderer from snapshot
    const PieceStatus& getState() const;
    const Img& currentImg() const;

private:
    std::string   pieceCode;
    SpriteLoader& loader;
    PieceStatus   state;
    int currentFrame = 0;
    int msAccum = 0;

    void enterState(const PieceStatus& newState);
};
