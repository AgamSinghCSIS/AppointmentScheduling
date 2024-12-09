#include <memory>
#include <utility>
#include <stdexcept>

#include <Wt/Dbo/Dbo.h>

#include "Term.h"
#include "TermQuery.h"

namespace dbo = Wt::Dbo;

using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

TermQuery::TermQuery(dbo::Session &s) : session(s) {}

void TermQuery::addTerm(std::unique_ptr<Term> term) {
    session.add(std::move(term));
} 

std::optional<dbo::ptr<Term>> TermQuery::findById(int64_t id) {
    using Terms = dbo::collection<dbo::ptr<Term>>;

    Terms terms = session
        .find<Term>()
        .where("id = ?")
        .bind(id)
        ;

    if (terms.size() == 0) {
        return {};
    }

    return terms.front();
}

std::optional<dbo::ptr<Term>> TermQuery::findByTime(TimePoint includedTime) {
    using Terms = dbo::collection<dbo::ptr<Term>>;

    Terms terms = session
        .find<Term>()
        .where("startTime <= ?")
        .bind(includedTime)
        .where("endTime >= ?")
        .bind(includedTime)
        ;

    if (terms.size() == 0) {
        return {}; 
    }

    return terms.front();
}

dbo::collection<dbo::ptr<Term>> TermQuery::findOldTerms() {
    using Terms = dbo::collection<dbo::ptr<Term>>;

    Terms terms = session
        .find<Term>()
        .where("endTime <= ?")
        .bind(std::chrono::system_clock::now())
        ;

    return terms;
}

