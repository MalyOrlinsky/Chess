#pragma once
#include <vector>

struct Position
{
    int row;
    int col;
};

struct Motion
{
    std::vector<Position> path;

    bool valid = true;

    int currentStep = 0;

    int startTime = 0;
    int stepDuration = 0;

    int order = 0;

    bool canSkip = false;
};


struct Jump
{
    int row;
    int col;

    int startTime;
    int endTime;

    int order;
};