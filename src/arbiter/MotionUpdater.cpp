#include "MotionUpdater.hpp"
#include <iostream>

std::vector<Motion> MotionUpdater::update(std::vector<Motion>& motions, const std::vector<Jump>& jumps,
     Board& board, int currentClock, Color& color) {
    CollisionResolver resolver;
    MotionAdvancer advancer;

    // std::vector<Motion> active;
    std::vector<Motion> finished;

    for (auto& motion : motions)
    {
        if (!motion.valid)
            continue;

        if (!shouldAdvance(motion, currentClock))
            continue;

        CollisionResult result = resolver.resolve(motion, jumps, board);
        if (result == CollisionResult::Stop)
        {
            finished.push_back(motion);
            motion.valid = false;
            continue;
        }

        if (result == CollisionResult::Died)
        {
            advancer.died(motion, board, color);
            motion.valid = false;
            continue;
        }

        advancer.advance(motion, motions, jumps, board, currentClock, color);
        if (result == CollisionResult::Finish || motion.currentStep >= static_cast<int>(motion.path.size()) - 1)
        {
            finished.push_back(motion);
            motion.valid = false;
            continue;
        }

    }

    return finished;
}

bool MotionUpdater::shouldAdvance(const Motion& motion, int currentClock) const {
    int elapsed = currentClock - motion.startTime;
    int expectedStep = elapsed / motion.stepDuration;

    return expectedStep > motion.currentStep && expectedStep % 1 == 0;
}