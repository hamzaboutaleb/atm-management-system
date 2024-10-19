#include "header.h"

void loginMenu(struct User **u)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    getPrompt(&(*u)->name);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    getPrompt(&(*u)->password);
    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
}

int checkAuth(struct User u)
{
    char *sql;
    int rc;
    sqlite3_stmt *stmt;

    sql = "SELECT 1 FROM users WHERE name = ? AND pass = ?";
    if (dbQuery(db, sql, &stmt) != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }
    dbBindText(stmt, 1, u.name);
    dbBindText(stmt, 2, u.password);
    if (dbStep(stmt) == SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        return 0;
    }
    else
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        exit(1);
    }
}