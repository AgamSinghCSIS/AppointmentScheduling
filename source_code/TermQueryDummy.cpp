#include "TermQueryDummy.h"

#include <chrono>
#include <memory>
#include <optional>
#include <utility>

#include "Term.h"
#include "TermBuilder.h"
#include "TermQueryDummy.h"
#include "TimeUtils.h"

namespace dbo = Wt::Dbo;

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

TermQueryDummy::TermQueryDummy(dbo::Session &s) {}

void TermQueryDummy::addTerm(std::unique_ptr<Term> term) {
    return;
} 

std::optional<dbo::ptr<Term>> TermQueryDummy::findById(int64_t id) {
    return TermBuilder()
        .withName("Spring 2024")
        .withStartTime(timePointFromStr("2024-01-08 00:00:00"))
        .withEndTime(timePointFromStr("2024-05-13 00:00:00"))
        .buildUnique()
        ;
}

std::optional<dbo::ptr<Term>> TermQueryDummy::findByTime(
    TimePoint includedTime
) {
    return TermBuilder()
        .withName("Spring 2024")
        .withStartTime(timePointFromStr("2024-01-08 00:00:00"))
        .withEndTime(timePointFromStr("2024-05-13 00:00:00"))
        .buildUnique()
        ;
}

