#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "api.h"
#include "request.h"

#include <iostream>
#include <QTextBlock>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MiniSQL");

    sqlEdit = new QTextEdit(this);
    QFont editFont("Consolas", 14);
    sqlEdit->setFont(editFont);

    result = new QTextEdit(this);
    QFont showFont("calibri", 12);
    result->setFont(showFont);

    execButton = new QPushButton("Execute");

    exitButton = new QPushButton("Exit");

    layout = new QGridLayout();
    layout->addWidget(sqlEdit, 0, 0, 8, 5);
    layout->addWidget(result, 8, 0, 4, 5);
    layout->addWidget(execButton, 12, 1, 1, 1);
    layout->addWidget(exitButton, 12, 3, 1, 1);

    window = new QWidget;
    window->setLayout(layout);

    setCentralWidget(window);

    connect(execButton, SIGNAL(clicked()), this, SLOT(interpreter()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));

    bufferM = new BufferManager;
    catalogM = new CatalogManager;
    recordM = new RecordManager(bufferM, catalogM);
    api = new API(catalogM, recordM);
}

MainWindow::~MainWindow()
{
    delete (api);
    delete (recordM);
    delete (catalogM);
    delete (bufferM);
}

void MainWindow::interpreter()
{
    QString txt = sqlEdit->toPlainText();
    txt.replace('\n', ' ');
    txt.replace('\t', ' ');
    txt.replace(QRegExp("[\\s]+"), " ");
    int insNum = txt.count(';');
    for (int insIndex = 0; insIndex < insNum; ++insIndex) {
        QString ins = txt.section(';', insIndex, insIndex).trimmed();
        Request req;
        QString insType = ins.section(' ', 0, 0);

        if (insType == "create") {
            QString insType1 = ins.section(' ', 1, 1);
            if (insType1 == "table") {
                req.type = create_table;
                req.tableName = ins.section(' ', 2, 2).toStdString();
                int attriBegin = ins.indexOf('(');
                int attriEnd = ins.lastIndexOf(')');
                QString attriTxt = ins.mid(attriBegin + 1, attriEnd - attriBegin - 1).trimmed(); // get info
                int attriNum = attriTxt.count(','); // attributes number
                req.attri = new std::vector<Attribute>;
                for (int i = 0; i < attriNum; ++i) {
                    int commaLoca = attriTxt.indexOf(',');
                    QString tempStr = attriTxt.left(commaLoca).trimmed();
                    Attribute tempAttri;
                    tempAttri.setName(tempStr.section(' ', 0, 0).toStdString());
                    QString nico = (tempStr.section(' ', 1, 1));
                    if (tempStr.contains('(')) {
                        nico += tempStr.section(' ', 2, 2);
                    }
                    tempAttri.setType(nico.toStdString());
                    if (tempStr.contains("unique") != 0) {
                        tempAttri.isUnique = true;
                    }
                    req.attri->push_back(tempAttri);
                    attriTxt = attriTxt.right(attriTxt.size() - commaLoca - 1).trimmed();
                }
                attriBegin = attriTxt.lastIndexOf('(');
                attriEnd = attriTxt.lastIndexOf(')');
                req.primaryKey = attriTxt.mid(attriBegin + 1, attriEnd - attriBegin - 1).trimmed().toStdString();
//                qDebug() << "attributes: " << endl;
//                for (int i = 0; i < req.attri->size(); ++i) {
//                    qDebug() << QString::fromStdString(req.attri->at(i).getName()) << " " << req.attri->at(i).getType() << endl;
//                }
            }
            else if (insType1 == "index") {
                req.type = create_index;
                req.indexName = ins.section(' ', 2, 2).toStdString();
                req.tableName = ins.section(' ', 4, 4).toStdString();
                int leftBraket = ins.indexOf('(');
                int rightBraket = ins.indexOf(')');
                req.indexAttr = ins.mid(leftBraket + 1, rightBraket - leftBraket - 1).trimmed().toStdString();
                // qDebug() << "indexName: " << QString::fromStdString(req.indexName) << endl;
                // qDebug() << "tableName: " << QString::fromStdString(req.tableName) << endl;
                // qDebug() << "indexAttr: " << QString::fromStdString(req.indexAttr) << endl;
            }
            else {
                qDebug() << "Instruction error" << endl;
            }
        }
        else if (insType == "drop") {
            ins.remove(';');
            QString insType1 = ins.section(' ', 1, 1);
            if (insType1 == "table") {
                req.type = drop_table;
                req.tableName = ins.section(' ', 2, 2).toStdString();
                // qDebug() << QString::fromStdString(req.tableName) << endl;
            }
            else if (insType1 == "index") {
                req.type = drop_index;
                req.tableName = ins.section(' ', 2, 2).toStdString();
                // qDebug() << QString::fromStdString(req.tableName) << endl;
            }
            else {
                qDebug() << "Instruction error" << endl;
            }
            ins += ';';
        }
        else if (insType == "insert") {
            req.type = insert;
            req.tableName = ins.section(' ', 2, 2).toStdString();
            int leftBraket = ins.indexOf('(');
            int rightBraket = ins.indexOf(')');
            QString valueTxt = ins.mid(leftBraket + 1, rightBraket - leftBraket - 1).trimmed();
            valueTxt += ",";
            int attriNum = valueTxt.count(',');
            req.insertValue = new std::vector<std::string>;
            for (int i = 0; i < attriNum; ++i) {
                int commaLoca = valueTxt.indexOf(',');
                QString tempStr = valueTxt.left(commaLoca).trimmed();
                req.insertValue->push_back(tempStr.section(',', 0, 0).toStdString());
                valueTxt = valueTxt.right(valueTxt.size() - commaLoca - 1).trimmed();
            }
//            for (int i = 0; i < attriNum; ++i) {
//                qDebug() << QString::fromStdString(req.insertValue->at(i)) << endl;
//            }
        }
        else if (insType == "delete") {
            req.type = _delete;
            ins.remove(';');
            req.tableName = ins.section(' ', 2, 2).toStdString();
            if (ins.contains("where") == false) {
                req.w = nullptr;
            }
            else {
                req.w = new std::vector<where>;
                int whereNum = ins.count("and");
                for (int i = 0; i < whereNum; ++i) {
                    struct where tempWhere;
                    tempWhere.attr = ins.section(' ', 5+4*i, 5+4*i).toStdString();
                    tempWhere.relation = ins.section(' ', 6+4*i, 6+4*i).toStdString();
                    tempWhere.value = ins.section(' ', 7+4*i, 7+4*i).toStdString();
                    req.w->push_back(tempWhere);
                }
            }
            ins += ';';
        }
        else if (insType == "select") {
            req.type = select;
            ins.remove(';');
            req.tableName = ins.section(' ', 3, 3).toStdString();
            if (ins.contains("where") == false) {
                req.w = nullptr;
            }
            else {
                req.w = new std::vector<where>;
                int whereNum = ins.count("and");
                for (int i = 0; i < whereNum; ++i) {
                    struct where tempWhere;
                    tempWhere.attr = ins.section(' ', 5+4*i, 5+4*i).toStdString();
                    tempWhere.relation = ins.section(' ', 6+4*i, 6+4*i).toStdString();
                    tempWhere.value = ins.section(' ', 7+4*i, 7+4*i).toStdString();
                    req.w->push_back(tempWhere);
                }
            }
            ins += ';';
        }
        api->drive(req);
//        delete(bufferM);
//        delete(catalogM);
//        delete(recordM);
//        delete(api);
//        bufferM = new BufferManager;
//        catalogM = new CatalogManager;
//        recordM = new RecordManager(bufferM, catalogM);
//        api = new API(catalogM, recordM);
    }
}
