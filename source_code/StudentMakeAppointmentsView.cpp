#include <iostream>
#include <string>
#include <sstream>

#include <memory>
#include <utility>
#include <stdexcept>
#include "AdvisorPairing.h"
#include "Profile.h"

#include <Wt/WCalendar.h>
#include <Wt/WComboBox.h>
#include <Wt/WDate.h>
#include <Wt/WDateTime.h>
#include <Wt/WTime.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTimeEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/Dbo/Dbo.h>

#include "TermQuery.h"
#include "Session.h"
#include "MainApp.h"
#include "AppointmentBuilder.h"
#include "StudentMakeAppointmentsView.h"
#include "UserRoleRedirectUtil.h"
#include "SetupRoute.h"

StudentMakeAppointmentsView::StudentMakeAppointmentsView(MainApp *app) : View(app) {
    addTemplate();
    getStuID();
    addBackButton();
    addAdvisorSelection();
    addCalendar();
    addAvailability();
    addApptTitle();
    addApptDescription();
    addStartApptTime();
    addEndApptTime();
    addBookButton();
    addApptDetails();
}

void StudentMakeAppointmentsView::addTemplate() {
    templ = addWidget(
        std::make_unique<Wt::WTemplate>(
        Wt::WString::tr("student-make-appointments-template")
        )
    );
    templ->setInternalPathEncoding(true);
}

void StudentMakeAppointmentsView::getStuID() {
    AASS::Session &session = parent->getSession();
    UserDatabase &users = dynamic_cast<UserDatabase&>(
        session.getUsers()
    );
    Wt::Dbo::ptr<AuthInfo> authInfo = users.find(
        session.getLogin().user()
    );
    Wt::Dbo::ptr<Profile> profile = authInfo->user();
    stuID = profile.id(); 
}

void StudentMakeAppointmentsView::addAdvisorSelection() {
    AASS::Session &session = parent->getSession();
    Wt::Dbo::Transaction trans(session);
    int64_t advisorID;
    std::string advisorName;

    advisor = templ->bindWidget("advisor-selection",
        std::make_unique<Wt::WComboBox>()
    ); 

    using AdvisorPairings = Wt::Dbo::collection<
        Wt::Dbo::ptr<AdvisorPairing>>;
    AdvisorPairings pairings = session
        .find<AdvisorPairing>()
        .where("studentID = ?")
        .bind(stuID)
        ;
 
    for (auto &pairing : pairings) {
        advisorID = pairing->getAdvisorID();
        Wt::Dbo::collection<Wt::Dbo::ptr<AuthInfo>> authInfos = session
            .find<AuthInfo>()
            .where("user_id = ?")
            .bind(advisorID)
            ;

        auto authInfo = authInfos.front();
        Wt::Dbo::ptr<Profile> profile = authInfo->user();
        advisorName = profile->getLastName();
        advisor->addItem(advisorName);
        advisorMap[advisorName] = advisorID;
    }
    advisor->setCurrentIndex(0);
}

void StudentMakeAppointmentsView::addCalendar() {
    Wt::WDate today = Wt::WDate::currentDate();
    calendar = templ->bindWidget("make-appointment-calendar",
        std::make_unique<Wt::WCalendar>()
    );
    calendar->browseTo(today);
    calendar->clicked().connect(this,
        &StudentMakeAppointmentsView::updateAvailability
    );
}

void StudentMakeAppointmentsView::addAvailability() {
    availability = templ->bindWidget(
        "available-appointment-times",
        std::make_unique<Wt::WText>("Please select a date"
        " to view availabilites.")
    );
}

void StudentMakeAppointmentsView::addStartApptTime() {
    startApptTime = templ->bindWidget(
        "start-appointment-time",
        std::make_unique<Wt::WTimeEdit>()
    );
    startApptTime->setFormat("HH:mm");
    startApptTime->setHourStep(1);
    startApptTime->setMinuteStep(1);
    startApptTime->setTime(Wt::WTime(0,0));
}

void StudentMakeAppointmentsView::addEndApptTime() {
    endApptTime = templ->bindWidget(
        "end-appointment-time",
        std::make_unique<Wt::WTimeEdit>()
    );
    endApptTime->setFormat("HH:mm");
    endApptTime->setHourStep(1);
    endApptTime->setMinuteStep(1);
    endApptTime->setTime(Wt::WTime(0,0));
}


void StudentMakeAppointmentsView::addApptTitle() {
    apptTitle = templ->bindWidget(
        "appointment-title",
        std::make_unique<Wt::WLineEdit>()
    );
}

void StudentMakeAppointmentsView::addApptDescription() {
    apptDescription = templ->bindWidget(
        "appointment-description",
        std::make_unique<Wt::WLineEdit>()
    );
}


void StudentMakeAppointmentsView::addApptDetails() {
    apptDetails = templ->bindWidget(
        "appointment-details",
        std::make_unique<Wt::WText>()
    );
}

