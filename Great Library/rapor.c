#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "user.h"
#include "fileProcess.h"
#include "rapor.h"


int getTodayDayOfYear() { //gets today's day of the year. if its february 7, function returns 38
    time_t now = time(NULL); //takes todays time as miliseconds
    struct tm *today = localtime(&now);
    return today->tm_yday + 1; // tm_yday starts from 0
}

int calculateOverdue(int borrowedDay, int todayDay, int allowedDays) { //calculates if the delay and if it has passed the allowed days
    int passedDays=todayDay-borrowedDay;
    int overdue=passedDays-allowedDays;
    return (overdue > 0) ? overdue : 0;
}


void checkUserDelay(RegularUser user, int currentDay){ //check delays user by user
    for(int i=0;i<3;i++){
        if(user.borrowedBooks[i]!=0){
            int delay=calculateOverdue(user.borrowDays[i], currentDay, 15);
            if(delay>0)
                printf("The book with the id:%d is %d days delayed\n", user.borrowedBooks[i], delay);
        }
    }
}


void checkAllBooksDelay(){ //checks books delay, book by book
    FILE *fp=fopen(BOOK_PATH, "r");
    if(fp==NULL)
        perror("File Couldnt Found: ");

    Books temp;
    while(readBookFile(fp, &temp)==7){
        int due = calculateOverdue(temp.borrowDay, getTodayDayOfYear(), ALLOWED_DAYS);
        if (due!=0 && temp.borrowDay!=0) {
            printf("Delayed Book -> ID: %d | Title: %s | Borrowed By: %s | Delay: %d days\n",
            temp.id, temp.title, temp.borrower, due);
        }
    }
    fclose(fp);
}
