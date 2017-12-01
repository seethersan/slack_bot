//
// Created by Juan Carlos Espinoza Zurita on 14/11/17.
//

#ifndef SLACK_BOT_CONNECTION_H
#define SLACK_BOT_CONNECTION_H

#endif //SLACK_BOT_CONNECTION_H
#pragma once
#include <iostream>
#include <cpprest/json.h>
#include <cpprest/ws_client.h>
#include <cpprest/http_client.h>
#include "db.h"

using namespace web::http::client;
using namespace web::json;

class Connection{
private:
    std::string url;
    std::string token;
    std::string name;
    bool batch_presence_aware;
    bool signed_in;
    web::http::client::http_client raw_client;
    Connection(std::string url = "",
               std::string token = "",
               std::string name = "",
               bool batch_presence_aware = false);
    static Connection* Instance;
public:
    static Connection *getInstance(std::string token = "", std::string name = "", bool batch_presence_aware = "");
    pplx::task<web::json::value> login();
    pplx::task<std::string> post(std::string wss);
    pplx::task<web::json::value> get();
    bool insertConnection(Db *db);
    static Connection* getConnection(Db *db, int id);
    static int selectConnection(Db *db);
    std::string getUrl();
    void printConnections(Db *db);
};
