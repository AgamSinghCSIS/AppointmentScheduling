#include <boost/algorithm/string.hpp>
#include <chrono>
#include <utility>
#include <Wt/WTemplate.h>
#include <Wt/Auth/Identity.h>
#include <Wt/WEmailValidator.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Auth/AuthUtils.h>
#include "ManageStudentsView.h"
#include "MainApp.h"
#include "SetupRoute.h"
#include "Profile.h"
#include "ProfileBuilder.h"
#include "DatabaseUtils.h"
#include "Session.h"
#include "UserRoleRedirectUtil.h"
#include "AdvisorPairing.h"
#include "ITermQuery.h"
#include "Term.h"
#include "TermQuery.h"

namespace dbo = Wt::Dbo;

ManageStudentsView::ManageStudentsView(MainApp *app) :
       	View(app), 
	session(app->getSession()) 
{
	if (redirectFromFaculty(parent)) {
		return;
	}

        addTemplate();
	addSearchButton();
        addAddStudentButton();
	addSaveChangesButton();
	addDeleteStudentButton();
	addReturnButton();
	addSearchInput();
	addFirstNameEdit();
	addLastNameEdit();
	addEmailEdit();
	addFirstNameAdd();
	addLastNameAdd();
	addEmailAdd();
	addChangeConfirm();
	addDeleteConfirm();
	addAddConfirm();
}

void ManageStudentsView::addTemplate() {
	templ = addWidget(
		std::make_unique<Wt::WTemplate>(
			Wt::WString::tr("manage-students-template")
		)
	);
}

void ManageStudentsView::addSearchButton() {
	searchButton = templ->bindWidget(
		"search-button",
		std::make_unique<Wt::WPushButton>("Search")
	);

	searchButton->clicked().connect([=]{
		if (searchInput->validate() != Wt::ValidationState::Valid) {
			return;
		}

		dbo::Transaction trans(session);

		auto errorReturn = [=] {
			resetEditInputs();
			userToEdit = Wt::Auth::User();
			sendErrorToast(
				"Unable to find student based on given email."
			);
		};

		auto &users = session.getUserDB();

		std::string emailToFind = searchInput->text().toUTF8();

		Wt::Auth::User userLookup = users.findWithIdentity(
			Wt::Auth::Identity::LoginName,
			emailToFind
		);

		if (!userLookup.isValid()){
			return errorReturn();
		}

		dbo::ptr<AuthInfo> authInfo = users.find(userLookup);
		dbo::ptr<Profile> profile = authInfo->user();

		if (profile->getRole() != UserRole::STUDENT) {
			return errorReturn();
		}

		userToEdit = userLookup;

		firstNameEdit->setText(profile->getFirstName());
		lastNameEdit->setText(profile->getLastName());
		emailEdit->setText(emailToFind);
	});
}

void ManageStudentsView::addAddStudentButton() {
	addStudentButton = templ->bindWidget(
		"add-student-button",
		std::make_unique<Wt::WPushButton>("Add Student")
	);

	addStudentButton->clicked().connect([=]{
		addUserEventHandler();
	});
}

void ManageStudentsView::addSaveChangesButton() {
	saveChangesButton = templ->bindWidget(
		"save-changes-button",
		std::make_unique<Wt::WPushButton>("Save Changes")
	);

	saveChangesButton->clicked().connect([=]{
		saveChangesEventHandler();
	});
}

void ManageStudentsView::addDeleteStudentButton() {
	deleteStudentButton = templ->bindWidget(
		"delete-student-button",
		std::make_unique<Wt::WPushButton>("Delete Student")
	);

	deleteStudentButton->clicked().connect([=]{
		deleteUserEventHandler();
	});
}

void ManageStudentsView::addReturnButton() {
	returnButton = templ->bindWidget(
		"return-button",
		std::make_unique<Wt::WPushButton>("Back")
	);

	returnButton->setLink(
		Wt::WLink(
			Wt::LinkType::InternalPath,
			"/faculty"
		)
	);
}

void ManageStudentsView::addSearchInput() {
	searchInput = templ->bindWidget(
		"search-input",
		std::make_unique<Wt::WLineEdit>("")
	);
	searchInput->setValidator(std::make_unique<Wt::WEmailValidator>());
}

void ManageStudentsView::addFirstNameEdit() {
	firstNameEdit = templ->bindWidget(
		"first-name-edit",
		std::make_unique<Wt::WLineEdit>("")
	);
}

