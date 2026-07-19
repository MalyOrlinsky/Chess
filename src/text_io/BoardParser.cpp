#include "BoardParser.hpp"
#include "BoardBuilder.hpp"

bool BoardParser::parse(const std::vector<std::string>& lines, Board& board, std::string& error) const {
    std::string input;
    for (const auto& line : lines)
        input += line + "\n";
    return BoardBuilder().build(input, board, error);
}
