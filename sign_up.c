#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>

#define MAXUSERSIZE 100

char *PROJECTPATH, *ADMINPATH, *USERSPATH;

// void userLogin(char *username, char *password, FILE *fp, int flag);
void userLogin(char *usrName, uint32_t password, FILE *fp, int flag);
void adminLogin();
uint32_t hashFunc(char *key, size_t len);

int main()
{
    system("clear");
    PROJECTPATH = (char *)malloc(sizeof(char) * 100);
    ADMINPATH = (char *)malloc(sizeof(char) * 100);
    USERSPATH = (char *)malloc(sizeof(char) * 100);
    PROJECTPATH = getenv("PWD");
    strcpy(ADMINPATH, PROJECTPATH);
    strcat(ADMINPATH, "/Admin");
    strcpy(USERSPATH, PROJECTPATH);
    strcat(USERSPATH, "/Users");

launch:
    printf("Welcome To DA_Forces\n\n"

           "Select the appropriate choice from following\n\n"

           "1) Enter 0 for User\n"
           "2) Enter 1 for Admin\n"
           "3) Enter -1 for Exit\n\n");

    int choice_1;
    scanf("%d", &choice_1);
    system("clear");
    if (choice_1 == 0)
    {
        printf("Select the appropriate choice from following\n\n"

               "1) Enter 0 for Login\n"
               "2) Enter 1 for Signup\n\n");

        int choice_2;
        scanf("%d", &choice_2);

        system("clear");

        // Login
        if (choice_2 == 0)
        {
            char us_id[50], password[50];
            printf("Username : ");
            scanf("%s", us_id);
            printf("Password : ");
            scanf("%s", password);
            char *path = (char *)malloc(sizeof(char) * 100);
            strcpy(path, ADMINPATH);
            strcat(path, "/users_data.txt");
            FILE *ptr = fopen(path, "r");
            if (ptr == NULL)
            {
                printf("Error in opening file\n");
                free(path);
                exit(1);
            }
            char *uname;
            uname = (char *)malloc(sizeof(char) * 50);
            uint32_t pass;

            int flag = 1;
            while (fscanf(ptr, "%s", uname) != EOF)
            {
                fscanf(ptr, "%u", &pass);
                if (strcmp(uname, us_id) == 0)
                {
                    if (pass == hashFunc(password, strlen(password)))
                    {
                        printf("\nLogin Successfull\n");
                        flag = 0;
                        fclose(ptr);
                        sleep(1);
                        system("clear");
                        userLogin(us_id, pass, ptr, 0);
                        break;
                    }
                    else
                    {
                        flag = 0;
                        printf("\nWrong Password\n");
                        sleep(1);
                        system("clear");
                    }
                }
            }
            if (flag)
            {
                printf("\nUser not found\n");
                sleep(1);
                system("clear");
            }
            free(path);
            free(uname);
            goto launch;
        }
        else
        {
            char us_id[50], password[50], confmPass[50];
        username:
            printf("Username : ");
            scanf("%s", us_id);

            char *path = (char *)malloc(sizeof(char) * 100);
            strcpy(path, ADMINPATH);
            strcat(path, "/users_data.txt");
            FILE *ptr = fopen(path, "r");
            if (ptr == NULL)
            {
                printf("Error in opening file\n");
                free(path);
                exit(1);
            }
            char *uname;
            uname = (char *)malloc(sizeof(char) * 50);
            uint32_t pass;
            int flag = 0;
            while (flag != EOF)
            {
                fscanf(ptr, "%s", uname);
                if (strcmp(uname, us_id) == 0)
                {
                    printf("Username already exists\n\n");
                    fclose(ptr);
                    free(uname);
                    free(path);
                    goto username;
                }
                flag = fscanf(ptr, "%u", &pass);
            }
        confirmPassword:
            printf("Password : ");
            scanf("%s", password);
            printf("Confirm Password : ");
            scanf("%s", confmPass);
            if (strcmp(password, confmPass) == 0)
            {
                FILE *ptr = fopen(path, "a");
                fprintf(ptr, "%s %u\n", us_id, hashFunc(password, strlen(password)));
                fclose(ptr);
                printf("\n\nSucessfully Signup\n");
                sleep(1);
                system("clear");
                userLogin(us_id, hashFunc(password, strlen(password)), ptr, 1);
            }
            else
            {
                printf("\n\nPassword and Confirm Password doesn't match\n\n");
                goto confirmPassword;
            }
            free(path);
            free(uname);
            goto launch;
        }
    }
    else if (choice_1 == 1)
    {
        adminLogin();
    }
    else if (choice_1 == -1)
    {
        printf("\n\nExiting...\n\n");
        // free(PROJECTPATH);
        free(ADMINPATH);
        free(USERSPATH);
        sleep(1);
        exit(0);
    }
    else
    {
        printf("\nWrong Choice\n");
        sleep(1);
        system("clear");
        goto launch;
    }
    return 0;
}

