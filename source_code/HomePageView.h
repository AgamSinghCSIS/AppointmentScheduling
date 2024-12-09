#ifndef HOME_PAGE_H
#define HOME_PAGE_H

#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

#include "View.h"

class HomePageView: public View {
public:
    explicit HomePageView(MainApp*);
private:
    Wt::WTemplate *templ;
    Wt::WPushButton *loginButton;
    
    void addTemplate();
    void addLoginLinkButton();
};

#endif // HOME_PAGE_H
