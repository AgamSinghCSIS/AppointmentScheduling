#include <memory>
#include <utility>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <Wt/Auth/Identity.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WDate.h>
#include <Wt/WDateEdit.h>
#include <Wt/WDateTime.h>
#include <Wt/WDialog.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WTextArea.h>
#include <Wt/WTime.h>
#include <Wt/WTimeEdit.h>

#include "Appointment.h"
#include "AppointmentBuilder.h"
#include "MainApp.h"
#include "SetupRoute.h"
#include "StudentManageAppointmentsView.h"
#include "TimeUtils.h"
#include "UserRoleRedirectUtil.h"
#include "View.h"

namespace dbo = Wt::Dbo;

StudentManageAppointmentsView::StudentManageAppointmentsView(MainApp *app) :
    View(app),
    session(app->getSession())
{
    if (redirectFromStudent(app)) {
        return;
    }

    addTemplate();
    renderAppointments();
}

void StudentManageAppointmentsView::addTemplate() {
    templ = addWidget(
        std::make_unique<Wt::WTemplate>(
            Wt::WString::tr(
                "student-manage-appointments-template"
            )
        )
    );
    templ->setInternalPathEncoding(true);
}

void StudentManageAppointmentsView::renderAppointments() {
    appointmentContainer = templ->bindWidget(
        "appointment-container",
        std::make_unique<Wt::WContainerWidget>()
    );

    dbo::Transaction trans(session);

    UserDatabase &users = session.getUserDB();

    Wt::Auth::User currentUser = session.getLogin().user();

    dbo::ptr<AuthInfo> authInfo = users.find(currentUser);

    int64_t studentID = authInfo.id();

    using Appts = dbo::collection<dbo::ptr<Appointment>>;

    Appts appts = session
        .find<Appointment>()
        .where("studentID = ?")
        .bind(studentID)
        ;

    for (dbo::ptr<Appointment> &appt : appts) {
        appointmentContainer->addWidget(
            createAppointmentWidget(appt.id())
        );
    }
}

