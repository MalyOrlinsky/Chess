#pragma once

enum class RuleResult {
    Ok,
    OutsideBoard,
    EmptySource,
    FriendlyDestination,
    IllegalPieceMove,
    BlockingTool
};
