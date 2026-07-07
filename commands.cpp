#include "commands.h"
#include <iostream>

void runCommands(const std::vector<std::string>& commands, const Board& board) {
    for (const std::string& cmd : commands) {
        if (cmd == "print board") {
            for (const auto& row : board.grid) {
                for (int i = 0; i < (int)row.size(); i++) {
                    if (i > 0) std::cout << " ";
                    std::cout << row[i];
                }
                std::cout << "\n";
            }
        }
    }
}
