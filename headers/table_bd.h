#ifndef TABLE_BD_H
#define TABLE_BD_H

#include <QWidget>
#include <QSqlRecord>
#include <QSqlField>
#include <QMetaType>
#include <QTableView>
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHeaderView>

class Table_BD : public QWidget
{
    Q_OBJECT
    QTableView* table;
    QVBoxLayout* QVBL;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * movement_mouse);
    void mouseReleaseEvent(QMouseEvent *event);

    QPointF dragOffset;
public:
    Table_BD(const QSqlRecord&&, QWidget *parent = nullptr);
};

class Viewing_the_tree : public QWidget{

    Q_OBJECT
    QVector<Table_BD*> replase;
public:
    Viewing_the_tree(QWidget *parent = nullptr);
    void add_table(const QSqlRecord&&);
    void show_replase();
};

#endif // TABLE_BD_H
