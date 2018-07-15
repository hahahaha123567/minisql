#include "api.h"
#include "catalogmanager.h"
#include "recordmanager.h"
#include <QMessageBox>
#include <QDebug>

API::API(CatalogManager *Manager1, RecordManager *Manager2)
    : catalogM(Manager1), recordM(Manager2)
{

}

void API::drive(const Request & r)
{
    switch (r.type) {
    case create_table: createTable(r); break;
    case drop_table: dropTable(r); break;
    case insert: insertTuple(r); break;
    case _delete: deleteTuple(r); break;
    case select: selectInfo(r); break;
    case create_index: createIndex(r); break;
    default: dropIndex(r); // drop_index
    }
}

void API::createTable(const Request & r)
{
    QMessageBox msgBox;
    qDebug() << "create table" << endl;

    if (catalogM->tableExists(r.tableName)) {
        msgBox.setText("Table has existed");
        msgBox.exec();
        return;
    }
    Table t;
    t.name = r.tableName;
    t.primaryKey = r.primaryKey;
    t.attri = *(r.attri);
    recordM->createTable(t);
    catalogM->addTable(t);

    msgBox.setText("Create table success");
    msgBox.exec();
}

void API::dropTable(const Request & r)
{
    QMessageBox msgBox;

    if (catalogM->tableExists(r.tableName) == false) {
        msgBox.setText("Table doesn't exist");
        msgBox.exec();
        return;
    }
    recordM->dropTable(r.tableName);
    catalogM->deleteTable(r.tableName);

    msgBox.setText("Drop table success");
    msgBox.exec();
}

void API::createIndex(const Request & r)
{
    qDebug() << "create index" << endl;
}

void API::dropIndex(const Request & r)
{
    qDebug() << "drop index" << endl;
}

bool API::selectInfo(const Request & r)
{
    qDebug() << "select" << endl;
    return false;
}

void API::insertTuple(const Request & r)
{
    QMessageBox msgBox;
    qDebug() << "insert" << endl;
    //qDebug() << r.insertValue->size() << " " <<  QString::fromStdString(r.insertValue->at(0)) << QString::fromStdString(r.insertValue->at(0)) << endl;

    if (catalogM->tableExists(r.tableName) == false) {
        msgBox.setText("Table doesn't exist");
        msgBox.exec();
        return;
    }
    if (r.insertValue->size() != catalogM->fieldCount(r.tableName)) {
        //qDebug() << r.insertValue->size() << " " << catalogM->fieldCount(r.tableName);
        msgBox.setText("Attributes number doesn't match");
        msgBox.exec();
        return;
    }
    Request check;
    check.w = new std::vector<where>;
    check.type = select;
    check.tableName = r.tableName;
    where checkCondition;
    checkCondition.attr = catalogM->fieldName(r.tableName, catalogM->pkOnTable(r.tableName));
    checkCondition.relation = "=";
    checkCondition.value = r.insertValue->at(catalogM->pkOnTable(r.tableName));
    check.w->push_back(checkCondition);
    if (selectInfo(check)) {
        msgBox.setText("Primary key conflict");
        msgBox.exec();
        return;
    }
    std::vector<std::string> data;
    for (int i = 0; i < r.insertValue->size(); ++i) {
        data.push_back(r.insertValue->at(i));
    }
    recordM->insert(catalogM->getTable(r.tableName), data);
    msgBox.setText("Insert success");
    msgBox.exec();
}

void API::deleteTuple(const Request & r)
{
    qDebug() << "delete" << endl;
}
