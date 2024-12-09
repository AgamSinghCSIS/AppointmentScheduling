#ifndef TERM_BUILDER_H
#define TERM_BUILDER_H

#include <chrono>
#include <memory>
#include <utility>

#include "Term.h"

class TermBuilder {
private:
    using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
    std::string name;
    TimePoint startTime;
    TimePoint endTime;

    void reset();
public:
    TermBuilder();
    TermBuilder& withName(std::string);
    TermBuilder& withStartTime(TimePoint);
    TermBuilder& withEndTime(TimePoint);
    Term build();
    std::unique_ptr<Term> buildUnique();
};

#endif // TERM_BUILDER_H
