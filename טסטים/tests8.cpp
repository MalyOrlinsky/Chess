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
    // לבן התחיל ראשון - מנצח
    runTest("enemy collision white started first",
        "Board:\nwR . . bR\nCommands:\nclick 50 50\nclick 350 50\nclick 350 50\nclick 50 50\nwait 3000\nprint board\n",
        ". . . wR\n");

    // שחור התחיל ראשון - מנצח
    runTest("enemy collision black started first",
        "Board:\nwR . . bR\nCommands:\nclick 350 50\nclick 50 50\nclick 50 50\nclick 350 50\nwait 3000\nprint board\n",
        "bR . . .\n");

    // נחיתה על ידידותי - מבוטל
    runTest("friendly landing cancelled",
        "Board:\nwR wK .\nCommands:\nclick 50 50\nclick 150 50\nwait 1000\nprint board\n",
        "wR wK .\n");

    // שני כלים לאותו תא - מי שהתחיל אחרון מנצח
    runTest("two pieces same destination last wins",
        "Board:\nwR . wB\nCommands:\nclick 50 50\nclick 150 50\nclick 250 50\nclick 150 50\nwait 1000\nprint board\n",
        ". wB .\n");

    // שני כלים לאותו תא - הראשון מנצח כשהשני התחיל אחרון
    runTest("two pieces same destination second started last wins",
        "Board:\nwR . wB\nCommands:\nclick 250 50\nclick 150 50\nclick 50 50\nclick 150 50\nwait 1000\nprint board\n",
        ". wR .\n");

    // כלי בתנועה לא ניתן לבחור
    runTest("moving piece cannot be selected",
        "Board:\nwR . .\nCommands:\nclick 50 50\nclick 250 50\nwait 500\nclick 50 50\nwait 2000\nprint board\n",
        ". . wR\n");

    // שני כלים זזים במקביל לתאים שונים
    runTest("two pieces move concurrently to different cells",
        "Board:\nwR . . wK\nCommands:\nclick 50 50\nclick 150 50\nclick 350 50\nclick 250 50\nwait 1000\nprint board\n",
        ". wR wK .\n");

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return 0;
}