// Admin login page
void adminLogin()
{
    printf("Select the appropriate choice from following\n\n"

           "1) Enter 0 to Shut-Down System\n"
           "2) Enter 1 to Adding Question\n"
           "3) Enter 2 to edit Question\n\n");

    int choice_3;
    scanf("%d", &choice_3);

    system("clear");
    char Que_id[100];
    if (choice_3 == 1)
    {
        printf("Enter Question ID : ");

        scanf("%s", Que_id);

        char *cmd;
        char *path;
        path = (char *)malloc(100 * sizeof(char));
        cmd = (char *)malloc(100 * sizeof(char));

        // Making Que_id directory
        strcpy(cmd, "mkdir Admin/Questions/");
        strcat(cmd, Que_id);
        system(cmd);

        // Storing address in path variable
        strcpy(path, "Admin/Questions/");
        strcat(path, Que_id);
        strcat(path, "/");

        // Creating description.txt
        strcpy(cmd, "gedit ");
        strcat(cmd, path);
        strcat(cmd, "description.txt");
        system(cmd);

        // Creating in.txt
        strcpy(cmd, "gedit ");
        strcat(cmd, path);
        strcat(cmd, "in.txt");
        system(cmd);

        // Creating out.txt
        strcpy(cmd, "gedit ");
        strcat(cmd, path);
        strcat(cmd, "out.txt");
        system(cmd);

        free(cmd);
        free(path);

        system("clear");
        printf("\n\nQuestion added successfully\n");
        sleep(1);
        system("clear");
        adminLogin();
    }
    else if (choice_3 == 0)
    {
        // Shut-Down System
        printf("\nShutting Down System...\n\n");
        sleep(1);
        exit(0);
    }
    else
    {
        char *questionsPath = (char *)malloc(sizeof(char) * 100);
        strcpy(questionsPath, ADMINPATH);
        strcat(questionsPath, "/Questions");
        DIR *dir;
        dir = opendir(questionsPath);
        if (dir == NULL)
        {
            printf("Error: Directory not found\n");
            sleep(1);
            free(questionsPath);
            adminLogin();
        }
        else
        {
            struct dirent *entry;
            int index = 0;
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry->d_name[0] != '.')
                {
                    index++;
                }
            }
            closedir(dir);

            char *saveQuestion[index];
            index = 0;
            dir = opendir(questionsPath);
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry->d_name[0] != '.')
                {
                    saveQuestion[index] = (char *)malloc(sizeof(char) * 100);
                    saveQuestion[index++] = entry->d_name;
                }
            }
            closedir(dir);

            for (int i = 0; i < index; i++)
            {
                for (int j = i + 1; j < index; j++)
                {
                    if (strcmp(saveQuestion[i], saveQuestion[j]) > 0)
                    {
                        char *temp;
                        temp = saveQuestion[i];
                        saveQuestion[i] = saveQuestion[j];
                        saveQuestion[j] = temp;
                    }
                }
            }
            for (int i = 0; i < index; i++)
            {
                printf("%s\n", saveQuestion[i]);
            }

            printf("\nEnter Question ID : ");
            scanf("%s", Que_id);

            system("clear");

            printf("\nSelect the appropriate choice from following\n\n"

                   "1) Enter 0 for description file\n"
                   "2) Enter 1 for in file\n"
                   "3) Enter 2 for out file\n\n");

            int choice_5;
            scanf("%d", &choice_5);

            char *cmd;
            cmd = (char *)malloc(100 * sizeof(100));

            strcpy(cmd, "gedit Admin/Questions/");
            strcat(cmd, Que_id);
            if (choice_5 == 0)
                strcat(cmd, "/description.txt");
            else if (choice_5 == 1)
                strcat(cmd, "/in.txt");
            else
                strcat(cmd, "/out.txt");

            system(cmd);
            system("clear");

            printf("\n\nQuestion edited successfully\n");
            sleep(1);
            system("clear");
            free(cmd);
            free(questionsPath);
            adminLogin();
        }
    }
}

