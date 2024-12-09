#ifndef APPOINTMENT_QUERY_DUMMY_H
#define APPOINTMENT_QUERY_DUMMY_H

#include <chrono>
#include <memory>
#include <optional>

#include "IAppointmentQuery.h"
#include "Appointment.h"

class AppointmentQueryDummy: public IAppointmentQuery {
public:
	AppointmentQueryDummy(Wt::Dbo::Session&);
	void addAppointment(std::unique_ptr<Appointment>);
	Wt::Dbo::collection<Wt::Dbo::ptr<Appointment>> findByStudent(int64_t);
	Wt::Dbo::collection<Wt::Dbo::ptr<Appointment>> findByFaculty(int64_t);
};

#endif // APPOINTMENT_QUERY_DUMMY_H
