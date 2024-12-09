#include <memory>
#include <utility>

#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

#include "MainApp.h"
#include "PasswordResetConfirmedView.h"
#include "SetupRoute.h"

PasswordResetConfirmedView::PasswordResetConfirmedView(MainApp *app) :
    View(app) 
{
    addTemplate();
    addBackToLoginScreenButton();
}

void PasswordResetConfirmedView::addTemplate() {
    templ = addWidget(
        std::make_unique<Wt::WTemplate>(
            Wt::WString::tr("password-reset-confirmed-template")
        )
    );
}

void PasswordResetConfirmedView::addBackToLoginScreenButton() {
    backToLoginScreen = templ->bindWidget(
        "back-to-login",
        std::make_unique<Wt::WPushButton>("Back to Login")
    );

    backToLoginScreen->setLink(
        Wt::WLink(
            Wt::LinkType::InternalPath,
            "/login"
        )
    );
}

SETUP_ROUTE("/password/reset/confirmed", PasswordResetConfirmedView)

