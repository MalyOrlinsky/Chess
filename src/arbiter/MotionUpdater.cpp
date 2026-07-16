#include "MotionUpdater.hpp"

std::vector<Motion> MotionUpdater::update(std::vector<Motion>& motions, const std::vector<Jump>& jumps, Board& board, int currentClock) {
    CollisionResolver resolver;
    MotionAdvancer advancer;

    std::vector<Motion> active;
    std::vector<Motion> finished;

    for (auto& motion : motions)
    {
        if (!shouldAdvance(motion, currentClock))
        {
            active.push_back(motion);
            continue;
        }

        CollisionResult result = resolver.resolve(motion, board);
        if (result == CollisionResult::Stop)
        {
            finished.push_back(motion);
            continue;
        }

        advancer.advance(motion, motions, jumps, board, currentClock);
        if (result == CollisionResult::Finish || motion.currentStep >= static_cast<int>(motion.path.size()) - 1)
        {
            finished.push_back(motion);
            continue;
        }

        active.push_back(motion);
    }

    motions = std::move(active);
    return finished;
}

bool MotionUpdater::shouldAdvance(const Motion& motion, int currentClock) const {
    int elapsed = currentClock - motion.startTime;
    int expectedStep = elapsed / motion.stepDuration;

    return expectedStep > motion.currentStep && expectedStep % 1 == 0;
}