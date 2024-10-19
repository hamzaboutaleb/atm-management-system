#include "header.h"

void registerMenu(struct User **u)
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

int insertUser(struct User u)
{
    char *sql;
    int rc;
    sqlite3_stmt *stmt;

    sql = "INSERT INTO users (name, pass) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    handleDbError(rc, db, sqlite3_errmsg(db));
    rc = sqlite3_bind_text(stmt, 1, u.name, -1, SQLITE_STATIC);
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    rc = sqlite3_bind_text(stmt, 2, u.password, -1, SQLITE_STATIC);
    handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_CONSTRAINT)
        return 1;
    else if (rc != SQLITE_DONE)
        handleStatementError(rc, db, sqlite3_errmsg(db), stmt);
    sqlite3_finalize(stmt);
    return 0;
}