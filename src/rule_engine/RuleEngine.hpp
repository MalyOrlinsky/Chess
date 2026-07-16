#pragma once
#include "RuleResult.hpp"
#include "../model/Board.hpp"
#include "../movement/MoveRule.hpp"
#include <memory>

class RuleEngine {
public:
    RuleResult validate(int fromRow, int fromCol, int toRow, int toCol,
                        const Board& board) const;
private:
    bool inBounds(int row, int col, const Board& board) const;
    std::unique_ptr<MoveRule> ruleFor(char type) const;
};
