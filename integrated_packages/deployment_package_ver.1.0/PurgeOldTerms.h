#ifndef PURGE_OLD_TERMS
#define PURGE_OLD_TERMS

#include <iostream>

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>

#include "MainApp.h"
#include "Session.h"
#include "View.h"

class PurgeTerms : public View {
public:
    explicit PurgeTerms(MainApp*);
private:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

    AASS::Session &session;
    Wt::WTemplate *templ;
    Wt::WContainerWidget *termContainer;
        
    void addTemplate();
    void addPurgeButton();
    void renderTerms();
};

#endif // PURGE_OLD_TERMS
