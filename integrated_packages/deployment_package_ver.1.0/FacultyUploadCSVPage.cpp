#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include <Wt/Auth/AuthUtils.h>
#include <Wt/Auth/Identity.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/WEmailValidator.h>
#include <Wt/WFileUpload.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>

#include "AdvisorPairing.h"
#include "FacultyUploadCSVPage.h"
#include "MainApp.h"
#include "Profile.h"
#include "ProfileBuilder.h"
#include "Session.h"
#include "SetupRoute.h"
#include "Term.h"
#include "TermQuery.h"
#include "UserRoleRedirectUtil.h"

namespace dbo = Wt::Dbo;

FacultyUploadCSVPage::FacultyUploadCSVPage(MainApp *app) :
    View(app),
    session(app->getSession()) 
{
	addTemplate();
	addUploadStudentsButton();
	addUploadScheduleButton();
}

void FacultyUploadCSVPage::addTemplate(){
	templ = addWidget(
		std::make_unique<Wt::WTemplate>(
			Wt::WString::tr("faculty-upload-csv-template")
		)
	);
	templ->setInternalPathEncoding(true);
}

void FacultyUploadCSVPage::addUploadStudentsButton() {
	uploadStudentsButton = templ->bindWidget(
		"upload-students-button",
		std::make_unique<Wt::WFileUpload>()
	);
    uploadStudentsButton->setFilters(".csv");

    uploadStudentsButton->changed().connect(
        uploadStudentsButton,
        &Wt::WFileUpload::upload
    );

    uploadStudentsButton->uploaded().connect([=] {
        std::string fileName = uploadStudentsButton->spoolFileName();
        std::ifstream inFile(fileName);
        using Tokenizer = boost::tokenizer<boost::escaped_list_separator<char>>;

        std::string line;
        uint64_t lineNum = 0;
        Wt::WEmailValidator emailValidator;

        dbo::Transaction trans(session);

        UserDatabase &users = session.getUserDB();

        TermQuery termQuery(session);
        auto currentTerm = termQuery.findByTime(
            std::chrono::system_clock::now()
        );
        int64_t termID = 0;
        if (currentTerm) {
            termID = (*currentTerm).id();
        }

        Wt::Auth::User currentUser = session.getLogin().user();
        int64_t facultyID = users.find(currentUser).id();

        while (std::getline(inFile, line)) {
            ++lineNum;
            Tokenizer tokenizer(line);

            std::vector<std::string> tokens;
            for (const auto &token : tokenizer) {
                tokens.push_back(token);
            }

            if (tokens[0] == "first" && tokens[1] == "last" && tokens[2] == "email") {
                continue;
            }

            if (tokens.size() == 3) {
                std::string studentFirstName =  std::move(tokens[0]);
                boost::algorithm::trim(studentFirstName);
                if (studentFirstName.length() < 1) {
                    std::stringstream ss;
                    ss << "Empty first name on line ";
                    ss << lineNum;
                    sendErrorToast(
                        ss.str()
                    );
                    continue;
                }
                std::string studentLastName =  std::move(tokens[1]);
                boost::algorithm::trim(studentLastName);
                if (studentLastName.length() < 1) {
                    std::stringstream ss;
                    ss << "Empty last name on line ";
                    ss << lineNum;
                    sendErrorToast(
                        ss.str()
                    );
                    continue;
                }
                std::string studentEmail =  std::move(tokens[2]);
                boost::algorithm::trim(studentEmail);
                if (studentEmail.length() < 1) {
                    std::stringstream ss;
                    ss << "Empty email on line ";
                    ss << lineNum;
                    sendErrorToast(
                        ss.str()
                    );
                    continue;
                }
                if (emailValidator.validate(studentEmail).state()
                    != Wt::ValidationState::Valid
                ) {
                    std::stringstream ss;
                    ss << "Invalid email on line ";
                    ss << lineNum;
                    sendErrorToast(
                        ss.str()
                    );
                    continue;
                }

                Wt::Auth::User user = users.findWithEmail(studentEmail);

                if (user.isValid()) {
                    dbo::ptr<AuthInfo> authInfo = users.find(user);
                    dbo::ptr<Profile> profile = authInfo->user();
                    if (profile->getRole() != UserRole::STUDENT) {
                        std::stringstream ss;
                        ss << "Email belong to non-student user on line ";
                        ss << lineNum;
                        sendErrorToast(
                            ss.str()
                        );
                        continue;
                    }

                    int64_t studentID = authInfo.id();

                    using Pairings = dbo::collection<dbo::ptr<AdvisorPairing>>;
                    Pairings pairings = session
                        .find<AdvisorPairing>()
                        .where("advisorID = ?")
                        .bind(facultyID)
                        .where("studentID = ?")
                        .bind(studentID)
                        ;

                    if (pairings.size() > 0) {
                        std::stringstream ss;
                        ss << "Student already associated with advisor on ";
                        ss << "line: ";
                        ss << lineNum;
                        sendErrorToast(
                            ss.str()
                        );
                        continue;
                    }

                    auto pairing = std::make_unique<AdvisorPairing>();
                    pairing->setStudentID(studentID);
                    pairing->setAdvisorID(facultyID);
                    pairing->setTermID(termID);
                    dbo::ptr<AdvisorPairing> pairingPtr = session.add(
                        std::move(pairing)
                    );
                    continue;
                }

                user  = users.registerNew();
                user.addIdentity(Wt::Auth::Identity::LoginName, studentEmail);
                user.setEmail(studentEmail);
                std::string randomPassword = Wt::Auth::Utils::createSalt(16);
                session.getPasswordAuthService().updatePassword(
                    user,
                    randomPassword
                );

                dbo::ptr<AuthInfo> authInfo = users.find(user);

                dbo::ptr<Profile> profile = session.add(
                    ProfileBuilder()
                        .withFirstName(studentFirstName)
                        .withLastName(studentLastName)
                        .withRole(UserRole::STUDENT)
                        .buildUnique()
                );
                authInfo.modify()->setUser(profile); 

                int64_t studentID = authInfo.id();

                auto pairing = std::make_unique<AdvisorPairing>();
                pairing->setStudentID(studentID);
                pairing->setAdvisorID(facultyID);
                pairing->setTermID(termID);
                dbo::ptr<AdvisorPairing> pairingPtr = session.add(
                    std::move(pairing)
                );
            }
        }
    });

    uploadStudentsButton->fileTooLarge().connect([=] {
        sendErrorToast(
            "Uploaded student CSV file was too large."
        );
        return;
    });
}

