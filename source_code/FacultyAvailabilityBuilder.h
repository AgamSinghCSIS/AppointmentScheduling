#ifndef FACULTY_AVAILABILITY_BUILDER_H
#define FACULTY_AVAILABILITY_BUILDER_H

#include <chrono>
#include <memory>
#include <utility>

#include "FacultyAvailability.h"

class FacultyAvailabilityBuilder {
private:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    TimePoint startTime;
    TimePoint endTime;
    int64_t facultyID;

    void reset();
public:
    FacultyAvailabilityBuilder();
    FacultyAvailabilityBuilder& withStartTime(TimePoint);
    FacultyAvailabilityBuilder& withEndTime(TimePoint);
    FacultyAvailabilityBuilder& withFacultyID(int64_t);
    FacultyAvailability build();
    std::unique_ptr<FacultyAvailability> buildUnique();
};

#endif // FACULTY_AVAILABILITY_BUILDER_H
