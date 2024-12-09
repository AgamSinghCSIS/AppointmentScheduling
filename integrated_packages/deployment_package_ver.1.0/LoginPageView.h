#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include <Wt/Auth/AuthWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>

#include "View.h"

class LoginPageView: public View {
public:
    explicit LoginPageView(MainApp*);
private:
    Wt::WTemplate *templ;
    Wt::Auth::AuthWidget *authWidget;
    bool redirectEarly;

    std::unique_ptr<Wt::Auth::AuthWidget> makeAuthWidget();
    void addTemplate();
    void addAuthWidget(std::unique_ptr<Wt::Auth::AuthWidget>);

    void authEvent();
};

#endif // LOGIN_PAGE_H

