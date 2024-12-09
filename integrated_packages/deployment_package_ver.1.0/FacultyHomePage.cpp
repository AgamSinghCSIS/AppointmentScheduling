#include <iostream>
#include <Wt/WTemplate.h>
#include <Wt/WContainerWidget.h>
#include "MainApp.h"
#include "FacultyHomePage.h"
#include "SetupRoute.h"
#include "UserRoleRedirectUtil.h"

FacultyHomePage::FacultyHomePage(MainApp *app) : View(app) {
	addTemplate();
	addLogoutButton();
}

void FacultyHomePage::addTemplate() {
	templ = addWidget(
		std::make_unique<Wt::WTemplate>(
			Wt::WString::tr("faculty-home-page-template")
		)
	);
	templ->setInternalPathEncoding(true);
}

void FacultyHomePage::addLogoutButton() {
	logoutButton = templ->bindWidget(
		"logout-button",
		std::make_unique<Wt::WPushButton>("Logout")
	);

	logoutButton->clicked().connect([=]{
		AASS::Session &session = parent->getSession();
		session.getLogin().logout();
		redirectFromFaculty(parent);
	});
}

SETUP_ROUTE("/faculty", FacultyHomePage)
