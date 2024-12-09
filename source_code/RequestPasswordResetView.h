#ifndef REQUEST_PASSWORD_RESET_H
#define REQUEST_PASSWORD_RESET_H

#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

#include "MainApp.h"
#include "View.h"

class RequestPasswordResetView: public View {
public:
    explicit RequestPasswordResetView(MainApp*);
private:
    Wt::WTemplate *templ;
    Wt::WLineEdit *emailInput;
    Wt::WPushButton *submitPasswordReset;
    Wt::WPushButton *backToLoginScreen;

    void addTemplate();
    void addEmailInput();
    void addSubmitPasswordResetButton();
    void addBackToLoginScreenButton();
};

#endif // REQUEST_PASSWORD_RESET_H

