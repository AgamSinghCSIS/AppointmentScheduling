//didn't included the ID part

#ifndef APPOINTMENT_BUILDER_H
#define APPOINTMENT_BUILDER_H

#include <chrono>
#include <memory>
#include <utility>

#include "Appointment.h"

class AppointmentBuilder{
private:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    std::string title;
    std::string description;
    TimePoint startTime;
    TimePoint endTime;
    int64_t studentID;
    int64_t facultyID;

    void reset();

public:
    AppointmentBuilder();
    AppointmentBuilder& withTitle(std::string);
    AppointmentBuilder& withDescription(std::string);
    AppointmentBuilder& withStartTime(TimePoint);
    AppointmentBuilder& withEndTime(TimePoint);
    AppointmentBuilder& withStudentID(int64_t);
    AppointmentBuilder& withFacultyID(int64_t);
    Appointment build();
    std::unique_ptr<Appointment> buildUnique();
};

#endif
