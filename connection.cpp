//
// Created by Juan Carlos Espinoza Zurita on 14/11/17.
//

#include "connection.h"


using namespace pplx;
using namespace web;
using namespace http;
using namespace web::websockets::client;

Connection *Connection::Instance = 0;

Connection::Connection(std::string url, std::string token, std::string name, bool batch_presence_aware)
        : raw_client("https://slack.com/api/"),
        signed_in(false){
    this->url = url;
    this->token = token;
    this->batch_presence_aware = batch_presence_aware;
    this->name = name;
}

Connection* Connection::getInstance(std::string token, std::string name, bool batch_presence_aware) {
    if (Instance == 0)
        Instance = new Connection("https://slack.com/api/rtm.connect", token, name,batch_presence_aware);
    return Instance;
}

int Connection::selectConnection(Db *db) {
    int rows;
    std::string sql;
    sql = "SELECT * FROM connection";
    db->setSql(const_cast<char *>(sql.c_str()));
    rows = db->dbCount();
    return rows;
}
void Connection::printConnections(Db *db) {
    std::vector<std::vector<std::string>> values;
    std::vector<std::string> row;
    std::string sql;
    sql = "SELECT id, token, name, batch_presence_aware FROM connection";
    db->setSql(const_cast<char *>(sql.c_str()));
    values = db->dbSelect();
    for (int i; i < values.size(); i++){
        std::cout << values[i][0] << "|" << values[i][1] << "|" << values[i][2] << "|" << values[i][3] << std::endl;
    }
}
bool Connection::insertConnection(Db *db) {
    bool status;
    std::string sql;
    int code;
    code = selectConnection(db) + 1;
    sql = "INSERT INTO connection (id, name, url, token, batch_presence_aware) "
                  "VALUES (" + std::to_string(code) + ",'" +  name + "','" + url +
          "','" + token + "'," + (batch_presence_aware ? "1":"0") + ");";
    db->setSql(const_cast<char *>(sql.c_str()));
    if (db->dbInsert()) {
        std::cout << "Row inserted successfully\n";
        status = true;
    }
    else {
        std::cout << "Error inserting row\n";
        status = false;
    }
    return status;
}
Connection* Connection::getConnection(Db *db, int id) {
    std::vector<std::vector<std::string>> values;
    std::vector<std::string> row;
    std::string sql;
    Connection *connection;
    sql = "SELECT id, token, name, batch_presence_aware FROM connection "
            "WHERE id = " + std::to_string(id);
    db->setSql(const_cast<char *>(sql.c_str()));
    if (db->dbCount() > 0){
        values = db->dbSelect();
        row = values[0];
        connection = getInstance(row[1], row[2], std::stoi(row[3]));
    }
    return connection;
}
std::string Connection::getUrl() {
    return url + "?token=" + token + "&batch_presence_aware=" + (batch_presence_aware ? "true":"false");
}
pplx::task<web::json::value> Connection::login() {
    http_client client(getUrl());
    http_request requester(methods::POST);

    return client.request(requester).then([](http_response response)
      {
          if (response.status_code() == status_codes::OK)
          {
              auto body = response.extract_json();
              return body;
          }
          else {
              /* Print bad status code */
              std::cout << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
          }

      });
}
pplx::task<std::string> Connection::post(std::string wss) {
    websocket_client client;
    client.connect(U(wss)).then([]{}).wait();
    client.receive().then([](websocket_incoming_message msg){
        return msg.extract_string();
    }).then([](std::string body){
        std::cout << body << std::endl;
    }).wait();
}
