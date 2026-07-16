#include "SpriteAnimator.hpp"

SpriteAnimator::SpriteAnimator(const std::string& pieceCode, SpriteLoader& loader)
    : pieceCode(pieceCode), loader(loader) {
    if (state != PieceStatus::Idle)
        enterState(PieceStatus::Idle);
}

void SpriteAnimator::tick(int dt) {
    const AnimConfig& cfg = loader.configManager.getConfig(pieceCode, state);
    msAccum += dt;
    int msPerFrame = (cfg.fps > 0) ? (1000 / cfg.fps) : 1000;

    while (msAccum >= msPerFrame) {
        msAccum -= msPerFrame;
        currentFrame++;
        int frameCount = (int)loader.getFrames(pieceCode, state).size();
        if (currentFrame >= frameCount) {
            if (cfg.isLoop) {
                currentFrame = 0;
            } else {
                currentFrame = frameCount - 1;
                if (state != cfg.nextState)
                    enterState(cfg.nextState);
                return;
            }
        }
    }
}

void SpriteAnimator::setState(const PieceStatus& newState) {
    if(newState != state)
        enterState(newState);
}

const PieceStatus& SpriteAnimator::getState() const { return state; }

const Img& SpriteAnimator::currentImg() const {
    return loader.getFrames(pieceCode, state)[currentFrame];
}

void SpriteAnimator::enterState(const PieceStatus& newState) {
    state = newState;
    currentFrame = 0;
    msAccum = 0;
}
