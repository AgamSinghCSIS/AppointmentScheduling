#include <chrono>
#include <memory>
#include <utility>

#include "TermBuilder.h"
#include "Term.h"

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

void TermBuilder::reset() {
    name = "";
    startTime = TimePoint::min();
    endTime = TimePoint::max();
}

TermBuilder::TermBuilder() {
    reset();
}

TermBuilder& TermBuilder::withName(std::string s) {
    name = std::move(s);
    return *this;
}

TermBuilder& TermBuilder::withStartTime(TimePoint t) {
    startTime = t;
    return *this;
}

TermBuilder& TermBuilder::withEndTime(TimePoint t) {
    endTime = t;
    return *this;
}

Term TermBuilder::build() {
    Term t;
    t.setName(name);
    t.setStartTime(startTime);
    t.setEndTime(endTime);
    reset();
    return t;
}

std::unique_ptr<Term> TermBuilder::buildUnique() {
    auto t = std::make_unique<Term>();
    t->setName(name);
    t->setStartTime(startTime);
    t->setEndTime(endTime);
    reset();
    return t;
}

