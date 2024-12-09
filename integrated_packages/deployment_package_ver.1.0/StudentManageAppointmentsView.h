#ifndef STUDENT_MANAGE_APPOINTMENTS_VIEW_H
#define STUDENT_MANAGE_APPOINTMENTS_VIEW_H

#include <memory>
#include <vector>

#include <Wt/WContainerWidget.h>
#include <Wt/WDateEdit.h>
#include <Wt/WTemplate.h>

#include "Appointment.h"
#include "MainApp.h"
#include "Session.h"
#include "View.h"

class StudentManageAppointmentsView : public View {
public:
    explicit StudentManageAppointmentsView(MainApp*);
private:
    AASS::Session &session;

    Wt::WTemplate *templ;
    Wt::WContainerWidget *appointmentContainer;

    void addTemplate();
    void renderAppointments();

    std::unique_ptr<Wt::WTemplate> createAppointmentWidget(int64_t);
    void sendErrorToast(std::string);
};

#endif // STUDENT_MANAGE_APPOINTMENTS_VIEW_H

