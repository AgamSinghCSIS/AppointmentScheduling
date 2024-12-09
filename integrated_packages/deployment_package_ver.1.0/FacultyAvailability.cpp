#include <chrono>
#include <string>
#include <utility>

#include "DatabaseUtils.h"
#include "FacultyAvailability.h"

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

FacultyAvailability::FacultyAvailability():
    facultyID(0),
    startTime(TimePoint::min()),
    endTime(TimePoint::max()) {}

TimePoint FacultyAvailability::getStartTime() const {
    return startTime;
}

void FacultyAvailability::setStartTime(TimePoint t) {
    startTime = t;
}

TimePoint FacultyAvailability::getEndTime() const {
    return endTime;
}

void FacultyAvailability::setEndTime(TimePoint t) {
    endTime = t;
}

int64_t FacultyAvailability::getFacultyID() const {
    return facultyID;
}

void FacultyAvailability::setFacultyID(int64_t newFacultyID) {
    facultyID = std::move(newFacultyID);
}

CREATE_TABLE(FacultyAvailability, faculty_availability)

