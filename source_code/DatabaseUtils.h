#ifndef DATABASE_UTILS_H
#define DATABASE_UTILS_H

#include <memory>

#include <Wt/Dbo/SqlConnection.h>
#include <Wt/Dbo/backend/Sqlite3.h>

#include "Session.h"
#include "TableCreatorSingleton.h"
#include "ITableCreator.h"

template<typename Model>
void addTable(
    std::unique_ptr<Wt::Dbo::SqlConnection> backend,
    std::string tableName
) {
    backend->setProperty("show-queries", "false");

    AASS::Session session(std::move(backend));

    try {
        session.mapClass<Model>(tableName.c_str());
        session.createTables();
    }
    catch (std::exception &e) {
        std::cerr << "Table " << tableName << " already exists in database.\n";
    }
}

template<typename Backend = Wt::Dbo::backend::Sqlite3>
std::unique_ptr<Wt::Dbo::SqlConnection> createDBBackend(
    std::string connStr = "db.sqlite3"
) {
    return std::make_unique<Backend>(connStr);
}


#define CREATE_TABLE(__MODEL_, __TABLE_NAME_) \
    namespace { \
        static struct TableCreator ## __TABLE_NAME_ : public ITableCreator { \
            inline static bool added = false; \
            TableCreator ## __TABLE_NAME_ () { \
                if (!added) { \
                    added = true; \
                    TableCreatorSingleton::addTableCreator( \
                        std::make_unique<TableCreator ## __TABLE_NAME_ >() \
                    ); \
                } \
            } \
            void createTable(std::unique_ptr<Wt::Dbo::SqlConnection> conn) override { \
                addTable<__MODEL_>(std::move(conn), # __TABLE_NAME_ ); \
            } \
            ~TableCreator ## __TABLE_NAME_ () {} \
        } TableCreator ## __TABLE_NAME_ ## Instance; \
    }

#endif // DATABASE_UTILS_H

