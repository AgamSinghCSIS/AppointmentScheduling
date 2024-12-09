#ifndef STUDENT_HOME_PAGE
#define STUDENT_HOME_PAGE

#include <Wt/WContainerWidget.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

#include "MainApp.h"
#include "View.h"

class StudentHomePage: public View{

        public:
                explicit StudentHomePage(MainApp*);
        private:
                Wt::WTemplate *templ;
                Wt::WPushButton *logoutButton;

                void addTemplate();
                void addLogoutButton();
};

#endif // STUDENT_HOME_PAGE
