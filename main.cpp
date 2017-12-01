#include <cpprest/http_client.h>

#include "connection.h"
#include "team.h"
#include "user.h"

int main(int argc, char* argv[])
{
    Connection *connection;
    Db *db = Db::getInstance();
    int id;
    db->setSql("SELECT * FROM connection");
    if (db->dbCount() > 0){
        connection->printConnections(db);
        std::cout << "Select a connection:\n";
        std::cin >> id;
        connection = Connection::getConnection(db, id);
    }
    else{
        std::string token, name;
        bool batch_presence_aware;
        std::cout << "Connection is missing. Please configure.\n";
        std::cout << "Enter Token:\n";
        std::cin >> token;
        std::cout << "Enter the Connection name\n";
        std::cin >> name;
        std::cout << "Enter batch_presence_aware\n";
        std::cin >> batch_presence_aware;
        connection = Connection::getInstance(token, name, batch_presence_aware);
        connection->insertConnection(db);
    }
    connection->login().then([&db, &connection](web::json::value respond){
        std::string conn = respond.at(U("url")).as_string();
        web::json::value t = respond.at(U("team"));
        Team *team = Team::getInstance(t.at(U("id")).as_string(), t.at(U("name")).as_string(),
                                       t.at(U("domain")).as_string());
        if (team->insertTeam(db))
            std::cout << "Team created successfully\n";
        else
            std::cout << "Team already created\n";
        web::json::value u = respond.at(U("self"));
        User user = User(u.at(U("id")).as_string(), u.at(U("name")).as_string());
        if (user.insertUser(db))
            std::cout << "User created successfully\n";
        else
            std::cout << "User already created\n";
        std::cout << conn << std::endl;
        connection->post(conn).wait();
    }).wait();
    return 0;
}