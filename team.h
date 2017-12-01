//
// Created by Juan Carlos Espinoza Zurita on 30/11/17.
//

#ifndef SLACK_BOT_TEAM_H
#define SLACK_BOT_TEAM_H

#endif //SLACK_BOT_TEAM_H

#pragma once
#include "db.h"

class Team{
private:
    std::string code;
    std::string name;
    std::string domain;
    static Team* Instance;
    Team(std::string code = "", std::string name = "", std::string domain = "");
public:
    static Team *getInstance(std::string code = "", std::string name = "", std::string domain = "");
    bool insertTeam(Db *db);
    static Team *getTeam(Db *db, std::string code);
    static int selectTeam(Db *db);
    bool findTeam(Db *db);
    std::string getCode();
};