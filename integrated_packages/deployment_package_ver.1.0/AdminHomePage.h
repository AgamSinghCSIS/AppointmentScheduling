#ifndef ADMIN_HOME_PAGE_H
#define ADMIN_HOME_PAGE_H

#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

#include "View.h"

class AdminHomePage: public View {

	public:
		explicit AdminHomePage(MainApp*);
	private:
		Wt::WTemplate *templ;
		Wt::WPushButton *logoutButton;
		
		void addTemplate();
        void addLogoutButton();
};
#endif // ADMIN_HOME_PAGE_H
