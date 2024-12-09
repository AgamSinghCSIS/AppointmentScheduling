#include <chrono>
#include <utility>
#include <memory>

#include "AppointmentBuilder.h"
#include "Appointment.h"

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

void AppointmentBuilder::reset() {
    title = "";
    description = "";
    startTime = TimePoint::min();
    endTime = TimePoint::max();
    studentID = 0;
    facultyID = 0;
}

AppointmentBuilder::AppointmentBuilder() {
    reset();
}

AppointmentBuilder& AppointmentBuilder::withTitle(std::string s) {
    title = std::move(s);
    return *this;
}

AppointmentBuilder& AppointmentBuilder::withDescription(std::string s) {
    description = std::move(s);
    return *this;
}

AppointmentBuilder& AppointmentBuilder::withStartTime(TimePoint t) {
    startTime = t;
    return *this;
}

AppointmentBuilder& AppointmentBuilder::withEndTime(TimePoint t) {
    endTime = t;
    return *this;
}

AppointmentBuilder& AppointmentBuilder::withStudentID(int64_t id) {
    studentID = id;
    return *this;
}

AppointmentBuilder& AppointmentBuilder::withFacultyID(int64_t id) {
    facultyID = id;
    return *this;
}

Appointment AppointmentBuilder::build() {
    Appointment a;
    a.setTitle(std::move(title));
    a.setDescription(std::move(description));
    a.setStartTime(startTime);
    a.setEndTime(endTime);
    a.setFacultyID(facultyID);
    a.setStudentID(studentID);
    reset();
    return a;
}

std::unique_ptr<Appointment> AppointmentBuilder::buildUnique() {
    auto a = std::make_unique<Appointment>();
    a->setTitle(std::move(title));
    a->setDescription(std::move(description));
    a->setStartTime(startTime);
    a->setEndTime(endTime);
    a->setFacultyID(facultyID);
    a->setStudentID(studentID);
    reset();
    return a;
}
