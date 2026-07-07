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
    // print board - לוח רגיל
    runTest("print board basic",
        "Board:\nwK . bK\n. . .\nCommands:\nprint board\n",
        "wK . bK\n. . .\n");

    // click - בחירת כלי
    runTest("click selects piece",
        "Board:\nwK .\n. .\nCommands:\nclick 50 50\nprint board\n",
        "wK .\n. .\n");

    // click - הזזת כלי לתא ריק
    runTest("click moves piece to empty cell",
        "Board:\nwK .\n. .\nCommands:\nclick 50 50\nclick 150 50\nprint board\n",
        ". wK\n. .\n");

    // click - הזזת כלי למטה
    runTest("click moves piece down",
        "Board:\nwK .\n. .\nCommands:\nclick 50 50\nclick 50 150\nprint board\n",
        ". .\nwK .\n");

    // click - החלפת בחירה לכלי ידידותי
    runTest("click switches to friendly piece",
        "Board:\nwK wR\n. .\nCommands:\nclick 50 50\nclick 150 50\nprint board\n",
        "wK wR\n. .\n");

    // click - תא ריק בלי בחירה - מתעלמים
    runTest("click empty cell without selection ignored",
        "Board:\nwK .\n. .\nCommands:\nclick 150 50\nprint board\n",
        "wK .\n. .\n");

    // click - מחוץ ללוח - מתעלמים
    runTest("click outside board ignored",
        "Board:\nwK .\n. .\nCommands:\nclick 50 50\nclick 9999 9999\nprint board\n",
        "wK .\n. .\n");

    // click - לחיצה שלילית מחוץ ללוח
    runTest("click negative coords ignored",
        "Board:\nwK .\n. .\nCommands:\nclick 50 50\nclick -1 -1\nprint board\n",
        "wK .\n. .\n");

    // click - כלי אויב נאכל
    runTest("click captures enemy piece",
        "Board:\nwK bR\n. .\nCommands:\nclick 50 50\nclick 150 50\nprint board\n",
        ". wK\n. .\n");

    // wait - לא משנה את הלוח
    runTest("wait does not change board",
        "Board:\nwK .\n. .\nCommands:\nwait 500\nprint board\n",
        "wK .\n. .\n");

    // wait - כמה פעמים
    runTest("multiple waits",
        "Board:\nwK .\n. .\nCommands:\nwait 100\nwait 200\nwait 300\nprint board\n",
        "wK .\n. .\n");

    // שילוב - click ואז wait ואז print
    runTest("click then wait then print",
        "Board:\nwK .\n. .\nCommands:\nclick 50 50\nclick 150 50\nwait 1000\nprint board\n",
        ". wK\n. .\n");

    // שילוב - כמה הזזות
    runTest("multiple moves",
        "Board:\nwK . .\n. . .\nCommands:\nclick 50 50\nclick 150 50\nclick 150 50\nclick 250 50\nprint board\n",
        ". . wK\n. . .\n");

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return 0;
}
