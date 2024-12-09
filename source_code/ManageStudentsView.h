#ifndef MANAGE_STUDENTS_H
#define MANAGE_STUDENTS_H

#include <Wt/WApplication.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTemplate.h>

#include "Session.h"
#include "View.h"

class ManageStudentsView: public View {
public:
	explicit ManageStudentsView(MainApp*);
private:
	AASS::Session &session;
	Wt::Auth::User userToEdit;

	Wt::WTemplate *templ;
	Wt::WPushButton *searchButton;
	Wt::WPushButton *addStudentButton;
	Wt::WPushButton *saveChangesButton;
	Wt::WPushButton *deleteStudentButton;
	Wt::WPushButton *returnButton;
	Wt::WLineEdit *searchInput;
	Wt::WLineEdit *firstNameEdit;
	Wt::WLineEdit *lastNameEdit;
	Wt::WLineEdit *emailEdit;
	Wt::WLineEdit *firstNameAdd;
	Wt::WLineEdit *lastNameAdd;
	Wt::WLineEdit *emailAdd;
	Wt::WText *changeConfirm;
	Wt::WText *deleteConfirm;
	Wt::WText *addConfirm;

	void addTemplate();
	void addSearchButton();
	void addAddStudentButton();
	void addSaveChangesButton();
	void addDeleteStudentButton();
	void addReturnButton();
	void addSearchInput();
	void addFirstNameEdit();
	void addLastNameEdit();
	void addEmailEdit();
	void addFirstNameAdd();
	void addLastNameAdd();
	void addEmailAdd();
	void addChangeConfirm();
	void addDeleteConfirm();
	void addAddConfirm();

	void sendErrorToast(std::string s);
	void saveChangesEventHandler();
	void addUserEventHandler();
	void deleteUserEventHandler();
	void resetEditInputs();
};

#endif // MANAGE_STUDENTS_H
