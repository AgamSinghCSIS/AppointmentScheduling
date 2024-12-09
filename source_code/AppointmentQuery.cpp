#include <memory>
#include <utility>
#include <stdexcept>

#include <Wt/Dbo/Dbo.h>

#include "Appointment.h"
#include "AppointmentQuery.h"

namespace dbo = Wt::Dbo;

AppointmentQuery::AppointmentQuery(dbo::Session &s) : session(s) {}

void AppointmentQuery::addAppointment(std::unique_ptr<Appointment> appt) {
	session.add(std::move(appt));
}

dbo::collection<dbo::ptr<Appointment>> AppointmentQuery::findByStudent(int64_t sID) {
	using Appointments = dbo::collection<dbo::ptr<Appointment>>;

	Appointments appointments = session
		.find<Appointment>()
		.where("studentID = ?")
		.bind(sID);
	
	if (appointments.size() == 0) {
		return {};
	}
	
	return appointments;
}

dbo::collection<dbo::ptr<Appointment>> AppointmentQuery::findByFaculty(int64_t fID) {
	using Appointments = dbo::collection<dbo::ptr<Appointment>>;

	Appointments appointments = session
		.find<Appointment>()
		.where("facultyID = ?")
		.bind(fID);

	if (appointments.size() == 0) {
		return {};
	}
	
	return appointments;
}	