std::unique_ptr<Wt::WTemplate>
StudentManageAppointmentsView::createAppointmentWidget(
    int64_t apptID
) {
    UserDatabase &users = session.getUserDB();

    dbo::Transaction trans(session);

    using Appts = dbo::collection<dbo::ptr<Appointment>>;

    Appts appts = session
        .find<Appointment>()
        .where("id = ?")
        .bind(apptID)
        ;

    dbo::ptr<Appointment> appt = appts.front();

    auto apptTempl = std::make_unique<Wt::WTemplate>(
        Wt::WString::tr("student-appointment-partial")
    );

    std::string title = appt->getTitle();

    apptTempl->bindWidget(
        "title",
        std::make_unique<Wt::WText>(appt->getTitle())
    );

    std::stringstream ss;
    std::string facultyID;
    ss << appt->getFacultyID();
    ss >> facultyID;

    auto user = users.findWithId(facultyID);
    dbo::ptr<AuthInfo> authInfo = users.find(user);
    dbo::ptr<Profile> profile = authInfo->user();

    std::string advisorName
        = profile->getFirstName()
        + ", "
        + profile->getLastName()
        ;

    apptTempl->bindWidget(
        "advisor",
        std::make_unique<Wt::WText>(advisorName)
    );

    Wt::WDateTime startTime(appt->getStartTime());
    Wt::WDateTime endTime(appt->getEndTime());

    std::string format = "ddd MMM d yyyy hh:mm:ss";

    apptTempl->bindWidget(
        "start",
        std::make_unique<Wt::WText>(startTime.toString(format))
    );

    apptTempl->bindWidget(
        "end",
        std::make_unique<Wt::WText>(endTime.toString(format))
    );

    Wt::WPushButton *editButton = apptTempl->bindWidget(
        "edit-button",
        std::make_unique<Wt::WPushButton>("Edit")
    );

    editButton->clicked().connect([=, id=apptID] {
        dbo::Transaction trans(session);

        Appts appts = session
            .find<Appointment>()
            .where("id = ?")
            .bind(id)
            ;

        dbo::ptr<Appointment> appt = appts.front();

        Wt::WDialog *dialog = addChild(
            std::make_unique<Wt::WDialog>("Edit Appointment")
        );

        Wt::WTemplate *modalTempl = dialog->contents()->addWidget(
            std::make_unique<Wt::WTemplate>(
                Wt::WString::tr(
                    "student-appointment-edit-modal"
                )
            )
        );

        Wt::WDateEdit *startDateInput = modalTempl->bindWidget(
            "start-date-input",
            std::make_unique<Wt::WDateEdit>()
        );
        startDateInput->setDate(startTime.date());

        Wt::WTimeEdit *startTimeInput = modalTempl->bindWidget(
            "start-time-input",
            std::make_unique<Wt::WTimeEdit>()
        );
        startTimeInput->setTime(startTime.time());

        Wt::WDateEdit *endDateInput = modalTempl->bindWidget(
            "end-date-input",
            std::make_unique<Wt::WDateEdit>()
        );
        endDateInput->setDate(endTime.date());

        Wt::WTimeEdit *endTimeInput = modalTempl->bindWidget(
            "end-time-input",
            std::make_unique<Wt::WTimeEdit>()
        );
        endTimeInput->setTime(endTime.time());

        Wt::WLineEdit *titleInput = modalTempl->bindWidget(
            "title-input",
            std::make_unique<Wt::WLineEdit>()
        );
        titleInput->setText(appt->getTitle());

        Wt::WTextArea *descriptionInput = modalTempl->bindWidget(
            "description-input",
            std::make_unique<Wt::WTextArea>()
        );
        descriptionInput->setText(appt->getDescription());

        Wt::WPushButton *cancelButton = modalTempl->bindWidget(
            "cancel-button",
            std::make_unique<Wt::WPushButton>("Cancel")
        );

        cancelButton->clicked().connect(dialog, &Wt::WDialog::accept);

        Wt::WPushButton *saveButton = modalTempl->bindWidget(
            "save-button",
            std::make_unique<Wt::WPushButton>("Save")
        );

        saveButton->clicked().connect([=] {
            dbo::Transaction trans(session);

            Appts appts = session
                .find<Appointment>()
                .where("id = ?")
                .bind(id)
                ;

            dbo::ptr<Appointment> appt = appts.front();

            if (!startDateInput->date().isValid()) {
                return sendErrorToast(
                    "Invalid start date."
                );
            }
            if (!startTimeInput->time().isValid()) {
                return sendErrorToast(
                    "Invalid start time."
                );
            }
            Wt::WDateTime newStartTime(
                startDateInput->date(),
                startTimeInput->time()
            );

            if (!endDateInput->date().isValid()) {
                return sendErrorToast(
                    "Invalid end date."
                );
            }
            if (!endTimeInput->time().isValid()) {
                return sendErrorToast(
                    "Invalid end time."
                );
            }
            Wt::WDateTime newEndTime(
                endDateInput->date(),
                endTimeInput->time()
            );

            std::string newTitle = titleInput->text().toUTF8();
            boost::algorithm::trim(newTitle);
            titleInput->setText(newTitle);
            if (newTitle.length() > 30) {
                return sendErrorToast(
                    "Title must be less than or equal to 30 characters."
                );
            }
            if (newTitle.length() < 1) {
                return sendErrorToast(
                    "Title cannot be empty."
                );
            }

            std::string newDesc = descriptionInput->text().toUTF8();
            boost::algorithm::trim(newDesc);
            descriptionInput->setText(newDesc);
            if (newDesc.length() > 100) {
                return sendErrorToast(
                    "Description must be less than or equal to 100 characters."
                );
            }

            appt.modify()->setStartTime(newStartTime.toTimePoint());
            appt.modify()->setEndTime(newEndTime.toTimePoint());
            appt.modify()->setTitle(newTitle);
            appt.modify()->setDescription(newDesc);

            dialog->accept();

            trans.commit();

            renderAppointments();
        });

        dialog->show();
    });

    Wt::WPushButton *deleteButton = apptTempl->bindWidget(
        "delete-button",
        std::make_unique<Wt::WPushButton>("Delete")
    );

    deleteButton->clicked().connect([=, id=apptID] {
        dbo::Transaction trans(session);

        Appts appts = session
            .find<Appointment>()
            .where("id = ?")
            .bind(id)
            ;

        dbo::ptr<Appointment> appt = appts.front();

        appt.remove();
        trans.commit();
        renderAppointments();
    });

    return apptTempl;
}

void StudentManageAppointmentsView::sendErrorToast(std::string s) {
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

SETUP_ROUTE("/student/manage_appointments", StudentManageAppointmentsView)

