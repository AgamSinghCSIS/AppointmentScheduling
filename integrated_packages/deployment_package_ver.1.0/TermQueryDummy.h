#ifndef TERM_QUERY_DUMMY_H
#define TERM_QUERY_DUMMY_H

#include <chrono>
#include <memory>
#include <optional>

#include "ITermQuery.h"
#include "Term.h"

class TermQueryDummy: public ITermQuery {
public:
    explicit TermQueryDummy(Wt::Dbo::Session&);
    virtual void addTerm(std::unique_ptr<Term>);
    virtual std::optional<Wt::Dbo::ptr<Term>> findById(int64_t);
    virtual std::optional<Wt::Dbo::ptr<Term>> findByTime(
        std::chrono::time_point<std::chrono::system_clock>
    );
};

#endif // TERM_QUERY_DUMMY_H
