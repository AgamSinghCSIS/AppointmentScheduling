#include <Wt/WLink.h>
#include <Wt/WTemplate.h>

#include "MainApp.h"
#include "HomePageView.h"
#include "SetupRoute.h"

HomePageView::HomePageView(MainApp *app) : View(app) {
    addTemplate();

    addLoginLinkButton();
}

void HomePageView::addTemplate() {
    templ = addWidget(
        std::make_unique<Wt::WTemplate>(
            Wt::WString::tr("landing-page-template")
        )
    );
}

void HomePageView::addLoginLinkButton() {
    loginButton = templ->bindWidget(
        "login-button",
        std::make_unique<Wt::WPushButton>("Login to Continue")
    );
    loginButton->setLink(
        Wt::WLink(
            Wt::LinkType::InternalPath,
            "/login"
        )
    );
}

SETUP_ROUTE("/", HomePageView)

