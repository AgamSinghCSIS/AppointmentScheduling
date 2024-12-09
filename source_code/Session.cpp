#include <memory>
#include <utility>

#include <Wt/Auth/AuthService.h>
#include <Wt/Auth/Dbo/AuthInfo.h>
#include <Wt/Auth/HashFunction.h>
#include <Wt/Auth/PasswordService.h>
#include <Wt/Auth/PasswordStrengthValidator.h>
#include <Wt/Auth/PasswordVerifier.h>
#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/SqlConnection.h>

#include "Appointment.h"
#include "AdvisorPairing.h"
#include "DatabaseUtils.h"
#include "Profile.h"
#include "Session.h"
#include "Term.h"

// Global auth services. Maybe move these to a singleton?
namespace {
    Wt::Auth::AuthService authService;
    Wt::Auth::PasswordService passwordService(authService); 
}

namespace AASS {

Session::Session(std::unique_ptr<Wt::Dbo::SqlConnection> conn) :
    Wt::Dbo::Session(),
    users(nullptr)
{
    setConnection(std::move(conn));

    mapClass<Profile>("profile");
    mapClass<AuthInfo>("auth_info");
    mapClass<AuthInfo::AuthIdentityType>("auth_identity");
    mapClass<AuthInfo::AuthTokenType>("auth_token");
    mapClass<Term>("term");
    mapClass<AdvisorPairing>("advisor_pairing");
    mapClass<Appointment>("appointment");

    users = new UserDatabase(*this);
}

Session::~Session() {
    if (users) {
        delete users;
    }
}

void Session::configureAuth() {
    authService.setIdentityPolicy(Wt::Auth::IdentityPolicy::EmailAddress);
    authService.setAuthTokensEnabled(true, "logincookie");
    authService.setEmailVerificationEnabled(true);
    // authService.setEmailVerificationRequired(true);

    auto verifier = std::make_unique<Wt::Auth::PasswordVerifier>();
    verifier->addHashFunction(
        std::make_unique<Wt::Auth::BCryptHashFunction>(7)
    );
    passwordService.setVerifier(std::move(verifier));
    passwordService.setAttemptThrottlingEnabled(true);
    passwordService.setStrengthValidator(
        std::make_unique<Wt::Auth::PasswordStrengthValidator>()
    );
}

Wt::Auth::AbstractUserDatabase& Session::getUsers() {
    return *dynamic_cast<Wt::Auth::AbstractUserDatabase*>(users);
}

UserDatabase& Session::getUserDB() {
    return *users;
}

Wt::Dbo::ptr<Profile> Session::user() const {
    if (login.loggedIn()) {
        Wt::Dbo::ptr<AuthInfo> authInfo = users->find(login.user());
        return authInfo->user();
    }
    else {
        return Wt::Dbo::ptr<Profile>();
    }
}

Wt::Auth::Login& Session::getLogin() {
    return login;
}

const Wt::Auth::AuthService& Session::getAuthService() {
    return authService;
}

const Wt::Auth::PasswordService& Session::getPasswordAuthService() {
    return passwordService;
}

} // AASS

CREATE_TABLE(AuthInfo, auth_info)
CREATE_TABLE(AuthInfo::AuthIdentityType, auth_identity)
CREATE_TABLE(AuthInfo::AuthTokenType, auth_token)

