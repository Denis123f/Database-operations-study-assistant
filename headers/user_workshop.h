#ifndef USER_WORKSHOP_H
#define USER_WORKSHOP_H

#include <textedituser.h>

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtSql>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QRandomGenerator>
#include <QApplication>

class User_WorkShop : public QWidget
{
    Q_OBJECT

    QSqlDatabase connect_to_database;
    Textedituser* query_user_text;
    QTextEdit* helper_toUser_query;

    QPushButton* run_query;
    QPushButton* clear_text_user;
    QPushButton* generation_table;

    QHBoxLayout* user_workshop;
    QHBoxLayout* user_tools_helper;

    QWidget* widget_user_tools_helper;

    QVBoxLayout* user_workshop_right_side;

    QVector<QString> custom_query;
    QVector<QString> custom_FirstName;
    QVector<QString> custom_LastName;
    QVector<QString> custom_Address;
    QVector<QString> custom_City;

public:
    User_WorkShop(QWidget* parent = nullptr);
    QPushButton* get_run_query();
    QPushButton* get_generation_table();
    QPushButton* get_clear_text_user();
    Textedituser* get_query_user_text();

public slots:
    bool user_generation_table(QSqlTableModel*, QSqlDatabase&);
};

#endif // USER_WORKSHOP_H
