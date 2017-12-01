//
// Created by Juan Carlos Espinoza Zurita on 29/11/17.
//

#ifndef SLACK_BOT_USER_H
#define SLACK_BOT_USER_H

#endif //SLACK_BOT_USER_H
#pragma once
#include "db.h"

class User{
private:
    std::string code;
    std::string name;
public:
    User(std::string code = "", std::string name = "");
    bool insertUser(Db *db);
    static User *getUser(Db *db, std::string code);
    static int selectUser(Db *db);
    bool findUser(Db *db);
};