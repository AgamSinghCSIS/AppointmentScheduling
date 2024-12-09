#ifndef TERM_QUERY_H 
#define TERM_QUERY_H

#include <memory>
#include <optional>

#include <Wt/Dbo/Dbo.h>

#include "Term.h"
#include "ITermQuery.h"

class TermQuery: public ITermQuery {
private:
    Wt::Dbo::Session &session;
public:
    explicit TermQuery(Wt::Dbo::Session&);
    void addTerm(std::unique_ptr<Term>) override;
    std::optional<Wt::Dbo::ptr<Term>> findById(int64_t) override;
    std::optional<Wt::Dbo::ptr<Term>> findByTime(
        std::chrono::time_point<std::chrono::system_clock>
    ) override;
    Wt::Dbo::collection<Wt::Dbo::ptr<Term>> findOldTerms();
};

#endif // TERM_QUERY_H
