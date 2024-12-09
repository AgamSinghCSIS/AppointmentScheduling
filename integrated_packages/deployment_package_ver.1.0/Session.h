#ifndef SESSION_H
#define SESSION_H

#include <memory>

#include <Wt/Auth/AuthService.h>
#include <Wt/Auth/Dbo/UserDatabase.h>
#include <Wt/Auth/Login.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/SqlConnection.h>
#include <Wt/WGlobal.h>

#include "Profile.h"

using UserDatabase = Wt::Auth::Dbo::UserDatabase<AuthInfo>;

namespace AASS {

class Session: public Wt::Dbo::Session {
public:
    explicit Session(std::unique_ptr<Wt::Dbo::SqlConnection>);

    ~Session();

    Wt::Auth::AbstractUserDatabase& getUsers();
    UserDatabase& getUserDB();
    Wt::Auth::Login& getLogin();

    Wt::Dbo::ptr<Profile> user() const;

    static void configureAuth();
    static const Wt::Auth::AuthService& getAuthService();
    static const Wt::Auth::PasswordService& getPasswordAuthService();
private:

    UserDatabase *users;
    Wt::Auth::Login login;
};

}

#endif // SESSION_H
