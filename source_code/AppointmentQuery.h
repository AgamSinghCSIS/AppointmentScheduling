#ifndef APPOINTMENT_QUERY_H
#define APPOINTMENT_QUERY_H

#include <memory>
#include <optional>

#include <Wt/Dbo/Dbo.h>

#include "Appointment.h"
#include "IAppointmentQuery.h"

class AppointmentQuery: public IAppointmentQuery {
private:
	Wt::Dbo::Session &session;
public:
	AppointmentQuery(Wt::Dbo::Session&);
	void addAppointment(std::unique_ptr<Appointment>);
	Wt::Dbo::collection<Wt::Dbo::ptr<Appointment>> findByStudent(int64_t);
	Wt::Dbo::collection<Wt::Dbo::ptr<Appointment>> findByFaculty(int64_t);
};

#endif // APPOINTMENT_QUERY_H
