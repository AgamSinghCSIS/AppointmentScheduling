#ifndef PASSWORD_RESET_CONFIRMED_H 
#define PASSWORD_RESET_CONFIRMED_H

#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

#include "View.h"

class PasswordResetConfirmedView: public View {
public:
    explicit PasswordResetConfirmedView(MainApp*);
private:
    Wt::WTemplate *templ;
    Wt::WPushButton *backToLoginScreen;

    void addTemplate();
    void addBackToLoginScreenButton();
};

#endif // PASSWORD_RESET_CONFIRMED_H

