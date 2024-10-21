#include "header.h"

static void free_split(char **split_result)
{
    int i;

    i = 0;
    if (!split_result)
        return;
    while (split_result[i])
    {
        free(split_result[i]);
        i++;
    }
    free(split_result);
}

static int checkDate(struct Date date)
{
    if (date.day <= 0 || date.day > 31)
        return 1;
    else if (date.month <= 0 || date.month > 12)
        return 1;
    else if (date.year < 2000)
        return 1;
    return 0;
}

static int checkAccountType(char *str)
{
    if (strcmp(str, "saving") == 0)
        return 0;
    else if (strcmp(str, "current") == 0)
        return 0;
    else if (strcmp(str, "fixed01") == 0)
        return 0;
    else if (strcmp(str, "fixed02") == 0)
        return 0;
    else if (strcmp(str, "fixed03") == 0)
        return 0;
    return 1;
}

void createNewAcc(struct User u)
{
    char *input = NULL;
    char **strs;
    struct Date date;
    struct Record record;

invalidDate:
    system("clear");
    printf("\t\t\t===== New record =====\n");
    printf("\nEnter today's date(mm/dd/yyyy):");
    getPrompt(&input);
    if (!validate_date(input))
    {
        free(input);
        printf("\n\nInvalid date!!\n\n");
        sleep(1);
        goto invalidDate;
    }
    strs = split(input, '/');
    getDateFromStrs(strs, &date);
    free_split(strs);
    record.deposit = input;
invalidAccountnumber:
    printf("\nEnter the account number:");
    getPrompt(&input);
    if ((record.accountNbr = atoi(input)) <= 0 && strcmp(input, "0") != 0)
    {
        free(input);
        printf("\n\nInvalid account number!!\n\n");
        fflush(stdout);
        sleep(1);
        system("clear");
        goto invalidAccountnumber;
    }
    free(input);
    if (checkExistingAcc(record.accountNbr))
    {
        printf("\n\nAccount number Already exists\n\n");
        fflush(stdout);
        sleep(1);
        system("clear");
        goto invalidAccountnumber;
    }
    printf("\nEnter the country:");
    getPrompt(&record.country);
invalidPhonenumber:
    printf("\nEnter the phone number:");
    getPrompt(&record.phone);
    if (!strIsInt(record.phone))
    {
        free(&record.phone);
        printf("\n\nInvalid phone number!!\n\n");
        fflush(stdout);
        sleep(1);
        system("clear");
        goto invalidPhonenumber;
    }
invalidAmount:
    printf("\nEnter amount to deposit: $");
    getPrompt(&input);
    if (checkAmount(input))
    {
        free(input);
        printf("\n\nInvalid amount to deposit!!\n\n");
        fflush(stdout);
        sleep(1);
        system("clear");
        goto invalidAmount;
    }
    record.amount = strtod(input, NULL);
    free(input);
invalidType:
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    getPrompt(&input);
    if (checkAccountType(input))
    {
        free(input);
        printf("\n\nInvalid account type!!\n\n");
        fflush(stdout);
        sleep(1);
        system("clear");
        goto invalidType;
    }
    record.accountType = input;
    insertRecord(record, u);
    freeRecord(&record);
    printf("\n✔ Success!\n\n");
    success(u);
}
