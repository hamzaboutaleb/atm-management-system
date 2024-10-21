#include "header.h"

int deleteAccount(struct User u, char *input)
{
    char *sql;
    int acc_id;
    int rc;
    sqlite3_stmt *stmt;

    if (checkAccount(u, input, 1))
        return false;
    acc_id = atoi(input);
    sql = "DELETE FROM records WHERE user_name = ? AND account_id = ?";
    dbQuery(db, sql, &stmt);
    dbBindText(stmt, 1, u.name);
    dbBindInt(stmt, 2, acc_id);
    rc = dbStep(stmt);
    if (dbStep(stmt) == SQLITE_DONE)
        return true;
    dbFinalize(stmt);
    return false;
}