void FacultyUploadCSVPage::addUploadScheduleButton() {
	uploadScheduleButton = templ->bindWidget(
		"upload-schedule-button",
		std::make_unique<Wt::WFileUpload>()
	);
    uploadScheduleButton->setFilters(".csv");

	// TODO button functionality
	// Open file explorer to select .csv file
	// Use faculty availability builder
	// based on data from .csv file
    uploadScheduleButton->changed().connect([=] {
        sendErrorToast(
            "Faculty availability is not implemented."
        );
    });
}

void FacultyUploadCSVPage::sendErrorToast(std::string s) {
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
    /*
    toastTempl->doJavaScript(
        "let containerEl = document.querySelector("
        "  '#" + toastTempl->id() + "'"
        ");"
        "let toastElList = [].slice.call("
        "  containerEl.querySelectorAll('.toast')"
        ");"
        "let toastList = toastElList.map((toastEl) => {"
        "  return new bootstrap.Toast(toastEl);"
        "});"
        "toastList.forEach(toast => toast.show());"
    );
    */
    // Removing variable declarations to prevent naming collisions.
    toastTempl->doJavaScript(
        "[].slice.call(document.querySelector('#" + toastTempl->id() + "').querySelectorAll('.toast')).map((toastEl) => new bootstrap.Toast(toastEl)).forEach(toast => toast.show());"
    );
}

SETUP_ROUTE("/faculty/uploadcsv", FacultyUploadCSVPage)
