#include "header.h"

void mainMenu(struct User u)
{
    int option;
    char *input;
unknownAcc:
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    getPrompt(&input);
    option = atoi(input);
    free(input);
    switch (option)
    {
    case 1:
        createNewAcc(u);
    case 2:
        system("clear");
        printf("What is the account number you want to change:");
        getPrompt(&input);
        if (updateAccount(u, input))
        {
            free(input);
            goto unknownAcc;
        }
        free(input);
        system("clear");
        printf("\n✔ Success!\n\n");
        success(u);
        goto unknownAcc;
    case 3:
        system("clear");
        printf("\nEnter the account number:");
        getPrompt(&input);
        if (checkAccount(u, input, 0))
            goto unknownAcc;
        system("clear");
        success(u);
        goto unknownAcc;
    case 4:
        checkAllAccounts(u);
        goto unknownAcc;
    case 5:
        system("clear");
        printf("Enter the account number of the customer: ");
        getPrompt(&input);
        if (transaction(u, input))
            goto unknownAcc;
        system("clear");
        printf("\n✔ Success!\n\n");
        success(u);
        goto unknownAcc;
    case 6:
        system("clear");
        printf("What is the account number you want to delete:");
        getPrompt(&input);
        if (deleteAccount(u, input))
            goto unknownAcc;
        printf("\n✔ Success!\n\n");
        success(u);
        system("clear");
        goto unknownAcc;
    case 7:
        system("clear");
        printf("Enter the account number you want to transfer ownership:");
        getPrompt(&input);
        if (transfer(u, input))
            goto unknownAcc;
        printf("\n✔ Success!\n\n");
        success(u);
        system("clear");
        goto unknownAcc;
    case 8:
        freeUser(&u);
        dbClose(db);
        exit(1);
    default:
        printf("Invalid operation!\n");
        fflush(stdout);
        goto unknownAcc;
    }
}

void initMenu(struct User *u)
{
    int option;
    char *input = NULL;
notFound:
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");

    getPrompt(&input);
    option = atoi(input);
    free(input);
    switch (option)
    {
    case 1:
        loginMenu(&u);
        if (checkAuth(*u))
            return; // done succefully
        freeUser(u);
        printf("\nWrong password!! or User Name\n");
        fflush(stdout);
        sleep(1);
        goto notFound;
    case 2:
        registerMenu(&u);
        if (!insertUser(*u))
            return;
        freeUser(u);
        printf("\nUser already exists!\n");
        fflush(stdout);
        sleep(1);
        goto notFound;
    case 3:
        sqlite3_close(db);
        exit(1);
        goto notFound;
    default:
        printf("Insert a valid operation!\n");
        fflush(stdout);
        sleep(1);
        goto notFound;
    }
}

int main()
{
    struct User u;

    createDatabase();
    initDatabase();
    initMenu(&u);
    mainMenu(u);
    return 0;
}
