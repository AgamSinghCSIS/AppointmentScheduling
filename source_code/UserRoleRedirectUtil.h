#ifndef USER_ROLE_REDIRECT_UTIL_H
#define USER_ROLE_REDIRECT_UTIL_H

#include <Wt/Auth/User.h>

#include "MainApp.h"

bool redirectFromLogin(MainApp*);

bool redirectFromAdmin(MainApp*);

bool redirectFromStudent(MainApp*);

bool redirectFromFaculty(MainApp*);

#endif // USER_ROLE_REDIRECT_UTIL_H