// void userLogin(char *username, char *password, FILE *fp, int flag)
void userLogin(char *usrName, uint32_t password, FILE *fp, int flag)
{
    char username[100];
    strcpy(username, usrName);
    char *folderPath = (char *)malloc(sizeof(char) * 100);
    strcpy(folderPath, ADMINPATH);
    strcat(folderPath, "/users/");
    strcat(folderPath, username);

    if (flag)
    {
        mode_t mode = 0777;
        mkdir(folderPath, mode);
    }
    free(folderPath);
    printf("Select the appropriate choice from following\n\n"

           "1) Enter 0 for Solve Question\n"
           "2) Enter 1 for Exit\n\n");

    int choice;
    scanf("%d", &choice);
    system("clear");
    if (choice == 0)
    {
    display:;
        char *questionsPath = (char *)malloc(sizeof(char) * 100);
        strcpy(questionsPath, ADMINPATH);
        strcat(questionsPath, "/Questions");
        DIR *dir;
        dir = opendir(questionsPath);
        if (dir == NULL)
        {
            printf("Error: Directory not found\n");
            return;
        }
        else
        {
            struct dirent *entry;
            int index = 0;
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry->d_name[0] != '.')
                {
                    index++;
                }
            }
            closedir(dir);

            index = 0;
            char *saveQuestion[index];
            dir = opendir(questionsPath);
            while ((entry = readdir(dir)) != NULL)
            {
                if (entry->d_name[0] != '.')
                {
                    saveQuestion[index] = (char *)malloc(sizeof(char) * 100);
                    saveQuestion[index++] = entry->d_name;
                }
            }
            closedir(dir);

            for (int i = 0; i < index; i++)
            {
                for (int j = i + 1; j < index; j++)
                {
                    if (strcmp(saveQuestion[i], saveQuestion[j]) > 0)
                    {
                        char *temp;
                        temp = saveQuestion[i];
                        saveQuestion[i] = saveQuestion[j];
                        saveQuestion[j] = temp;
                    }
                }
            }
            for (int i = 0; i < index; i++)
            {
                printf("%s\n", saveQuestion[i]);
            }

            printf("\nSelect the appropriate choice from following\n\n"

                   "1) Enter Question Number\n"
                   "2) Enter -1 for Exit\n\n");
            int choice;
            scanf("%d", &choice);
            if (choice == -1)
            {
                printf("\n\nExiting...\n\n");
                free(questionsPath);
                sleep(1);
                exit(0);
            }
            else
            {
                // Read Question.
                char *question = (char *)malloc(sizeof(char) * 100);
                strcpy(question, questionsPath);

                dir = opendir(questionsPath);
                struct dirent *ques;
                while ((ques = readdir(dir)) != NULL)
                {
                    if (ques->d_name[0] - '0' != choice)
                    {
                        continue;
                    }
                    else
                    {
                        strcat(question, "/");
                        strcat(question, ques->d_name);
                        strcat(question, "/description.txt");
                        break;
                    }
                }
                closedir(dir);
                char *cmd;
                cmd = (char *)malloc(sizeof(char) * 100);
                strcpy(cmd, "ls Admin/users/");
                strcat(cmd, username);
                strcat(cmd, "/");
                strcat(cmd, ques->d_name);
                if (system(cmd) != 0)
                {
                    system("clear");
                    char *cmd2;
                    cmd2 = (char *)malloc(sizeof(char) * 100);
                    strcpy(cmd2, "mkdir Admin/users/");
                    strcat(cmd2, username);
                    strcat(cmd2, "/");
                    strcat(cmd2, ques->d_name);
                    system(cmd2);
                    free(cmd2);
                }
                system("clear");
                free(cmd);
                FILE *ptr = fopen(question, "r");
                if (ptr == NULL)
                {
                    printf("Error in opening file\n");
                    exit(1);
                }
                char buffer;
                while ((buffer = fgetc(ptr)) != EOF)
                {
                    printf("%c", buffer);
                }
                printf("\n");
                fclose(ptr);
                free(questionsPath);
                free(question);

                printf("Select the appropriate choice from following\n\n"

                       "1) Enter 0 to Code\n"
                       "2) Enter 1 to go back\n\n");

                int choice;
                scanf("%d", &choice);
                if (choice == 0)
                {
                    system("touch Users/submission.c; chmod 755 Users/submission.c; gedit Users/submission.c");
                    system("clear");
                    printf("Select the appropriate choice from following\n\n"

                           "1) Enter 0 for Submit Code\n"
                           "2) Enter 1 to Cancel\n\n");

                    int choice;
                    scanf("%d", &choice);
                    if (choice == 0)
                    {
                        char *cmd_move;
                        cmd_move = (char *)malloc(sizeof(char) * 100);
                        strcpy(cmd_move, "mv Users/submission.c Admin/users/");
                        strcat(cmd_move, username);
                        strcat(cmd_move, "/");
                        strcat(cmd_move, ques->d_name);
                        system(cmd_move);
                        free(cmd_move);

                        // System Testing.

                        // Error
                        char *cmd_run;
                        cmd_run = (char *)malloc(sizeof(char) * 100);
                        strcpy(cmd_run, "./Admin/runQuestion.sh ");
                        strcat(cmd_run, ques->d_name);
                        strcat(cmd_run, " ");
                        strcat(cmd_run, username);
                        system(cmd_run);
                        free(cmd_run);

                        // Display Verdict.txt
                        FILE *ptr = fopen("verdict.txt", "r");
                        if (ptr == NULL)
                        {
                            printf("Error in opening file\n");
                            exit(1);
                        }
                        char buffer;
                        while ((buffer = fgetc(ptr)) != EOF)
                        {
                            printf("%c", buffer);
                        }
                        printf("\n");
                        fclose(ptr);
                        sleep(10);
                        system("clear");
                        system("rm verdict.txt");
                    }
                    system("rm Users/submission*");
                }
                else
                {
                    system("clear");
                    goto display;
                }
            }
        }
    }
    else
    {
        printf("Logging Out...\n\n");
        sleep(1);
    }
}

uint32_t hashFunc(char *key, size_t len)
{
    uint32_t hash = 5381;
    int c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
