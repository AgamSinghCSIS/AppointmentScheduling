#include <Wt/Auth/Login.h>
#include <Wt/Auth/User.h>
#include <Wt/Dbo/Dbo.h>

#include "MainApp.h"
#include "Profile.h"
#include "Session.h"
#include "UserRoleRedirectUtil.h"

namespace dbo = Wt::Dbo;

namespace {

void redirectToLogin(MainApp *app) {
    app->setInternalPath("/login", true);
}

void redirectToAdmin(MainApp *app) {
    app->setInternalPath("/admin", true);
}

void redirectToStudent(MainApp *app) {
    app->setInternalPath("/student", true);
}

void redirectToFaculty(MainApp *app) {
    app->setInternalPath("/faculty", true);
}

bool redirectInvalidUser(MainApp *app) {
    AASS::Session &session = app->getSession();

    dbo::Transaction trans(session);

    Wt::Auth::Login &login = session.getLogin();

    if (!login.loggedIn()) {
        redirectToLogin(app);
        return true;
    }

    const Wt::Auth::User &user = login.user();
    if (!user.isValid()) {
        login.logout();
        redirectToLogin(app);
        return true;
    }

    return false;
}

UserRole getUserRole(MainApp *app) {
    AASS::Session &session = app->getSession();

    dbo::Transaction trans(session);

    UserDatabase &users = dynamic_cast<UserDatabase&>(session.getUsers());
    dbo::ptr<AuthInfo> authInfo = users.find(
        session.getLogin().user()
    );
    dbo::ptr<Profile> profile = authInfo->user();
    return profile->getRole();
}

}

bool redirectFromLogin(MainApp *app) {
    AASS::Session &session = app->getSession();

    dbo::Transaction trans(session);

    Wt::Auth::Login &login = session.getLogin();

    if (!login.loggedIn()) {
        return false;
    }

    const Wt::Auth::User &user = login.user();
    if (!user.isValid()) {
        login.logout();
        return false;
    }

    std::cerr << "User ID: " << user.id() << '\n';

    UserRole role = getUserRole(app);

    bool needToChange = false;
    switch (role) {
        case UserRole::STUDENT:
            needToChange = true;
            redirectToStudent(app);
            break;
        case UserRole::FACULTY:
            needToChange = true;
            redirectToFaculty(app);
            break;
        case UserRole::ADMIN:
            needToChange = true;
            redirectToAdmin(app);
            break;
        default:
            break;
    }
    return needToChange;
}

bool redirectFromAdmin(MainApp *app) {
    if (redirectInvalidUser(app)) {
        return true;
    }
    UserRole role = getUserRole(app);

    bool needToChange = false;
    switch (role) {
        case UserRole::STUDENT:
            redirectToStudent(app);
            needToChange = true;
            break;
        case UserRole::FACULTY:
            redirectToFaculty(app);
            needToChange = true;
            break;
        case UserRole::ADMIN:
            break;
        default:
            redirectToLogin(app);
            needToChange = true;
            break;
    }
    return needToChange;
}

bool redirectFromStudent(MainApp *app) {
    if (redirectInvalidUser(app)) {
        return true;
    }
    UserRole role = getUserRole(app);

    bool needToChange = false;
    switch (role) {
        case UserRole::STUDENT:
            break;
        case UserRole::FACULTY:
            redirectToFaculty(app);
            needToChange = true;
            break;
        case UserRole::ADMIN:
            redirectToAdmin(app);
            needToChange = true;
            break;
        default:
            redirectToLogin(app);
            needToChange = true;
            break;
    }
    return needToChange;
}

bool redirectFromFaculty(MainApp *app) {
    if (redirectInvalidUser(app)) {
        return true;
    }
    UserRole role = getUserRole(app);

    bool needToChange = false;
    switch (role) {
        case UserRole::STUDENT:
            redirectToStudent(app);
            needToChange = true;
            break;
        case UserRole::FACULTY:
            break;
        case UserRole::ADMIN:
            redirectToAdmin(app);
            needToChange = true;
            break;
        default:
            redirectToLogin(app);
            needToChange = true;
            break;
    }
    return needToChange;
}

