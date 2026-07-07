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
    // צריח חסום - לא יכול לעבור
    runTest("rook blocked by piece",
        "Board:\nwR wP .\nCommands:\nclick 50 50\nclick 250 50\nprint board\n",
        "wR wP .\n");

    // צריח לא חסום - יכול לעבור
    runTest("rook not blocked",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 250 50\nprint board\n",
        ". . wR\n");

    // רץ חסום - לא יכול לעבור
    runTest("bishop blocked by piece",
        "Board:\nwB . .\n. wP .\n. . .\nCommands:\nclick 50 50\nclick 250 250\nprint board\n",
        "wB . .\n. wP .\n. . .\n");

    // רץ לא חסום - יכול לעבור
    runTest("bishop not blocked",
        "Board:\nwB . .\n. . .\n. . .\nCommands:\nclick 50 50\nclick 250 250\nprint board\n",
        ". . .\n. . .\n. . wB\n");

    // פרש קופץ מעל כלים
    runTest("knight jumps over pieces",
        "Board:\nwN wP .\nwP . .\n. . .\nCommands:\nclick 50 50\nclick 150 250\nprint board\n",
        ". wP .\nwP . .\n. wN .\n");

    // אי אכילת ידידותי - צריח
    runTest("rook cannot capture friendly",
        "Board:\nwR wR .\nCommands:\nclick 50 50\nclick 150 50\nprint board\n",
        "wR wR .\n");

    // אי אכילת ידידותי - רץ
    runTest("bishop cannot capture friendly",
        "Board:\nwB . .\n. wP .\n. . .\nCommands:\nclick 50 50\nclick 150 150\nprint board\n",
        "wB . .\n. wP .\n. . .\n");

    // אכילת אויב - צריח
    runTest("rook captures enemy",
        "Board:\nwR bR .\nCommands:\nclick 50 50\nclick 150 50\nprint board\n",
        ". wR .\n");

    // אכילת אויב - רץ
    runTest("bishop captures enemy",
        "Board:\nwB . .\n. bP .\n. . .\nCommands:\nclick 50 50\nclick 150 150\nprint board\n",
        ". . .\n. wB .\n. . .\n");

    // מלכה חסומה
    runTest("queen blocked by piece",
        "Board:\nwQ wP .\nCommands:\nclick 50 50\nclick 250 50\nprint board\n",
        "wQ wP .\n");

    // מלכה אוכלת אויב
    runTest("queen captures enemy",
        "Board:\nwQ . bR\nCommands:\nclick 50 50\nclick 250 50\nprint board\n",
        ". . wQ\n");

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return 0;
}
