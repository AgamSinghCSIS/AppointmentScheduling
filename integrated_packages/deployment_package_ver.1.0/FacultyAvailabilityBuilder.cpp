#include <chrono>
#include <utility>
#include <memory>

#include "FacultyAvailabilityBuilder.h"
#include "FacultyAvailability.h"

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

void FacultyAvailabilityBuilder::reset() {
    startTime = TimePoint::min();
    endTime = TimePoint::max();
    facultyID = 0;
}

FacultyAvailabilityBuilder& FacultyAvailabilityBuilder::withStartTime(
    TimePoint t
) {
    startTime = t;
    return *this;
}

FacultyAvailabilityBuilder& FacultyAvailabilityBuilder::withEndTime(
    TimePoint t
) {
    endTime = t;
    return *this;
}

FacultyAvailabilityBuilder& FacultyAvailabilityBuilder::withFacultyID(
    int64_t id
) {
    facultyID = id;
    return *this;
}

FacultyAvailability FacultyAvailabilityBuilder::build() {
    FacultyAvailability a;
    a.setStartTime(startTime);
    a.setEndTime(endTime);
    reset();
    return a;
}

std::unique_ptr<FacultyAvailability> FacultyAvailabilityBuilder::buildUnique() {
    auto a = std::make_unique<FacultyAvailability>();
    a->setStartTime(startTime);
    a->setEndTime(endTime);
    a->setFacultyID(facultyID);
    reset();
    return a;
}

