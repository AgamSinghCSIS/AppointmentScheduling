#include <iostream>

#include <Wt/WTemplate.h>
#include <Wt/WContainerWidget.h>

#include "AdminHomePage.h"
#include "MainApp.h"
#include "SetupRoute.h"
#include "UserRoleRedirectUtil.h"

AdminHomePage::AdminHomePage(MainApp *app) : View(app) {
    if (redirectFromAdmin(parent)) {
        return;
    }
	addTemplate();
    addLogoutButton();
}

void AdminHomePage::addTemplate(){
	templ = addWidget(
		std::make_unique<Wt::WTemplate>(
			Wt::WString::tr("admin-home-page-template")
		)
	);
    templ->setInternalPathEncoding(true);
}

void AdminHomePage::addLogoutButton() {
    logoutButton = templ->bindWidget(
        "logout-button",
        std::make_unique<Wt::WPushButton>("Logout")
    ); 

    logoutButton->clicked().connect([=]{
        AASS::Session &session = parent->getSession();
        session.getLogin().logout();
        redirectFromAdmin(parent);
    });
}

SETUP_ROUTE("/admin", AdminHomePage)

