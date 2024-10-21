#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <regex.h>
#include <stdbool.h>

struct Date
{
    int month, day, year;
};

// database declaration
extern sqlite3 *db;

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char *name;
    char *country;
    char *phone;
    char *accountType;
    int accountNbr;
    double amount;
    char *deposit;
    // struct Date withdraw;
};

// user struct
struct User
{
    int id;
    char *name;
    char *password;
};

// authentication functions
void loginMenu(struct User **u);
void registerMenu(struct User **u);
int insertUser(struct User u);
int checkAuth(struct User u);

// system function
void createNewAcc(struct User u);
void insertRecord(struct Record r, struct User u);
void checkAllAccounts(struct User u);
int checkAccount(struct User u, char *input, int op);
int updateAccount(struct User u, char *input);
int deleteAccount(struct User u, char *input);
int transaction(struct User u, char *input);
int transfer(struct User user, char *input);
void mainMenu(struct User u);
void success(struct User u);

// database
void createDatabase();
void initDatabase();
int dbOpen(sqlite3 **db, const char *filename);
void dbClose(sqlite3 *db);
int dbExecute(sqlite3 *db, const char *query);
int dbQuery(sqlite3 *db, const char *query, sqlite3_stmt **stmt);
int dbBindInt(sqlite3_stmt *stmt, int index, int value);
int dbBindText(sqlite3_stmt *stmt, int index, const char *value);
int dbBindDouble(sqlite3_stmt *stmt, int index, double value);
int dbStep(sqlite3_stmt *stmt);
int getColumnInt(sqlite3_stmt *stmt, int col);
const char *getColumnText(sqlite3_stmt *stmt, int col);
double getColumnDouble(sqlite3_stmt *stmt, int col);
int isColumnNull(sqlite3_stmt *stmt, int col);
int dbFinalize(sqlite3_stmt *stmt);
int userNameExist(char *username);
// utils
void getPrompt(char **str);
char **split(char *str, char delim);
void getDateFromStrs(char **str, struct Date *date);
int countArr(char **arr);
void handleDbError(int rc, sqlite3 *db, const char *errmsg);
void handleStatementError(int rc, sqlite3 *db, const char *errmsg, sqlite3_stmt *stmt);
int checkExistingAccByUser(char *user, int id);
int checkExistingAcc(int id);
void printRecord(sqlite3_stmt *stmt);
int checkAmount(char *str);
int checkExistingUser(char *user, int *user_id);
int strIsInt(char *str);
int validate_date(const char *date);
void freeUser(struct User *u);
void freeRecord(struct Record *r);
#endif