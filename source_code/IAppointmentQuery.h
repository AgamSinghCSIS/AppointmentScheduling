#ifndef I_APPOINTMENT_QUERY_H
#define I_APPOINTMENT_QUERY_H

#include <chrono>
#include <memory>
#include <optional>

#include <Wt/Dbo/Dbo.h>

#include "Appointment.h"

class IAppointmentQuery {
public:
	virtual void addAppointment(std::unique_ptr<Appointment>) = 0;
	virtual Wt::Dbo::collection<Wt::Dbo::ptr<Appointment>> findByStudent(int64_t) = 0;
	virtual Wt::Dbo::collection<Wt::Dbo::ptr<Appointment>> findByFaculty(int64_t) = 0;
};

#endif // I_APPOINTMENT_QUERY_H
