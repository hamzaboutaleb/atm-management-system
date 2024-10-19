#include "header.h"


sqlite3 *db = NULL;

void createDatabase()
{
    int rc;

    rc = sqlite3_open("data/database.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(1);
    }
}

void initDatabase()
{
    const char *user_sql, *record_sql;
    char *errmsg = NULL;
    int rc;

    user_sql =   "CREATE TABLE IF NOT EXISTS users ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "name TEXT NOT NULL UNIQUE, "
                "pass TEXT);";
    record_sql = "CREATE TABLE IF NOT EXISTS records ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "user_id INTEGER, "
                "user_name TEXT, "
                "account_id INTEGER UNIQUE, "
                "date_of_creation TEXT, "
                "country TEXT, "
                "phone_number TEXT, "
                "balance REAL, "
                "type_of_account TEXT, "
                "FOREIGN KEY (user_id) REFERENCES users (id) ON DELETE CASCADE);";

    rc = sqlite3_exec(db, user_sql, 0, 0, &errmsg);
    handleDbError(rc, db, errmsg);
    rc = sqlite3_exec(db, record_sql, 0, 0, &errmsg);
    handleDbError(rc, db, errmsg);
}

void insertRecord(struct Record r, struct User u)
{
    char *sql;
    int rc;
    sqlite3_stmt *stmt;

    sql =   "INSERT INTO records (user_id, user_name, account_id, date_of_creation, country, phone_number, balance, type_of_account)"
            "SELECT id, name, ?, ?, ?, ?, ?, ? FROM users WHERE name = ? LIMIT 1;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_int(stmt, 1, r.accountNbr);
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    rc = sqlite3_bind_text(stmt, 2, r.deposit, -1, SQLITE_STATIC);
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    rc = sqlite3_bind_text(stmt, 3, r.country, -1, SQLITE_STATIC);
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    rc = sqlite3_bind_text(stmt, 4, r.phone, -1, SQLITE_STATIC);
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    rc = sqlite3_bind_double(stmt, 5, r.amount);
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    rc = sqlite3_bind_text(stmt, 6, r.accountType, -1, SQLITE_STATIC);
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    rc = sqlite3_bind_text(stmt, 7, u.name, -1, SQLITE_STATIC);
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
        handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    sqlite3_finalize(stmt);
}

int dbOpen(sqlite3 **db, const char *filename) {
    int rc = sqlite3_open(filename, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    return SQLITE_OK;
}

void dbClose(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int dbExecute(sqlite3 *db, const char *query) {
    char *errmsg;
    int rc = sqlite3_exec(db, query, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
    return rc;
}

int dbQuery(sqlite3 *db, const char *query, sqlite3_stmt **stmt) {
    int rc = sqlite3_prepare_v2(db, query, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
    }
    return rc;
}

int dbBindInt(sqlite3_stmt *stmt, int index, int value) {
    int rc = sqlite3_bind_int(stmt, index, value);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind int: %s\n", sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
    return rc;
}

int dbBindText(sqlite3_stmt *stmt, int index, const char *value) {
    int rc = sqlite3_bind_text(stmt, index, value, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind text: %s\n", sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
    return rc;
}

int dbBindDouble(sqlite3_stmt *stmt, int index, double value) {
    int rc = sqlite3_bind_double(stmt, index, value);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to bind double: %s\n", sqlite3_errmsg(sqlite3_db_handle(stmt)));
    }
    return rc;
}

int dbStep(sqlite3_stmt *stmt) {
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(sqlite3_db_handle(stmt)));
        return rc; 
    }
    return rc; 
}


int getColumnInt(sqlite3_stmt *stmt, int col) {
    if (sqlite3_column_type(stmt, col) == SQLITE_NULL) {
        return 0; 
    }
    return sqlite3_column_int(stmt, col);
}

const char* getColumnText(sqlite3_stmt *stmt, int col) {
    if (sqlite3_column_type(stmt, col) == SQLITE_NULL) {
        return NULL;
    }
    return (const char*)sqlite3_column_text(stmt, col);
}

double getColumnDouble(sqlite3_stmt *stmt, int col) {
    if (sqlite3_column_type(stmt, col) == SQLITE_NULL) {
        return 0.0; // Or handle NULL as appropriate
    }
    return sqlite3_column_double(stmt, col);
}

int isColumnNull(sqlite3_stmt *stmt, int col) {
    return sqlite3_column_type(stmt, col) == SQLITE_NULL;
}