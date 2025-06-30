#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user.h"
#include "book.h"
#include "fileProcess.h"

int readRegularUserFile(FILE *fp, RegularUser *tmpUser){ //Reads the data from regular users text file
    return fscanf(fp, "%49[^|]|%19[^|]|%d|%d|%d|%d|%d|%d|%d\n",
          tmpUser->userName, tmpUser->password, &tmpUser->borrowedBooks[0], &tmpUser->borrowDays[0], &tmpUser->borrowedBooks[1], &tmpUser->borrowDays[1],
          &tmpUser->borrowedBooks[2], &tmpUser->borrowDays[2], &tmpUser->numBorrow);
}


int readAdminUserFile(FILE *fp, AdminUser *tmpUser){ //reads data from admin users text file
    return fscanf(fp, "%49[^|]|%19[^|]|\n",tmpUser->userName, tmpUser->password);
}


int readBookFile(FILE *fp, Books *temp){ //reads data from books text file
     return fscanf(fp, "%d|%49[^|]|%49[^|]|%49[^|]|%d|%49[^|]|%d\n",
        &temp->id, temp->title, temp->author, temp->category, &temp->isBorrowed, temp->borrower, &temp->borrowDay);
}


int searchRegularUser(RegularUser *rglUser){ //checks if the given user present on the file. If present, the given user is overwritten according to what file says
    FILE *fp=fopen(REGULAR_USER_PATH, "r");
    if(fp==NULL)
        perror("File couldnt found: ");

    int isFound=0;
    RegularUser tmpUser;
    while(readRegularUserFile(fp, &tmpUser)==9){ //this function must always return 9
        if(strcmp(rglUser->userName, tmpUser.userName)==0 && strcmp(rglUser->password, tmpUser.password)==0){ //if username and password are found on the file
            strcpy(rglUser->userName, tmpUser.userName);
            strcpy(rglUser->password, tmpUser.password);
            rglUser->borrowedBooks[0]=tmpUser.borrowedBooks[0];
            rglUser->borrowedBooks[1]=tmpUser.borrowedBooks[1];
            rglUser->borrowedBooks[2]=tmpUser.borrowedBooks[2];
            rglUser->borrowDays[0]=tmpUser.borrowDays[0];
            rglUser->borrowDays[1]=tmpUser.borrowDays[1];
            rglUser->borrowDays[2]=tmpUser.borrowDays[2];
            rglUser->numBorrow=tmpUser.numBorrow;
            isFound=1;
            break;
        }

    }
    if(isFound!=1)
        printf("no name matched with the data we have");
    fclose(fp);
    return isFound;
}


int searchAdminUser(AdminUser *admUser){ //checks if the given admin is present on the file. If present, the given admin is overwritten according to what file says
    FILE *fp=fopen(ADMIN_USER_PATH, "r");
    if(fp==NULL)
        perror("File couldnt found: ");

    int isFound=0;
    AdminUser tmpUser;
    while(readAdminUserFile(fp, &tmpUser)!=EOF){
        if(strcmp(admUser->userName, tmpUser.userName)==0 && strcmp(admUser->password, tmpUser.password)==0){ //check if both the username and password present on the file
            strcpy(admUser->userName, tmpUser.userName);
            strcpy(admUser->password, tmpUser.password);
            isFound=1;
            break;
        }

    }
    fclose(fp);
    return isFound;
}


void addRegularUser(RegularUser rglUser){ //adds new user to file
    FILE *fp=fopen(REGULAR_USER_PATH, "a");
    if(fp==NULL)
        perror("file couldnt found: ");

    writeRegularUser(fp, rglUser);
    printf("Successfully added\n");
    //open user file for writing
    fclose(fp);
}


void removeRegularUser(RegularUser rglUser){ //remove a user that is present on the file
    FILE *fp=fopen(REGULAR_USER_PATH, "r");
    FILE *tmfp=fopen(TEMPORARY_REGULAR_USER_PATH, "w");
    if(fp==NULL || tmfp==NULL)
        perror("One of the files are not found: ");

    int isFound=0;
    RegularUser tmpUser;
    while(readRegularUserFile(fp, &tmpUser)==9){
        if(strcmp(rglUser.userName, tmpUser.userName)==0){
            printf("Successfully deleted\n");
            isFound=1;
        }

        else
            writeRegularUser(tmfp, tmpUser);
    }
    if(isFound==0)
        printf("Couldnt found a name that matches\n");

    fclose(fp);
    fclose(tmfp);

    if(remove(REGULAR_USER_PATH)!=0)
        perror("Error deleting books.txt");

    if(rename(TEMPORARY_REGULAR_USER_PATH, REGULAR_USER_PATH)!= 0)
        perror("Error renaming tmp.txt to books.txt");
}


void displayRegularUsers(){ //display all the users
    FILE *fp=fopen(REGULAR_USER_PATH, "r");
    if(fp==NULL)
        perror("File couldnt found: ");

    RegularUser tmp_usr;
    printf("\n======== REGULAR USERS LIST =========\n");
    printf("Borrowed books are displayed by their id, if its '0', then no book was borrowed in that section\nDays show the day the spesific book was borrowed\nNumber of books borrowed show how many books were borrowed by that user. If it is '-1' it means the user is warned\n");
    printf("=========================================\n\n");

    while(readRegularUserFile(fp, &tmp_usr)==9){
        printf("NAME:%s BORROWED BOOK1:%d DAY:%d BORROWED BOOK2:%d DAY:%d BORROWED BOOK3:%d DAY:%d NUM BORROWED:%d\n",
               tmp_usr.userName, tmp_usr.borrowedBooks[0], tmp_usr.borrowDays[0], tmp_usr.borrowedBooks[1], tmp_usr.borrowDays[1],
               tmp_usr.borrowedBooks[2], tmp_usr.borrowDays[2], tmp_usr.numBorrow);
    }
    fclose(fp);
}

