#include <boost/algorithm/string.hpp>

#include <Wt/Auth/AuthUtils.h>
#include <Wt/Auth/Identity.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/WEmailValidator.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>

#include "AdminManageFacultyView.h"
#include "AdvisorPairing.h"
#include "MainApp.h"
#include "Profile.h"
#include "ProfileBuilder.h"
#include "Session.h"
#include "SetupRoute.h"
#include "UserRoleRedirectUtil.h"

namespace dbo = Wt::Dbo;

AdminManageFacultyView::AdminManageFacultyView(MainApp *app) :
    View(app),
    session(app->getSession())
{
    if (redirectFromAdmin(parent)) {
        return;
    }

    addTemplate();
    addSearchInput();
    addSearchButton();
    addFirstNameEdit();
    addLastNameEdit();
    addEmailEdit();
    addSaveChangesButton();
    addDeleteFacultyButton();
    addFirstNameEntry();
    addLastNameEntry();
    addEmailEntry();
    addAddFacultyButton();
}

void AdminManageFacultyView::addTemplate() {
    templ = addWidget(
        std::make_unique<Wt::WTemplate>(
            Wt::WString::tr("admin-manage-faculty-template")
        )
    );
    templ->setInternalPathEncoding(true);
}

void AdminManageFacultyView::addSearchInput() {
    searchInput = templ->bindWidget(
        "search-input",
        std::make_unique<Wt::WLineEdit>()
    );
    searchInput->setValidator(std::make_unique<Wt::WEmailValidator>());
}

void AdminManageFacultyView::addSearchButton() {
    searchButton = templ->bindWidget(
        "search-button",
        std::make_unique<Wt::WPushButton>("Search")
    );

    searchButton->clicked().connect(
        this,
        &AdminManageFacultyView::searchUserEventHandler
    );
}

void AdminManageFacultyView::searchUserEventHandler() {
    if (searchInput->validate() != Wt::ValidationState::Valid) {
        return;
    }

    dbo::Transaction trans(session);

    auto errorReturn = [=] {
        resetEditInputs();
        userToEdit = Wt::Auth::User();
        sendErrorToast(
            "Unable to find faculty member based on given email."
        );
    };

    auto &users = session.getUserDB();

    std::string emailToFind = searchInput->text().toUTF8();

    Wt::Auth::User userLookup = users.findWithIdentity(
        Wt::Auth::Identity::LoginName,
        emailToFind
    );

    if (!userLookup.isValid()) {
        return errorReturn();
    }

    dbo::ptr<AuthInfo> authInfo = users.find(userLookup);
    dbo::ptr<Profile> profile = authInfo->user();

    if (profile->getRole() != UserRole::FACULTY) {
        return errorReturn();
    }
    userToEdit = userLookup;

    firstNameEdit->setText(profile->getFirstName());
    lastNameEdit->setText(profile->getLastName());
    emailEdit->setText(emailToFind);
}

