#ifndef FACULTY_HOME_PAGE_H
#define FACULTY_HOME_PAGE_H

#include "View.h"
#include <Wt/WContainerWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

class FacultyHomePage: public View {
public:
	explicit FacultyHomePage(MainApp*);
private:
	Wt::WTemplate *templ;
	Wt::WPushButton *logoutButton;

	void addTemplate();
	void addLogoutButton();
};

#endif // FACULTY_HOME_PAGE_H
