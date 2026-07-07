#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "../דפים/board.h"
#include "../דפים/commands.h"

int passed = 0;
int failed = 0;

void runTest(const std::string& testName, const std::string& input, const std::string& expected) {
    std::istringstream ss(input);
    std::string line;
    std::getline(ss, line); // "Board:"

    std::vector<std::string> boardLines;
    while (std::getline(ss, line) && line != "Commands:") {
        boardLines.push_back(line);
    }

    std::vector<std::string> commands;
    while (std::getline(ss, line)) {
        if (!line.empty()) commands.push_back(line);
    }

    std::streambuf* oldBuf = std::cout.rdbuf();
    std::ostringstream out;
    std::cout.rdbuf(out.rdbuf());

    GameState state;
    std::string error;
    if (!parseBoard(boardLines, state.board, error)) {
        std::cout << error << "\n";
    } else {
        runCommands(commands, state);
    }

    std::cout.rdbuf(oldBuf);

    if (out.str() == expected) {
        std::cout << "[PASS] " << testName << "\n";
        passed++;
    } else {
        std::cout << "[FAIL] " << testName << "\n";
        std::cout << "  expected: " << expected;
        std::cout << "  got:      " << out.str();
        failed++;
    }
}

int main() {
    // לבן זז למעלה צעד אחד
    runTest("white pawn moves up one",
        "Board:\n. . .\n. wP .\n. . .\nCommands:\nclick 150 150\nclick 150 50\nprint board\n",
        ". wP .\n. . .\n. . .\n");

    // שחור זז למטה צעד אחד
    runTest("black pawn moves down one",
        "Board:\n. . .\n. bP .\n. . .\nCommands:\nclick 150 150\nclick 150 250\nprint board\n",
        ". . .\n. . .\n. bP .\n");

    // לבן לא יכול לזוז שתיים
    runTest("white pawn cannot move two",
        "Board:\n. . .\n. . .\n. wP .\n. . .\nCommands:\nclick 150 250\nclick 150 50\nprint board\n",
        ". . .\n. . .\n. wP .\n. . .\n");

    // שחור לא יכול לזוז שתיים
    runTest("black pawn cannot move two",
        "Board:\n. . .\n. bP .\n. . .\n. . .\nCommands:\nclick 150 150\nclick 150 350\nprint board\n",
        ". . .\n. bP .\n. . .\n. . .\n");

    // לבן אוכל באלכסון שמאל
    runTest("white pawn captures diagonally left",
        "Board:\nbR . .\n. wP .\n. . .\nCommands:\nclick 150 150\nclick 50 50\nprint board\n",
        "wP . .\n. . .\n. . .\n");

    // לבן אוכל באלכסון ימין
    runTest("white pawn captures diagonally right",
        "Board:\n. . bR\n. wP .\n. . .\nCommands:\nclick 150 150\nclick 250 50\nprint board\n",
        ". . wP\n. . .\n. . .\n");

    // שחור אוכל באלכסון
    runTest("black pawn captures diagonally",
        "Board:\n. . .\n. bP .\n. wR .\nCommands:\nclick 150 150\nclick 250 250\nprint board\n",
        ". . .\n. . .\n. . bP\n");

    // לבן לא יכול לאכול קדימה
    runTest("white pawn cannot capture forward",
        "Board:\n. bR .\n. wP .\n. . .\nCommands:\nclick 150 150\nclick 150 50\nprint board\n",
        ". bR .\n. wP .\n. . .\n");

    // שחור לא יכול לאכול קדימה
    runTest("black pawn cannot capture forward",
        "Board:\n. . .\n. bP .\n. wR .\nCommands:\nclick 150 150\nclick 150 250\nprint board\n",
        ". . .\n. bP .\n. wR .\n");

    // לבן לא יכול לזוז למטה
    runTest("white pawn cannot move down",
        "Board:\n. . .\n. wP .\n. . .\nCommands:\nclick 150 150\nclick 150 250\nprint board\n",
        ". . .\n. wP .\n. . .\n");

    // שחור לא יכול לזוז למעלה
    runTest("black pawn cannot move up",
        "Board:\n. . .\n. bP .\n. . .\nCommands:\nclick 150 150\nclick 150 50\nprint board\n",
        ". . .\n. bP .\n. . .\n");

    // לבן לא יכול לאכול ידידותי באלכסון
    runTest("white pawn cannot capture friendly diagonally",
        "Board:\nwR . .\n. wP .\n. . .\nCommands:\nclick 150 150\nclick 50 50\nprint board\n",
        "wR . .\n. wP .\n. . .\n");

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return 0;
}
