#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <chrono>
#include <string>

#include <Wt/Dbo/Dbo.h>

class Appointment{
private:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    std::string title;
    std::string description;
    TimePoint startTime;
    TimePoint endTime;
    int64_t studentID;
    int64_t facultyID;

public:
    Appointment();

    std::string getTitle() const;
    void setTitle(const std::string);

    std::string getDescription() const;
    void setDescription(const std::string);

    TimePoint getStartTime() const;
    void setStartTime(TimePoint);

    TimePoint getEndTime() const;
    void setEndTime(TimePoint);

    int64_t getStudentID() const;
    void setStudentID(int64_t);

    int64_t getFacultyID() const;
    void setFacultyID(int64_t);

    template<typename Action>
    void persist(Action&);
};

template <typename Action>
void Appointment::persist(Action &a){
    namespace dbo = Wt::Dbo;
    dbo::field(a, title, "title");
    dbo::field(a, description, "description");
    dbo::field(a, startTime, "startTime");
    dbo::field(a, endTime, "endTime");
    dbo::field(a, studentID, "studentID");
    dbo::field(a, facultyID, "facultyID");
}

#endif
