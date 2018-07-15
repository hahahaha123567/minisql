#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QTextEdit>
#include <QPushButton>

#include "buffermanager.h"
#include "catalogmanager.h"
#include "recordmanager.h"
#include "api.h"

#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    // gui
    Ui::MainWindow *ui;
    QWidget * window;
    QGridLayout * layout;
    QTextEdit * sqlEdit;
    QTextEdit * result;
    QPushButton * execButton;
    QPushButton * exitButton;
    // function
    BufferManager * bufferM;
    CatalogManager * catalogM;
    RecordManager * recordM;
    API * api;
private slots:
    // interpreter
    void interpreter();
};

#endif // MAINWINDOW_H
