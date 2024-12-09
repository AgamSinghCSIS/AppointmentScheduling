#ifndef ADMIN_MANAGE_FACULTY_VIEW_H
#define ADMIN_MANAGE_FACULTY_VIEW_H

#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

#include "Session.h"
#include "View.h"

class AdminManageFacultyView : public View {
public:
    explicit AdminManageFacultyView(MainApp*);
private:
    AASS::Session &session;
    Wt::Auth::User userToEdit;

    Wt::WTemplate *templ;
    Wt::WLineEdit *searchInput;
    Wt::WPushButton *searchButton;
    Wt::WLineEdit *firstNameEdit;
    Wt::WLineEdit *lastNameEdit;
    Wt::WLineEdit *emailEdit;
    Wt::WPushButton *saveChangesButton;
    Wt::WPushButton *deleteFacultyButton;
    Wt::WLineEdit *firstNameEntry;
    Wt::WLineEdit *lastNameEntry;
    Wt::WLineEdit *emailEntry;
    Wt::WPushButton *addFacultyButton;

    void addTemplate();
    void addSearchInput();
    void addSearchButton();
    void addFirstNameEdit();
    void addLastNameEdit();
    void addEmailEdit();
    void addSaveChangesButton();
    void addDeleteFacultyButton();
    void addFirstNameEntry();
    void addLastNameEntry();
    void addEmailEntry();
    void addAddFacultyButton();

    void sendErrorToast(std::string s);
    void deleteUserEventHandler();
    void addUserEventHandler();
    void saveChangesEventHandler();
    void searchUserEventHandler();
    void resetEditInputs();
};

#endif // ADMIN_MANAGE_FACULTY_VIEW_H

