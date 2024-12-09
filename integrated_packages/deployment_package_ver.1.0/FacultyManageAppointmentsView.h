#ifndef FACULTY_MANAGE_APPOINTMENTS_VIEW_H
#define FACULTY_MANAGE_APPOINTMENTS_VIEW_H

#include <memory>
#include <vector>

#include <Wt/WContainerWidget.h>
#include <Wt/WDateEdit.h>
#include <Wt/WTemplate.h>

#include "Appointment.h"
#include "MainApp.h"
#include "Session.h"
#include "View.h"

class FacultyManageAppointmentsView : public View {
public:
    explicit FacultyManageAppointmentsView(MainApp*);
private:
    AASS::Session &session;

    Wt::WTemplate *templ;
    Wt::WContainerWidget *appointmentContainer;
    Wt::WPushButton *newAppointmentButton;

    void addTemplate();
    void addNewAppointmentButton();
    void renderAppointments();

    std::unique_ptr<Wt::WTemplate> createAppointmentWidget(int64_t);
    void sendErrorToast(std::string);
};

#endif // FACULTY_MANAGE_APPOINTMENTS_VIEW_H

