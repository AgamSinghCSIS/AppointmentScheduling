#include <iostream>
#include <memory>
#include <utility>

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>

#include "ITermQuery.h"
#include "Term.h"
#include "TermBuilder.h"
#include "TermStub.h"
#include "TermQuery.h"
#include "TimeUtils.h"

namespace dbo = Wt::Dbo;

auto createSqliteBackend(std::string connName = "db.sqlite3") {
    return std::make_unique<dbo::backend::Sqlite3>(connName);
}

int runTermTest() {
    auto sqlite3 = createSqliteBackend();
    sqlite3->setProperty("show-queries", "true");

    dbo::Session session;
    session.setConnection(std::move(sqlite3));

    session.mapClass<Term>("Term");

    try {
        session.createTables();
    }
    catch (...) {}

    {
        auto term = TermBuilder()
            .withName("foo")
            .withStartTime(
                timePointFromStr("2024-04-22 12:30:00")
            )
            .withEndTime(
                timePointFromStr("2024-04-22 13:00:00")
            )
            .buildUnique()
            ;

        std::cout << "Name: " << term->getName() << '\n'
                  << "Start Time: " << timePointToStr(term->getStartTime()) << '\n'
                  << "End Time: " << timePointToStr(term->getEndTime()) << '\n'
                  ;
    }

    return 0;
}

