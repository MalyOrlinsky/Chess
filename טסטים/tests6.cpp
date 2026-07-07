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
    // לפני הגעה - כלי עדיין במקום המקורי
    runTest("piece not moved before arrival",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 150 50\nwait 500\nprint board\n",
        "wR . .\n");

    // אחרי הגעה - כלי במקום החדש
    runTest("piece moved after arrival",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 150 50\nwait 1000\nprint board\n",
        ". wR .\n");

    // מהלך 2 תאים - לפני ואחרי
    runTest("two cell move before and after arrival",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 250 50\nwait 1000\nprint board\nwait 1000\nprint board\n",
        "wR . .\n. . wR\n");

    // מהלך 2 תאים - בדיוק בזמן
    runTest("two cell move exactly at arrival",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 250 50\nwait 2000\nprint board\n",
        ". . wR\n");

    // כמה waits לפני הגעה
    runTest("multiple waits before arrival",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 150 50\nwait 300\nwait 300\nprint board\n",
        "wR . .\n");

    // כמה waits שמצטברים לאחרי הגעה
    runTest("multiple waits reach arrival",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 150 50\nwait 500\nwait 500\nprint board\n",
        ". wR .\n");

    // print לפני ואחרי באותו מהלך
    runTest("print before and after arrival",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 150 50\nwait 500\nprint board\nwait 500\nprint board\n",
        "wR . .\n. wR .\n");

    // מהלך אלכסוני - מרחק 2, זמן הגעה 2000ms
    runTest("diagonal move arrival time",
        "Board:\nwB . .\n. . .\n. . .\nCommands:\nclick 50 50\nclick 250 250\nwait 1000\nprint board\nwait 1000\nprint board\n",
        "wB . .\n. . .\n. . .\n. . .\n. . .\n. . wB\n");

    // כלי בתנועה - לא ניתן להפנות מחדש
    runTest("moving piece ignores redirect",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 250 50\nwait 1000\nclick 50 50\nclick 150 50\nwait 1000\nprint board\n",
        ". . wR\n");

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return 0;
}
