//
// Created by Juan Carlos Espinoza Zurita on 29/11/17.
//

#include "user.h"

User::User(std::string code, std::string name) {
    this->code = code;
    this->name = name;
}
int User::selectUser(Db *db) {
    int rows;
    std::string sql;
    sql = "SELECT * FROM user";
    db->setSql(const_cast<char *>(sql.c_str()));
    rows = db->dbCount();
    return rows;
}
bool User::findUser(Db *db) {
    bool found = false;
    std::string sql;
    sql = "SELECT id, code, name FROM user "
            "WHERE code = '" + code + "'";
    db->setSql(const_cast<char *>(sql.c_str()));
    if (db->dbCount() > 0)
        found = true;
    return found;
}
bool User::insertUser(Db *db) {
    bool status = false;
    if (findUser(db)){
        status = false;
    }
    else{
        std::string sql;
        int id;
        id = selectUser(db) + 1;
        sql = "INSERT INTO user (id, code, name) "
                "VALUES (" + std::to_string(id) + ", '" + code + "', '" + name + "');";
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
User* User::getUser(Db *db, std::string code) {
    std::vector<std::vector<std::string>> values;
    std::vector<std::string> row;
    std::string sql;
    User *user;
    sql = "SELECT id, code, name FROM user "
                  "WHERE code = " + code;
    db->setSql(const_cast<char *>(sql.c_str()));
    if (db->dbCount() > 0){
        values = db->dbSelect();
        row = values[0];
        user = new User(row[1], row[2]);
    }
    return user;
}