#pragma once
#include "../network/GameSnapshot.hpp"

class Renderer {
public:
    virtual void render(const GameSnapshot& snap) {}
    virtual ~Renderer() = default;
};
