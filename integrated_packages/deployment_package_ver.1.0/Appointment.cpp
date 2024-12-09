//student id and faculty id should be unsigned
//Where is Action Class

#include <chrono>
#include <string>
#include <utility>

#include "Appointment.h"
#include "DatabaseUtils.h"

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

Appointment::Appointment():
    title(""),
    description(""),
    startTime(TimePoint::min()),
    endTime(TimePoint::max()),
    studentID(0),
    facultyID(0){}

std::string Appointment::getTitle() const {
    return title;
}

void Appointment::setTitle(std::string newTitle) {
    if (newTitle.length() >= 1 && newTitle.length() <= 30) 
        title = std::move(newTitle);
    else 
        std::cerr << "Error: Title must have a length in the range [1,30]."
                  << std::endl
                  ;
}

std::string Appointment::getDescription() const {
    return description;
}

void Appointment::setDescription(std::string newDescription) {
    if (newDescription.length() <= 100) 
        description = std::move(newDescription);
    else 
        std::cerr << "Error: Description must have a length in the "
                     "range [0,100]."
                  << std::endl
                  ;
}

TimePoint Appointment::getStartTime() const {
    return startTime;
}

void Appointment::setStartTime(TimePoint t) {
    startTime = t;
}

TimePoint Appointment::getEndTime() const {
    return endTime;
}

void Appointment::setEndTime(TimePoint t) {
    endTime = t;
}

int64_t Appointment::getStudentID() const {
    return studentID;
}

void Appointment::setStudentID(int64_t newStudentID) {
    studentID = std::move(newStudentID);
}

int64_t Appointment::getFacultyID() const {
    return facultyID;
}

void Appointment::setFacultyID(int64_t newFacultyID) {
    facultyID = std::move(newFacultyID);
}

CREATE_TABLE(Appointment, appointment)

