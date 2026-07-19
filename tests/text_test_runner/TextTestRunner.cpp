#include "TextTestRunner.hpp"
#include "../../src/game_engine/GameEngine.hpp"
#include "../../src/text_io/BoardParser.hpp"
#include <string>
#include <vector>
#include <iostream>

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(' ');
    return start == std::string::npos ? "" : s.substr(start);
}

void TextTestRunner::run(std::istream& in) {
    std::vector<std::string> boardLines, commands;
    std::string line;
    while (std::getline(in, line) && trim(line) != "Board:") {}
    while (std::getline(in, line) && trim(line) != "Commands:")
        boardLines.push_back(line);
    while (std::getline(in, line)) {
        std::string t = trim(line);
        if (!t.empty()) commands.push_back(t);
    }

    GameEngine engine;
    engine.loadBoard("board.txt");
    std::string error;
    if (!BoardParser().parse(boardLines, engine.getBoard(), error)) {
        std::cout << error << "\n";
        return;
    }

    for (const std::string& cmd : commands)
        engine.execute(cmd);
}
