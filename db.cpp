//
// Created by Juan Carlos Espinoza Zurita on 16/11/17.
//

#include "db.h"

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for(i = 0; i<argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
int Db::dbCreate() {
    char *zErrMsg = 0;
    int rc;
    int status;

    if (db){
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            status = 0;
        } else {
            fprintf(stdout, "Table created successfully\n");
            status = 1;
        }
    }
    else
        status = 0;
    return status;
}
int Db::dbInsert() {
    int status = 0;
    sqlite3_stmt *statement;
    int result, rc;

    rc = sqlite3_prepare(db, sql, -1, &statement, 0);

    if (rc == SQLITE_OK)
    {
        int res = sqlite3_step(statement);
        result = res;
        sqlite3_finalize(statement);
        status = 1;
        std::cout << result;
    }
    else{
        fprintf(stderr, "SQL error: %i\n", rc);
    }
    return status;
}
int Db::dbCount() {
    int count = 0;
    sqlite3_stmt *statement;

    if ( sqlite3_prepare(db, sql, -1, &statement, 0 ) == SQLITE_OK )
    {
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
                count++;

            if ( res == SQLITE_DONE || res == SQLITE_ERROR)
            {
                std::cout << "";
                break;
            }
        }
    }
    return count;
}
std::vector<std::vector<std::string>> Db::dbSelect() {
    int status = 0;
    std::vector<std::vector <std::string>> values;
    std::vector<std::string> row;
    sqlite3_stmt *statement;

    if ( sqlite3_prepare(db, sql, -1, &statement, 0 ) == SQLITE_OK )
    {
        int ctotal = sqlite3_column_count(statement);
        int res = 0;

        while ( 1 )
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW )
            {
                for ( int i = 0; i < ctotal; i++ )
                {
                    std::string s = (char*)sqlite3_column_text(statement, i);
                    row.push_back(s);
                }
                values.push_back(row);
                status++;
            }

            if ( res == SQLITE_DONE || res == SQLITE_ERROR)
            {
                std::cout << "";
                break;
            }
        }
    }
    return values;
}

Db* Db::Instance = 0;

Db::Db(char* name) {
    this->name = name;
    sqlite3 *db;
    int rc;


    std::ifstream database (this->name);

    rc = sqlite3_open(this->name, &db);
    this->db = db;

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

    } else {
        fprintf(stderr, "Opened database successfully\n");
        if (database.is_open()){
            fprintf(stderr, "Database already created\n");
            database.close();
        }
        else{
            char sql[1000] = {0};
            std::string line;
            std::ifstream myfile ("../create.sql");
            if (myfile.is_open()){
                while (getline(myfile, line)){
                    strcat(sql, line.c_str());
                }
                myfile.close();
            }
            this->sql = sql;
            if (dbCreate() > 0)
                fprintf(stderr, "Database created successfully\n");
            else
                fprintf(stderr, "Error creating Database\n");
        }
    }
}
Db * Db::getInstance(char* name) {
    if (Instance == 0)
        Instance = new Db(name);
    return Instance;
}
sqlite3 * Db::getDb() {
    return db;
}
char* Db::getName() {
    return name;
}
void Db::setSql(char *sql) {
    this->sql = sql;
}
char* Db::getSql() {
    return sql;
}
Db::~Db() {
    sqlite3_close(db);
}

