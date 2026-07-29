#include "Database.hpp"

Database::Database(
    const std::string &host,
    const std::string &port,
    const std::string &db,
    const std::string &user,
    const std::string &password)
{
    std::string connectionString =
        "host=" + host +
        " port=" + port +
        " dbname=" + db +
        " user=" + user +
        " password=" + password;

    connection = std::make_unique<pqxx::connection>(connectionString);
}

Database::~Database()
{
}

bool Database::isConnected()
{
    return connection && connection->is_open();
}

void Database::initialize()
{
    std::ifstream file("server/database/sql/create_tables.sql");

    if (!file.is_open())
    {
        std::cerr << "Cannot open SQL file" << std::endl;
        return;
    }

    std::string sql((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    pqxx::work transaction(*connection);

    transaction.exec(sql);

    transaction.commit();

    std::cout << "Database initialized" << std::endl;
}

bool Database::userExists(const std::string &username)
{
    pqxx::work transaction(*connection);

    pqxx::result result =
        transaction.exec_params(
            "SELECT id FROM users WHERE username = $1",
            username);

    transaction.commit();

    return !result.empty();
}

void Database::createUser(const std::string &username)
{
    pqxx::work transaction(*connection);

    transaction.exec_params(
        "INSERT INTO users(username, password_hash, rating) "
        "VALUES($1, $2, $3)",
        username,
        "",
        1200);

    transaction.commit();
}

int Database::getUserRating(const std::string &username)
{
    pqxx::work transaction(*connection);

    pqxx::result result =
        transaction.exec_params(
            "SELECT rating FROM users WHERE username = $1",
            username);

    transaction.commit();

    if (result.empty())
        return 1200;

    return result[0]["rating"].as<int>();
}

void Database::updateUserRating(const std::string &username, int rating)
{
    pqxx::work transaction(*connection);

    transaction.exec_params(
        "UPDATE users SET rating = $1 WHERE username = $2",
        rating,
        username);

    transaction.commit();
}