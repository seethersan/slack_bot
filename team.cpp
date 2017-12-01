//
// Created by Juan Carlos Espinoza Zurita on 30/11/17.
//

#include "team.h"

Team *Team::Instance = 0;

Team::Team(std::string code, std::string name, std::string domain) {
    this->code = code;
    this->name = name;
    this->domain = domain;
}
Team* Team::getInstance(std::string code, std::string name, std::string domain) {
    if (Instance == 0)
        Instance = new Team(code, name, domain);
    return Instance;
}
int Team::selectTeam(Db *db) {
    int rows;
    std::string sql;
    sql = "SELECT * FROM Team";
    db->setSql(const_cast<char *>(sql.c_str()));
    rows = db->dbCount();
    return rows;
}
bool Team::findTeam(Db *db) {
    bool found = false;
    std::string sql = "SELECT id, code, name, domain FROM team WHERE code = '" + code + "'";
    db->setSql(const_cast<char *>(sql.c_str()));
    if (db->dbCount() > 0)
        found = true;
    return found;
}
bool Team::insertTeam(Db *db) {
    bool status = false;
    if (findTeam(db)){
        status = false;
    }
    else{
        std::string sql;
        int id;
        id = selectTeam(db) + 1;
        sql = "INSERT INTO team (id, code, name, domain) "
                      "VALUES (" + std::to_string(id) + ", '" + code + "', '" + name + "', '" + domain + "');";
        db->setSql(const_cast<char *>(sql.c_str()));
        if (db->dbInsert()) {
            std::cout << "Row inserted successfully\n";
            status = true;
        }
        else {
            std::cout << "Error inserting row\n";
            status = false;
        }
    }
    return status;
}
Team* Team::getTeam(Db *db, std::string code) {
    std::vector<std::vector<std::string>> values;
    std::vector<std::string> row;
    std::string sql;
    Team *team;
    sql = "SELECT id, code, name, domain FROM team "
                  "WHERE code = " + code;
    db->setSql(const_cast<char *>(sql.c_str()));
    if (db->dbCount() > 0){
        values = db->dbSelect();
        row = values[0];
        team = getInstance(row[1], row[2]);
    }
    return team;
}
std::string Team::getCode() {
    return code;
}