#pragma once

#include <fstream>
#include <iostream>

#include <pqxx/pqxx>
#include <string>

class Database
{
public:
    Database(
        const std::string &host,
        const std::string &port,
        const std::string &db,
        const std::string &user,
        const std::string &password);

    ~Database();

    bool isConnected();
    void initialize();

    bool userExists(const std::string &username);
    void createUser(const std::string &username);
    int getUserRating(const std::string &username);

private:
    std::unique_ptr<pqxx::connection> connection;
};