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
    // תקין - לוח 3x4
    runTest("valid 3x4 board",
        "Board:\nwK . . bK\n. . . .\nwR . . bR\nCommands:\nprint board\n",
        "wK . . bK\n. . . .\nwR . . bR\n");

    // תקין - לוח 3x3 עם כל סוגי כלים
    runTest("valid 3x3 all pieces",
        "Board:\nwK . bQ\n. wN .\nbP . wR\nCommands:\nprint board\n",
        "wK . bQ\n. wN .\nbP . wR\n");

    // תקין - לוח 1x1
    runTest("valid 1x1 board",
        "Board:\n.\nCommands:\nprint board\n",
        ".\n");

    // תקין - לוח 8x8
    runTest("valid 8x8 board",
        "Board:\nwR wN wB wQ wK wB wN wR\nwP wP wP wP wP wP wP wP\n. . . . . . . .\n. . . . . . . .\n. . . . . . . .\n. . . . . . . .\nbP bP bP bP bP bP bP bP\nbR bN bB bQ bK bB bN bR\nCommands:\nprint board\n",
        "wR wN wB wQ wK wB wN wR\nwP wP wP wP wP wP wP wP\n. . . . . . . .\n. . . . . . . .\n. . . . . . . .\n. . . . . . . .\nbP bP bP bP bP bP bP bP\nbR bN bB bQ bK bB bN bR\n");

    // שגוי - טוקן לא חוקי
    runTest("unknown token xZ",
        "Board:\nwK xZ\n. .\nCommands:\n",
        "ERROR UNKNOWN_TOKEN\n");

    // שגוי - אות גדולה במקום קטנה
    runTest("unknown token WK uppercase",
        "Board:\nWK .\n. .\nCommands:\n",
        "ERROR UNKNOWN_TOKEN\n");

    // שגוי - צבע לא חוקי
    runTest("unknown token xK invalid color",
        "Board:\nxK .\n. .\nCommands:\n",
        "ERROR UNKNOWN_TOKEN\n");

    // שגוי - שורות בגדלים שונים
    runTest("row width mismatch",
        "Board:\nwK . .\n. bK\nCommands:\n",
        "ERROR ROW_WIDTH_MISMATCH\n");

    // click - בחירת כלי
    runTest("click selects piece",
        "Board:\nwK .\n. .\nCommands:\nclick 50 50\nprint board\n",
        "wK .\n. .\n");

    // click - הזזת כלי
    runTest("click moves piece",
        "Board:\nwK .\n. .\nCommands:\nclick 50 50\nclick 150 50\nprint board\n",
        ". wK\n. .\n");

    // click - החלפת בחירה לכלי ידידותי
    runTest("click switches selection to friendly piece",
        "Board:\nwK wR\n. .\nCommands:\nclick 50 50\nclick 150 50\nprint board\n",
        "wK wR\n. .\n");

    // click - מחוץ ללוח מתעלמים
    runTest("click outside board ignored",
        "Board:\nwK .\n. .\nCommands:\nclick 50 50\nclick 9999 9999\nprint board\n",
        "wK .\n. .\n");

    // click - תא ריק בלי בחירה מתעלמים
    runTest("click empty cell no selection ignored",
        "Board:\nwK .\n. .\nCommands:\nclick 150 50\nprint board\n",
        "wK .\n. .\n");

    // wait - מקדם את השעון
    runTest("wait advances clock",
        "Board:\nwK .\n. .\nCommands:\nwait 1000\nprint board\n",
        "wK .\n. .\n");

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return 0;
}
