#ifndef DIRECTOR_WORK_H
#define DIRECTOR_WORK_H


#include <tool_settings.h>
#include <user_workshop.h>
#include <table_bd.h>
#include <notification.h>
#include <decoder_save_information.h>
#include <sql_working.h>
#include "thread"
#include <vector>
#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtSql>
#include <QTableView>
#include <QTextEdit>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QPalette>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QSystemTrayIcon>
#include <tuple>
#include <QFile>
#include <QTextStream>


class Director_work : public QWidget
{
    Q_OBJECT

    Sql_working* sql_main_work;
    User_WorkShop* user_WorkShop;
    Tool_settings* tool_settings;
    Notification* cav;
    Viewing_the_tree* jbd;

    QVBoxLayout* main_window;

    QString query;
    QString log_error;

public:
    Director_work(QWidget *parent = nullptr);
    void location_BD();
    void setting_window();

    ~Director_work();
};
#endif // DIRECTOR_WORK_H
