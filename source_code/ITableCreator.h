#ifndef I_TABLE_CREATOR_H
#define I_TABLE_CREATOR_H

struct ITableCreator {
    virtual void createTable(std::unique_ptr<Wt::Dbo::SqlConnection>) = 0;
    virtual ~ITableCreator() {}
};

#endif
