//
// Created by Juan Carlos Espinoza Zurita on 16/11/17.
//

#ifndef SLACK_BOT_DB_H
#define SLACK_BOT_DB_H

#endif //SLACK_BOT_DB_H
#pragma once
#include <iostream>
#include <sqlite3.h>
#include <fstream>
#include <vector>

class Db{
private:
    char* name;
    sqlite3 *db;
    char* sql;
    Db(char* name = "slack.db");
    static Db* Instance;
    ~Db();
public:
    static Db *getInstance(char* name = "slack.db");
    sqlite3 * getDb();
    char *getName();
    void setSql(char* sql);
    char* getSql();
    int dbCreate();
    int dbInsert();
    int dbCount();
    std::vector<std::vector<std::string>> dbSelect();
};

