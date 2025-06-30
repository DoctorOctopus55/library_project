#include "user.h"
#ifndef RAPOR_H_INCLUDED
#define RAPOR_H_INCLUDED

#define ALLOWED_DAYS 15

int getTodayDayOfYear();
int calculateOverdue(int borrowedDay, int todayDay, int allowedDays);
void checkUserDelay(RegularUser user, int currentDay);
void checkAllBooksDelay();

#endif // RAPOR_H_INCLUDED
