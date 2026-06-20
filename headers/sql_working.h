#ifndef SQL_WORKING_H
#define SQL_WORKING_H

#include "decoder_save_information.h"
#include <user_workshop.h>
#include <table_bd.h>
#include <notification.h>

#include <tuple>
#include <QVector>
#include <QSqlQuery>
#include <QFile>
#include <QDir>
#include <QTableView>
#include <QSqlQueryModel>
#include <QHeaderView>

static QString las_zapros;
static QQueue<QString> qu_query;
static std::tuple<QString, QVector<QString>> save_connections;

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject* parent = nullptr);
    void asy_working(QSqlDatabase&);
signals:
     void result_Ready(const QString &);
};

class Sql_working : public QWidget
{
    Q_OBJECT
    QSqlDatabase connect_to_database;
    User_WorkShop* user_WorkShop;

    QTableView* table_user;
    QSqlQueryModel* QSQM;
    QSqlTableModel* QSTM;

    void splitBySemicolonSafe(const QString& str);
public:

    Sql_working(QWidget *parent, User_WorkShop* user_WorkShop);
    QString connecting_database(std::tuple<QString, QVector<QString>> as);

    QTableView* get_table_user();
    QSqlTableModel* get_QSTM();
    QSqlDatabase& get_connect();
    void run_query();
    QString last_connect();

public slots:
    void handleResults(const QString &);

signals:
     void call_worker(QSqlDatabase &);
};

#endif // SQL_WORKING_H
