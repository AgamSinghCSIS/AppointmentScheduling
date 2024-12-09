#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WLink.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

#include "MainApp.h"
#include "RequestPasswordResetView.h"
#include "SetupRoute.h"

RequestPasswordResetView::RequestPasswordResetView(MainApp *app) : View(app) {
    addTemplate();
    addEmailInput();
    addSubmitPasswordResetButton();
    addBackToLoginScreenButton();
}

void RequestPasswordResetView::addTemplate() {
    templ = addWidget(
        std::make_unique<Wt::WTemplate>(
            Wt::WString::tr("request-password-reset-template")
        )
    );
}

void RequestPasswordResetView::addEmailInput() {
    emailInput = templ->bindWidget(
        "email-input",
        std::make_unique<Wt::WLineEdit>()
    );
}

void RequestPasswordResetView::addSubmitPasswordResetButton() {
    submitPasswordReset = templ->bindWidget(
        "submit-request-button",
        std::make_unique<Wt::WPushButton>("Submit")
    );

    submitPasswordReset->clicked().connect([=]{
        std::cout << "Password reset button was clicked!" << std::endl;
        parent->setInternalPath("/password/reset/confirmed", true);
    });
}

void RequestPasswordResetView::addBackToLoginScreenButton() {
    backToLoginScreen = templ->bindWidget(
        "go-back-button",
        std::make_unique<Wt::WPushButton>("Back")
    );
    backToLoginScreen->setLink(
        Wt::WLink(
            Wt::LinkType::InternalPath,
            "/login"
        )
    );
}

SETUP_ROUTE("/password/reset", RequestPasswordResetView)

