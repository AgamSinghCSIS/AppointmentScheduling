#include "AppointmentQueryDummy.h"

#include <chrono>
#include <memory>
#include <optional>
#include <utility>

#include "Appointment.h"
#include "AppointmentBuilder.h"
#include "TimeUtils.h"

namespace dbo = Wt::Dbo;

AppointmentQueryDummy::AppointmentQueryDummy(dbo::Session &s) {}

void AppointmentQueryDummy::addAppointment(std::unique_ptr<Appointment> appt) {
	return;
}

dbo::collection<dbo::ptr<Appointment>> AppointmentQueryDummy::findByStudent(int64_t sID){
	dbo::collection<dbo::ptr<Appointment>> c;
	auto a = std::make_unique<Appointment>();
	a->setStudentID(sID);
	a->setStartTime(timePointFromStr("2024-01-15 08:00:00"));
	a->setEndTime(timePointFromStr("2024-01-15 09:00:00"));
	c.insert(dbo::ptr(std::move(a)));
    return c;
}

dbo::collection<dbo::ptr<Appointment>> AppointmentQueryDummy::findByFaculty(int64_t fID){
	dbo::collection<dbo::ptr<Appointment>> c;
	auto a = std::make_unique<Appointment>();
	a->setFacultyID(fID);
	a->setStartTime(timePointFromStr("2024-01-15 08:00:00"));
	a->setEndTime(timePointFromStr("2024-01-15 09:00:00"));
	c.insert(dbo::ptr(std::move(a)));
    return c;
}

