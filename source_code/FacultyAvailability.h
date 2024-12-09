#ifndef FACULTY_AVAILABILITY_H
#define FACULTY_AVAILABILITY_H

#include <chrono>
#include <string>

#include <Wt/Dbo/Dbo.h>

class FacultyAvailability {
private:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    int64_t facultyID;
    TimePoint startTime;
    TimePoint endTime;
public:
    FacultyAvailability();

    int64_t getFacultyID() const;
    void setFacultyID(int64_t);

    TimePoint getStartTime() const;
    void setStartTime(TimePoint);

    TimePoint getEndTime() const;
    void setEndTime(TimePoint);

    template<typename Action>
    void persist(Action&);
};

template <typename Action>
void FacultyAvailability::persist(Action &a){
    namespace dbo = Wt::Dbo;
    dbo::field(a, facultyID, "facultyID");
    dbo::field(a, startTime, "startTime");
    dbo::field(a, endTime, "endTime");
}

#endif

