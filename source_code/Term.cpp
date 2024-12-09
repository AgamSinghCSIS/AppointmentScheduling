#include <chrono>
#include <string>
#include <utility>

#include "DatabaseUtils.h"
#include "Term.h"

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

Term::Term() :
    name(""),
    startTime(TimePoint::min()),
    endTime(TimePoint::max()) {}

std::string Term::getName() const {
    return name;
}

void Term::setName(std::string s) {
    name = std::move(s);
}

TimePoint Term::getStartTime() const {
    return startTime;
}

void Term::setStartTime(TimePoint t) {
    startTime = t;
}

TimePoint Term::getEndTime() const {
    return endTime;
}

void Term::setEndTime(TimePoint t) {
    endTime = t;
}

CREATE_TABLE(Term, term)

