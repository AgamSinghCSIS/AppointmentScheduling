#include <functional>
#include <memory>

#include <Wt/Dbo/SqlConnection.h>

#include "DatabaseUtils.h"
#include "Session.h"
#include "TableCreatorSingleton.h"

void TableCreatorSingleton::addTableCreator(
    std::unique_ptr<ITableCreator> creator
) {
    creators.push_back(std::move(creator));
}

void TableCreatorSingleton::createTables(
    std::function<std::unique_ptr<Wt::Dbo::SqlConnection>()> dbCreator
) {
    for (const auto &creator : creators) {
        creator->createTable(dbCreator()); 
    }
}
