#ifndef MAIN_APP_H
#define MAIN_APP_H

#include <Wt/WApplication.h>

#include "Session.h"

class MainApp: public Wt::WApplication {
public:
    explicit MainApp(const Wt::WEnvironment&);
    AASS::Session& getSession();
private:
    Wt::WContainerWidget *content;

    void onInternalPathChange();
    void showHomeScreen();
    void addMessageResourceBundles();

    AASS::Session session;
};


#endif // MAIN_APP_H
