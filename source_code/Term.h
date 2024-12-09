#ifndef TERM_H 
#define TERM_H

#include <chrono>
#include <string>

#include <Wt/Dbo/Dbo.h>

class Term {
private:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    std::string name;
    TimePoint startTime;
    TimePoint endTime;
public:
    Term();

    std::string getName() const;
    void setName(std::string);

    TimePoint getStartTime() const;
    void setStartTime(TimePoint);

    TimePoint getEndTime() const;
    void setEndTime(TimePoint);

    template<typename Action>
    void persist(Action&);
};

template<typename Action>
void Term::persist(Action &a) {
    namespace dbo = Wt::Dbo;

    dbo::field(a, name, "name");
    dbo::field(a, startTime, "startTime");
    dbo::field(a, endTime, "endTime");
}

#endif // TERM_H
