#ifndef TABLE_CREATOR_SINGLETON_H
#define TABLE_CREATOR_SINGLETON_H

#include <functional>
#include <vector>

#include "DatabaseUtils.h"
#include "ITableCreator.h"

class TableCreatorSingleton {
public:
    static void addTableCreator(std::unique_ptr<ITableCreator>);
    static void createTables(
        std::function<std::unique_ptr<Wt::Dbo::SqlConnection>()>
    );
private:
    TableCreatorSingleton();
    inline static std::vector<std::unique_ptr<ITableCreator>> creators;
};

#endif // TABLE_CREATOR_SINGLETON_H
