#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QString>
#include <QtSql/qsql.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>
#include <QSqlTableModel>
#include <qsqldatabase.h>
#include <QtSql/qsqlrecord.h>
#include <QFileDialog>
#include "player.hh"
#include "fileIO.hh"
#include <fstream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    // open file slot connected to load history file button
    void openFile();
    //when a player is chosen, the general table changes
    void tableChanged(QString);
    //when a player is chosen ,the statistics and the graph changes
    void tableUpdate(QString);
    //when the private cards change, the hands table changes
    void handsChanged(QString);
    //when a session in the table is chosen, the table of session details changes
    void sessionHands(QModelIndex);
    //when the dates change, the table changes
    void dateShow();
    //when a session in the table is chosen, the table of session details changes
    void sessionDate(QModelIndex);
    //when the money won changes, the table changes
    void moneyChanged();
    //when a session in the table is chosen, the table of session details changes
    void sessionMoney(QModelIndex);
    //when the position changes, the table changes
    void positionChanged(QString);  
    //when a session in the table is chosen, the table of session details changes
    void sessionPosition(QModelIndex);
    
private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    //table model used in the general page
    QSqlTableModel *table;
    //table model used in the hands page
    QSqlTableModel *table_2;
    //table model used in the positions page
    QSqlTableModel *table_4;
    //table model used in the sessions page
    QSqlTableModel *table_6;
    //table model used in the money won page
    QSqlTableModel *table_8;

};

#endif // MAINWINDOW_H