void AdminManageFacultyView::sendErrorToast(std::string s) {
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

void AdminManageFacultyView::addFirstNameEdit() {
    firstNameEdit = templ->bindWidget(
        "first-name-edit",
        std::make_unique<Wt::WLineEdit>()
    );
}

void AdminManageFacultyView::addLastNameEdit() {
    lastNameEdit = templ->bindWidget(
        "last-name-edit",
        std::make_unique<Wt::WLineEdit>()
    );
}

void AdminManageFacultyView::addEmailEdit() {
    emailEdit = templ->bindWidget(
        "email-edit",
        std::make_unique<Wt::WLineEdit>()
    );
    emailEdit->setValidator(std::make_unique<Wt::WEmailValidator>());
}

void AdminManageFacultyView::addSaveChangesButton() {
    saveChangesButton = templ->bindWidget(
        "save-changes-button",
        std::make_unique<Wt::WPushButton>("Save Changes")
    );

    saveChangesButton->clicked().connect(
        this,
        &AdminManageFacultyView::saveChangesEventHandler
    );
}

void AdminManageFacultyView::saveChangesEventHandler() {
    dbo::Transaction trans(session);

    UserDatabase &users = session.getUserDB();

    if (!userToEdit.isValid()) {
        sendErrorToast(
            "No selected user to edit."
        );
        return;
    }

    if (emailEdit->validate() != Wt::ValidationState::Valid) {
        sendErrorToast(
            "Cannot give a user an invalid email."
        );
        return;
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
}

void AdminManageFacultyView::addDeleteFacultyButton() {
    deleteFacultyButton = templ->bindWidget(
        "delete-faculty-button",
        std::make_unique<Wt::WPushButton>("Delete Member")
    );

    deleteFacultyButton->clicked().connect(
        this,
        &AdminManageFacultyView::deleteUserEventHandler
    );
}

void AdminManageFacultyView::deleteUserEventHandler() {
    dbo::Transaction trans(session);

    UserDatabase &users = session.getUserDB();

    if (!userToEdit.isValid()) {
        sendErrorToast(
            "No selected user to delete."
        );
    }

    dbo::ptr<AuthInfo> authInfo = users.find(userToEdit);
    dbo::ptr<Profile> profile = authInfo->user();

    using AdvisorPairings = dbo::collection<dbo::ptr<AdvisorPairing>>;

    AdvisorPairings pairings = session
        .find<AdvisorPairing>()
        .where("advisorID = ?")
        .bind(authInfo.id())
        ;

    for (auto &pairing : pairings) {
        pairing.remove();
    }

    users.deleteUser(userToEdit);

    userToEdit = Wt::Auth::User();
    resetEditInputs();
}

void AdminManageFacultyView::addFirstNameEntry() {
    firstNameEntry = templ->bindWidget(
        "first-name-add",
        std::make_unique<Wt::WLineEdit>()
    );
}

void AdminManageFacultyView::addLastNameEntry() {
    lastNameEntry = templ->bindWidget(
        "last-name-add",
        std::make_unique<Wt::WLineEdit>()
    );
}

void AdminManageFacultyView::addEmailEntry() {
    emailEntry = templ->bindWidget(
        "email-add",
        std::make_unique<Wt::WLineEdit>()
    );
    emailEntry->setValidator(std::make_unique<Wt::WEmailValidator>());
}

void AdminManageFacultyView::addAddFacultyButton() {
    addFacultyButton = templ->bindWidget(
        "add-faculty-button",
        std::make_unique<Wt::WPushButton>("Add Faculty")
    );

    addFacultyButton->clicked().connect(
        this,
        &AdminManageFacultyView::addUserEventHandler
    );
}

void AdminManageFacultyView::addUserEventHandler() {
    if (emailEntry->validate() != Wt::ValidationState::Valid) {
        return sendErrorToast(
            "Invalid email for new faculty member."
        );
    }

    std::string firstName = firstNameEntry->text().toUTF8();
    boost::algorithm::trim(firstName);
    firstNameEntry->setText(firstName);

    if (firstName.length() < 1) {
        return sendErrorToast(
            "First name for new faculty member cannot be empty."
        );
    }

    std::string lastName = lastNameEntry->text().toUTF8();
    boost::algorithm::trim(lastName);
    lastNameEntry->setText(lastName);

    if (lastName.length() < 1) {
        return sendErrorToast(
            "Last name for new faculty member cannot be empty."
        );
    }

    std::string email = emailEntry->text().toUTF8();

    dbo::Transaction trans(session);

    auto &users = session.getUserDB();

    Wt::Auth::User userLookup = users.findWithIdentity(
        Wt::Auth::Identity::LoginName,
        email 
    );

    if (userLookup.isValid()) {
        return sendErrorToast(
            "Email address already exists."
        );
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
            .withRole(UserRole::FACULTY)
            .buildUnique()
    );
    authInfo.modify()->setUser(profile);
}

void AdminManageFacultyView::resetEditInputs() {
    firstNameEdit->setText("");
    lastNameEdit->setText("");
    emailEdit->setText("");
    searchInput->setText("");
}

SETUP_ROUTE("/admin/manage_faculty", AdminManageFacultyView)

