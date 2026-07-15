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
    : spriteLoader("assets/pieces2")
{
        arbiter.setKingCapturedCallback([this](Color w) {
        gameOver = true;
        winner = (w == Color::White) ? "white" : "black";
        arbiter = RealTimeArbiter();
    });
}

void GameEngine::handleClick(CellPos pos) {
    if (gameOver) return;
    bool moving  = pos.valid && arbiter.isMoving(pos.row, pos.col);
    bool jumping = pos.valid && arbiter.isJumping(pos.row, pos.col);
    ClickResult result = controller.onClick(pos, board, moving, jumping);

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

    std::string code = pieceCode(*board.getPiece(from.row, from.col));
    
    const AnimConfig& cfg = spriteLoader.getConfig(code, "move");

    double speed = cfg.speed_m_per_sec;

    if (speed <= 0)
        return;

    int dist = std::max(std::abs(to.row - from.row), std::abs(to.col - from.col));

    int duration = static_cast<int>((dist * speed) * 1000);

    arbiter.startMotion( from.row, from.col, to.row, to.col, duration);
}

void GameEngine::requestJump(CellPos pos) {
    if (gameOver) return;
    if (!pos.valid || board.getPiece(pos.row, pos.col) == nullptr) return;
    if (arbiter.isMoving(pos.row, pos.col)) return;
    if (arbiter.isJumping(pos.row, pos.col)) return;
    
    std::string code = pieceCode(*board.getPiece(pos.row, pos.col));

    const AnimConfig& cfg = spriteLoader.getConfig(code, "jump");

    double speed = cfg.speed_m_per_sec;

    if (speed <= 0)
        return;

    arbiter.startJump(pos.row, pos.col, speed);
}

void GameEngine::handleWait(int ms) {
    if (gameOver) return;
    arbiter.advanceClock(ms, board);
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
    snap.rows     = board.rows;
    snap.cols     = board.cols;
    snap.gameOver = gameOver;
    snap.winner   = winner;
    snap.cells.resize(board.rows, std::vector<CellSnapshot>(board.cols));
    for (int r = 0; r < board.rows; r++)
        for (int c = 0; c < board.cols; c++) {
            Piece* p = board.getPiece(r, c);
            if (!p) continue;
            snap.cells[r][c] = {
                p->id,
                p->type,
                p->color,
                arbiter.isMoving(r,c),
                arbiter.isJumping(r,c)
            };
        }
    return snap;
}

std::string GameEngine::pieceCode(const Piece& p) {
    std::string code;
    code += p.type;
    code += (p.color == Color::White) ? 'W' : 'B';
    return code;
}
