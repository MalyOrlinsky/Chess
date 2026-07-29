#pragma once

template<typename Self>
auto GameManager::findGameImpl(Self& self, int gameId)
{
    auto it = self.games.find(gameId);

    if (it == self.games.end())
        return static_cast<decltype(it->second.get())>(nullptr);

    return it->second.get();
}

template<typename Self>
decltype(auto) GameManager::getGameImpl(Self& self, int gameId)
{
    auto it = self.games.find(gameId);

    if (it == self.games.end())
        throw std::out_of_range("Invalid game id");

    return *it->second;
}