#include <iostream>

#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>

#include "MainApp.h"
#include "StudentHomePage.h"
#include "SetupRoute.h"
#include "UserRoleRedirectUtil.h"

StudentHomePage::StudentHomePage(MainApp *app) : View(app) {
	addTemplate();
    addLogoutButton();
}

void StudentHomePage::addTemplate() {
	templ = addWidget(
		std::make_unique<Wt::WTemplate>(
			Wt::WString::tr("student-home-page-template")
		)
	);
    templ->setInternalPathEncoding(true);
}

void StudentHomePage::addLogoutButton() {
    logoutButton = templ->bindWidget(
        "logout-button",
        std::make_unique<Wt::WPushButton>("Logout")
    ); 

    logoutButton->clicked().connect([=]{
        AASS::Session &session = parent->getSession();
        session.getLogin().logout();
        redirectFromStudent(parent);
    });
}

SETUP_ROUTE("/student", StudentHomePage)