void ManageStudentsView::addLastNameEdit() {
	lastNameEdit = templ->bindWidget(
		"last-name-edit",
		std::make_unique<Wt::WLineEdit>("")
	);
}

void ManageStudentsView::addEmailEdit() {
	emailEdit = templ->bindWidget(
		"email-edit",
		std::make_unique<Wt::WLineEdit>("")
	);
	emailEdit->setValidator(std::make_unique<Wt::WEmailValidator>());
}

void ManageStudentsView::addFirstNameAdd() {
	firstNameAdd = templ->bindWidget(
		"first-name-add",
		std::make_unique<Wt::WLineEdit>("")
	);
}

void ManageStudentsView::addLastNameAdd() {
	lastNameAdd = templ->bindWidget(
		"last-name-add",
		std::make_unique<Wt::WLineEdit>("")
	);
}

void ManageStudentsView::addEmailAdd() {
	emailAdd = templ->bindWidget(
		"email-add",
		std::make_unique<Wt::WLineEdit>("")
	);
	emailAdd->setValidator(std::make_unique<Wt::WEmailValidator>());
}

void ManageStudentsView::addChangeConfirm() {
	changeConfirm = templ->bindWidget(
		"change-confirm",
		std::make_unique<Wt::WText>("")
	);
}

void ManageStudentsView::addDeleteConfirm() {
	deleteConfirm = templ->bindWidget(
		"delete-confirm",
		std::make_unique<Wt::WText>("")
	);
}


void ManageStudentsView::addAddConfirm() {
	addConfirm = templ->bindWidget(
		"add-confirm",
		std::make_unique<Wt::WText>("")
	);
}

void ManageStudentsView::sendErrorToast(std::string s) {
	Wt::WTemplate *toastTempl = addWidget(
		std::make_unique<Wt::WTemplate>(
			Wt::WString::tr(
				"error-toast"
			)
		)
	);
	toastTempl->bindWidget(
		"toast-error-msg",
		std::make_unique<Wt::WText>(
			std::move(s)
		)
	);
	toastTempl->doJavaScript(
		"let containerEl = document.querySelector("
		"  '#" + toastTempl->id() + "'"
		");"
		"console.log(containerEl);"
		"let toastElList = [].slice.call("
		"  containerEl.querySelectorAll('.toast')"
		");"
		"console.log(toastElList);"
		"let toastList = toastElList.map((toastEl) => {"
		"  return new bootstrap.Toast(toastEl);"
		"});"
		"toastList.forEach(toast => toast.show());"
	);
}

void ManageStudentsView::resetEditInputs() {
	firstNameEdit->setText("");
	lastNameEdit->setText("");
	emailEdit->setText("");
	searchInput->setText("");
}

void ManageStudentsView::saveChangesEventHandler() {
	dbo::Transaction trans(session);

	UserDatabase &users = session.getUserDB();

	if (!userToEdit.isValid()) {
		return sendErrorToast(
			"No selected user to edit."
		);
	}

	if (emailEdit->validate() != Wt::ValidationState::Valid) {
		return sendErrorToast(
			"Cannot give a student an invalid email."
		);
	}

	dbo::ptr<AuthInfo> authInfo = users.find(userToEdit);
	dbo::ptr<Profile> profile = authInfo->user();

	std::string emailToChange = emailEdit->text().toUTF8();

	profile.modify()->setFirstName(firstNameEdit->text().toUTF8());
	profile.modify()->setLastName(lastNameEdit->text().toUTF8());
	userToEdit.setIdentity(
		Wt::Auth::Identity::LoginName,
		emailToChange
	);
	userToEdit.setEmail(emailToChange);

	searchInput->setText(emailToChange);
	changeConfirm->setText("Student profile edited.");
	addConfirm->setText("");
	deleteConfirm->setText("");
}

