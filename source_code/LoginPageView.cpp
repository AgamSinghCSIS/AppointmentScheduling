#include <iostream>

#include <Wt/Auth/AuthWidget.h>
#include <Wt/WTemplate.h>

#include "LoginPageView.h"
#include "MainApp.h"
#include "Profile.h"
#include "Session.h"
#include "SetupRoute.h"
#include "UserRoleRedirectUtil.h"

LoginPageView::LoginPageView(MainApp *app) : View(app), redirectEarly(false) {
    // Configures auth widget and hooks auth event
    auto widget = makeAuthWidget();

    if (redirectEarly) {
        return;
    }

    addTemplate();
    addAuthWidget(std::move(widget));
}

void LoginPageView::addTemplate() {
    templ = addWidget(
        std::make_unique<Wt::WTemplate>(
            Wt::WString::tr("login-page-template")
        )
    );
    templ->setInternalPathEncoding(true);
}

std::unique_ptr<Wt::Auth::AuthWidget> LoginPageView::makeAuthWidget() {
    AASS::Session &session = parent->getSession();
    session.getLogin().changed().connect(this, &LoginPageView::authEvent);

    auto widget = std::make_unique<Wt::Auth::AuthWidget>(
        AASS::Session::getAuthService(),
        session.getUsers(),
        session.getLogin()
    );

    widget->model()->addPasswordAuth(
        &AASS::Session::getPasswordAuthService()
    );
    widget->setRegistrationEnabled(true);

    widget->processEnvironment();

    return widget;
}

void LoginPageView::addAuthWidget(std::unique_ptr<Wt::Auth::AuthWidget> w) {
    authWidget = templ->bindWidget(
        "auth-widget",
        std::move(w)
    );
}

void LoginPageView::authEvent() {
    redirectEarly = redirectFromLogin(parent);
}

SETUP_ROUTE("/login", LoginPageView)

