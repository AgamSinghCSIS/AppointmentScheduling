#ifndef I_TERM_QUERY_H
#define I_TERM_QUERY_H

#include <chrono>
#include <memory>
#include <optional>

#include <Wt/Dbo/Dbo.h>

#include "Term.h"

class ITermQuery {
public:
    virtual void addTerm(std::unique_ptr<Term>) = 0;
    virtual std::optional<Wt::Dbo::ptr<Term>> findById(int64_t) = 0;
    virtual std::optional<Wt::Dbo::ptr<Term>> findByTime(
        std::chrono::time_point<std::chrono::system_clock>
    ) = 0;
    virtual ~ITermQuery() {}
};

#endif // I_TERM_QUERY_H