void ManageStudentsView::deleteUserEventHandler() {
	dbo::Transaction trans(session);

	UserDatabase &users = session.getUserDB();

	Wt::Dbo::ptr<AuthInfo> authInfo = users.find(
		session.getLogin().user()
	);
	int64_t facultyID = authInfo.id();

	if (!userToEdit.isValid()) {
		return sendErrorToast(
			"No selected student to delete."
		);
	}

	authInfo = users.find(
		userToEdit
	);
	int64_t studentID = authInfo.id();

	using AdvisorPairings = dbo::collection<dbo::ptr<AdvisorPairing>>;

	AdvisorPairings pairings = session
		.find<AdvisorPairing>()
		.where("advisorID = ?")
		.bind(facultyID)
		.where("studentID = ?")
		.bind(studentID)
		;

	for (auto &pairing : pairings) {
		pairing.remove();
	}

	AdvisorPairings studentPairings = session
		.find<AdvisorPairing>()
		.where("studentID = ?")
		.bind(studentID)
		;
		
	if (studentPairings.size() < 1) {
		users.deleteUser(userToEdit);		
	}

	userToEdit = Wt::Auth::User();

	resetEditInputs();
	deleteConfirm->setText("Student profile deleted.");
	addConfirm->setText("");
	changeConfirm->setText("");
}

void ManageStudentsView::addUserEventHandler() {
	if (emailAdd->validate() != Wt::ValidationState::Valid) {
		return sendErrorToast(
			"Invalid email for new student."
		);
	}

	std::string firstName = firstNameAdd->text().toUTF8();
	boost::algorithm::trim(firstName);
	firstNameAdd->setText(firstName);

	if (firstName.length() < 1) {
		return sendErrorToast(
			"First name for new student cannot be empty."
		);
	}

	std::string lastName = lastNameAdd->text().toUTF8();
	boost::algorithm::trim(lastName);
	lastNameAdd->setText(lastName);

	if (lastName.length() < 1) {
		return sendErrorToast(
			"Last name for new student cannot be empty."
		);
	}

	std::string email = emailAdd->text().toUTF8();

	dbo::Transaction trans(session);

	auto &users = session.getUserDB();

	Wt::Auth::User userLookup = users.findWithIdentity(
		Wt::Auth::Identity::LoginName,
		email
	);

	int64_t facultyID = users.find(session.getLogin().user()).id();

	std::unique_ptr<ITermQuery> termQuery = std::make_unique<TermQuery>(
		session
	);
	auto term = *(termQuery->findByTime(
		std::chrono::system_clock::now()
	));
	int termID = term.id();

	using AdvisorPairings = dbo::collection<dbo::ptr<AdvisorPairing>>;

	if (userLookup.isValid()) {
		dbo::ptr<AuthInfo> authInfo = users.find(userLookup);
		dbo::ptr<Profile> profile = authInfo->user();
		if (profile->getRole() != UserRole::STUDENT) {
			return sendErrorToast(
				"Email address belong to non-student user."
			);
		}
		int64_t studentID = authInfo.id();
		AdvisorPairings pairings = session
			.find<AdvisorPairing>()
			.where("studentID = ?")
			.bind(studentID)
			.where("advisorID = ?")
			.bind(facultyID)
			;
		if (pairings.size() > 0) {
			return sendErrorToast(
				"Email address already associated with faculty."
			);
		}

		auto newPairing = std::make_unique<AdvisorPairing>();
		newPairing->setStudentID(studentID);
		newPairing->setAdvisorID(facultyID);
		newPairing->setTermID(termID);
		dbo::ptr<AdvisorPairing> pairingPtr = session.add(
			std::move(newPairing)
		);
		return;
	}

	Wt::Auth::User user = users.registerNew();
	user.addIdentity(Wt::Auth::Identity::LoginName, email);
	user.setEmail(email);
	std::string randomPassword = Wt::Auth::Utils::createSalt(16);
	session.getPasswordAuthService().updatePassword(
		user,
		randomPassword
	);

	dbo::ptr<AuthInfo> authInfo = users.find(user);

	dbo::ptr<Profile> profile = session.add(
		ProfileBuilder()
			.withFirstName(firstName)
			.withLastName(lastName)
			.withRole(UserRole::STUDENT)
			.buildUnique()
	);
	authInfo.modify()->setUser(profile);

	auto advisorPairing = std::make_unique<AdvisorPairing>();
	advisorPairing->setStudentID(authInfo.id());
	advisorPairing->setAdvisorID(facultyID);
	advisorPairing->setTermID(termID);
	dbo::ptr<AdvisorPairing> pairingPtr = session.add(
		std::move(advisorPairing)
	);
	addConfirm->setText("Student profile added.");
	changeConfirm->setText("");
	deleteConfirm->setText("");
}

SETUP_ROUTE("/faculty/manage_students", ManageStudentsView)