void StudentMakeAppointmentsView::addBookButton() {
    bookButton = templ->bindWidget(
        "book-appointment-button",
        std::make_unique<Wt::WPushButton>("Book")
    );

    bookButton->clicked().connect(this,
        &StudentMakeAppointmentsView::updateApptDetails
    );
}


void StudentMakeAppointmentsView::addBackButton() {
    backButton = templ->bindWidget(
        "back-button",
        std::make_unique<Wt::WPushButton>("Back")
    );
    backButton->setLink(
        Wt::WLink(Wt::LinkType::InternalPath,
        "/student")
    );
}

void StudentMakeAppointmentsView::updateAvailability() {
    std::string times;
    AASS::Session &session = parent->getSession();
    Wt::Dbo::Transaction trans(session);
    TermQuery termQuery(session);
    Wt::WDate Wdate = *(calendar->selection().begin());
    TimePoint TPdate = Wt::WDateTime(Wdate).toTimePoint();
    auto inTerm = termQuery.findByTime(TPdate);
    if (!inTerm.has_value()){
        times = "Date selected is not within a valid term.";
        availability->setText(times);
        return;
    }
    times = "<div>one availability window"
        "</div><div>"
        "two availability windows"
        "</div><div>"
        "three availability windows"
        "</div><div>"
        "a bunch of availability windows"
        "</div>";
    //TODO - need faculty availability in db to grab
    availability->setText(times);
}

void StudentMakeAppointmentsView::updateApptDetails() {
    std::string details;
    if (!isApptValid()) {
        details = "Failed to create appointment:"
            " invalid or empty input(s).";
        apptDetails->setText(details);
        return;
    }
    createAppointment();
    Wt::WDate date  = *(calendar->selection().begin());
    Wt::WTime start = startApptTime->time();
    Wt::WTime end   = endApptTime->time();
    details = 
        "<div>Appointment created!"
        "</div><div>"
        "Title: " + apptTitle->text().toUTF8() +
        "</div><div>"
        "Description: " + apptDescription->text().toUTF8() +
        "</div><div>"
        "Advisor: " + advisor->currentText().toUTF8() +
        "</div><div>"
        "Date: " + date.toString("yyyy/MM/dd").toUTF8() +
        "</div><div>"
        "Start: " + start.toString("HH:mm").toUTF8() +
        "</div><div>"
        "End: " + end.toString("HH:mm").toUTF8() +
        "</div>";
    apptDetails->setText(details);
    
    calendar->clearSelection();
    endApptTime->setTime(Wt::WTime(0, 0));
    startApptTime->setTime(Wt::WTime(0, 0));
    apptTitle->setText("");
    apptDescription->setText("");
    availability->setText("");
}

bool StudentMakeAppointmentsView::isApptValid() {
    bool valid = true;
    AASS::Session &session = parent->getSession();
    Wt::Dbo::Transaction trans(session);
    TermQuery termQuery(session);
    Wt::WDate Wdate = *(calendar->selection().begin());
    TimePoint TPdate = Wt::WDateTime(Wdate).toTimePoint();
    auto inTerm = termQuery.findByTime(TPdate);
    Wt::WTime start = startApptTime->time();
    Wt::WTime end = endApptTime->time();
    Wt::WDate today = Wt::WDate::currentDate();

    if (!start.isValid() ||
        !end.isValid()) {
        valid = false;
    } else if (Wdate < today) {
        valid = false;
    } else if (end <= start) {
        valid = false;
    } else if (calendar->selection().empty()) {
        valid = false;
    } else if (apptTitle->text() == "") {
        valid = false;
    } else if (apptDescription->text() == "") {
        valid = false;
    } else if (!inTerm.has_value()) {
        valid = false;
    } //TODO else if not in available appt window
    
    return valid;
}

void StudentMakeAppointmentsView::createAppointment() {
    AASS::Session &session = parent->getSession();
    Wt::Dbo::Transaction trans(session);
    std::string advisorName = advisor->currentText().toUTF8();
    int64_t advisorID = advisorMap[advisorName];
    Wt::WDate date = *(calendar->selection().begin());
    TimePoint start = Wt::WDateTime(date,
        startApptTime->time()).toTimePoint();
    TimePoint end   = Wt::WDateTime(date,
        endApptTime->time()).toTimePoint();
    AppointmentBuilder apptBuilder;
    
    apptBuilder.withTitle(apptTitle->text().toUTF8());
    apptBuilder.withDescription(apptDescription->text().toUTF8());
    apptBuilder.withStartTime(start);
    apptBuilder.withEndTime(end);
    apptBuilder.withStudentID(stuID);
    apptBuilder.withFacultyID(advisorID);
    Wt::Dbo::ptr<Appointment> apptPtr = session.add(apptBuilder.buildUnique());
}

SETUP_ROUTE("/make_appointment", StudentMakeAppointmentsView)
