#include "GameEngine.hpp"
#include "../text_io/BoardPrinter.hpp"
#include <cmath>
#include <sstream>

static CommandType parseCommandType(const std::string& cmd) {
    if (cmd == "print board")        return CommandType::PrintBoard;
    if (cmd.substr(0, 5) == "click") return CommandType::Click;
    if (cmd.substr(0, 4) == "jump")  return CommandType::Jump;
    if (cmd.substr(0, 4) == "wait")  return CommandType::Wait;
    return CommandType::Unknown;
}

static std::pair<int,int> parseXY(const std::string& s) {
    int x, y;
    std::istringstream(s) >> x >> y;
    return {x, y};
}

GameEngine::GameEngine()
    : spriteLoader("assets/pieces2"),
      arbiter("assets/pieces2")
{
    arbiter.setKingCapturedCallback([this](Color w) {
        gameOver = true;
        winner = (w == Color::Black) ? "white" : "black";
        arbiter = RealTimeArbiter("assets/pieces2");
    });
}

void GameEngine::handleClick(CellPos pos) {
    if (gameOver) return;
    PieceStatus status = pos.valid ? arbiter.getStatus(pos.row, pos.col) : PieceStatus::Idle;
    ClickResult result = controller.onClick(pos, board, status);

    if (result.action == ClickAction::MoveRequest)
        requestMove(result.from, result.to);
    else if (result.action == ClickAction::JumpRequest)
        requestJump(result.from);
    else return;
}

void GameEngine::requestMove(CellPos from, CellPos to) {
    if (gameOver) return;

    RuleResult rule = ruleEngine.validate(from.row, from.col, to.row, to.col, board);
    if (rule != RuleResult::Ok) return;

    Piece* piece = board.getPiece(from.row, from.col);
    if (!piece) return;

    std::string code = piece->toString();
    const AnimConfig& cfg = spriteLoader.configManager.getConfig(code, PieceStatus::Move);
    double speed = cfg.speed_m_per_sec;

    if (speed <= 0)
        return;

    int dist = std::max(std::abs(to.row - from.row), std::abs(to.col - from.col));
    int duration = static_cast<int>((dist * speed) * 1000);

    recordAction(piece, "MOVE " + cellToNotation(from.row, from.col) + " " + cellToNotation(to.row, to.col));

    arbiter.startMotion(from.row, from.col, to.row, to.col, duration, board);
}

void GameEngine::requestJump(CellPos pos) {
    if (gameOver) return;

    if (!pos.valid || board.getPiece(pos.row, pos.col) == nullptr)
        return;

    if (arbiter.getStatus(pos.row, pos.col) != PieceStatus::Idle)
        return;

    Piece* piece = board.getPiece(pos.row, pos.col);
    std::string code = piece->toString();
    const AnimConfig& cfg =  spriteLoader.configManager.getConfig(code, PieceStatus::Jump);
    double speed = cfg.speed_m_per_sec;

    if (speed <= 0)
        return;

    recordAction(piece,"JUMP " +cellToNotation(pos.row, pos.col));

    arbiter.startJump(pos.row, pos.col, speed * 1000);
}

void GameEngine::recordAction(Piece* piece, const std::string& action)
{
    if (piece->color == Color::Black)
        movesLogBlack.push_back(action);
    else
        movesLogWhite.push_back(action);
}

void GameEngine::handleWait(int ms) {
    if (gameOver) return;

    std::pair<int, int> scoreTemp = arbiter.advanceClock(ms, board);

    score.first += scoreTemp.first;
    score.second += scoreTemp.second;
}

void GameEngine::execute(const std::string& cmd) {
    switch (parseCommandType(cmd)) {
        case CommandType::PrintBoard:
            handleWait(0);
            BoardPrinter().print(board);
            break;
        case CommandType::Click: {
            if (gameOver) break;
            auto [x, y] = parseXY(cmd.substr(6));
            handleClick(mapper.toCell(x, y, board.rows, board.cols));
            break;
        }
        case CommandType::Jump: {
            if (gameOver) break;
            auto [x, y] = parseXY(cmd.substr(5));
            requestJump(mapper.toCell(x, y, board.rows, board.cols));
            break;
        }
        case CommandType::Wait: {
            if (gameOver) break;
            int ms;
            std::istringstream(cmd.substr(5)) >> ms;
            handleWait(ms);
            break;
        }
        case CommandType::Unknown:
            break;
    }
}

int GameEngine::clock() const { return arbiter.clock(); }

GameSnapshot GameEngine::snapshot() const {
    GameSnapshot snap;

    snap.rows = board.rows;
    snap.cols = board.cols;

    auto sel = controller.getSelected();
    snap.selectedRow = sel.first;
    snap.selectedCol = sel.second;

    snap.gameOver = gameOver;
    snap.winner = winner;

    snap.scoreWhite = score.second;
    snap.scoreBlack = score.first;

    snap.playerBlack = "black";
    snap.playerWhite = "white";

    snap.movesLogBlack = movesLogBlack;
    snap.movesLogWhite = movesLogWhite;

    snap.cells.resize(board.rows, std::vector<CellSnapshot>(board.cols));

    for (int r = 0; r < board.rows; r++)
    {
        for (int c = 0; c < board.cols; c++)
        {
            Piece* p = board.getPiece(r, c);
            if (!p)
                continue;

            snap.cells[r][c] = {
                p->id,
                p->type,
                p->color,
                arbiter.getStatus(r, c)
            };
        }
    }

    return snap;
}

std::string GameEngine::cellToNotation(int row, int col)
{
    char file = 'a' + col;
    int rank = row + 1;

    return std::string(1, file) + std::to_string(rank);
}