#include "tool_settings.h"

// Auxiliary functions for a more simplified window setup.
inline void removeBorders(QWidget* widg)
{
    widg->setContentsMargins(0,0,0,0);
}
inline void removeBorders(QLayout* lay)
{
    lay->setContentsMargins(0,0,0,0);
}

Tool_settings::Tool_settings(QWidget* parent)
    : QWidget(parent)
{
    // settings window //


    //    setStyleSheet(
    //            "background-color: #f5f7fa;"
    //            "border: none;"
    //            "border-bottom: 1px solid black;"
    //        );

    removeBorders(this);
    // settings window //

    base_border = "QLineEdit { border: 2px solid #cccccc; border-radius: 4px; padding: 4px; } QLineEdit:focus { border: 2px solid #0078d4; }";
    error_border = "QLineEdit { border: 2px solid red; border-radius: 4px; padding: 4px; background-color: #ffe6e6; }";
    tool_settings = new QPushButton("Settings");
    tree_database = new QPushButton("tree_database");

    tools_bar = new QHBoxLayout();
    tools_bar->setAlignment(Qt::AlignLeft);
    tools_bar->addWidget(tool_settings);
    tools_bar->addWidget(tree_database);
    removeBorders(tools_bar);
    setLayout(tools_bar);
}

QPushButton* Tool_settings::get_tree_database()
{
    return tree_database;
}
QPushButton* Tool_settings::get_settings()
{
    return tool_settings;
}
void help_in_setting(QVBoxLayout* QVBL, const QVector<QString>& nameing, bool setVisible)
{
    for(int i = 0; i < QVBL->count(); ++i)
    {
        if(QVBL->itemAt(i)->layout()) continue;
        for(int j = 0; j < nameing.size(); ++j){
            if(nameing[j] == QVBL->itemAt(i)->widget()->objectName())
            {
                QVBL->itemAt(i)->widget()->setVisible(setVisible);
            }
        }
    }
}

bool Tool_settings::non_duplicate_error_dorder(QLineEdit* error)
{
    if(error->text().isEmpty()){
         error->setStyleSheet(error_border);
         connect(error, &QLineEdit::textChanged, [error, this](){
             error->setStyleSheet(base_border);
         });
         return false;
    }else{return true;}
    return false;
}

[[nodiscard]]std::tuple<bool, QString, QVector<QString>> Tool_settings::setting_menu()
{
    std::tuple<bool, QString, QVector<QString>> result = {0, "", {}};
    QDialog* menu = new QDialog();
    menu->setGeometry(300, 300, 300, 300);
    connect(menu, &QWidget::destroyed, [=](){
       delete menu;
    });

    QVBoxLayout* QVBL = new QVBoxLayout(menu);
    QVBL->setAlignment(Qt::AlignTop);

    QLabel* label_enter = new QLabel("Enter the DataBase to work: ", menu);
    label_enter->setObjectName("label_enter");

    QPushButton* bnt_connect_to_RAM = new QPushButton("Connect to Ram", menu);
    bnt_connect_to_RAM->setObjectName("bnt_connect_to_RAM");
    QCheckBox* is_user_want_save = new QCheckBox(menu);
    is_user_want_save->setText("Save your data?");
    is_user_want_save->setLayoutDirection(Qt::RightToLeft);
    is_user_want_save->setChecked(1);

    QHBoxLayout* QHBL = new QHBoxLayout();
    QHBL->setAlignment(Qt::AlignRight);

    // Settings postgresql !

    QLabel* port_posgresql = new QLabel("Enter the port: ", menu);
    QLineEdit* port_posgresql_text = new QLineEdit(menu);
    port_posgresql_text->setObjectName("port_posgresql_text");
    port_posgresql_text->setStyleSheet(base_border);
    port_posgresql->setObjectName("port_posgresql");

    QLabel* host_posgresql = new QLabel("Enter the host: ", menu);
    QLineEdit* host_posgresql_text = new QLineEdit(menu);
    host_posgresql_text->setObjectName("host_posgresql_text");
    host_posgresql_text->setStyleSheet(base_border);
    host_posgresql->setObjectName("host_posgresql");

    QLabel* username = new QLabel("Enter the username: ", menu);
    QLineEdit* username_text = new QLineEdit(menu);
    username_text->setObjectName("username_text");
    username_text->setStyleSheet(base_border);
    username->setObjectName("username");

    QLabel* password = new QLabel("Enter the password: ", menu);
    QLineEdit* password_text = new QLineEdit(menu);
    password_text->setObjectName("password_text");
    password_text->setStyleSheet(base_border);
    password->setObjectName("password");

    QLabel* databaseName = new QLabel("Enter the database name: ", menu);
    QLineEdit* databaseName_text = new QLineEdit(menu);
    databaseName_text->setObjectName("databaseName_text");
    databaseName_text->setStyleSheet(base_border);
    databaseName->setObjectName("databaseName");

    QPushButton* connect_posgresql= new QPushButton("Connect");
    connect_posgresql->setObjectName("connect_posgresql");

    // End settings postgresql !

    connect_posgresql->setVisible(0);
    port_posgresql->setVisible(0);
    port_posgresql_text->setVisible(0);
    host_posgresql ->setVisible(0);
    host_posgresql_text->setVisible(0);
    username->setVisible(0);
    username_text->setVisible(0);
    password->setVisible(0);
    password_text->setVisible(0);
    databaseName->setVisible(0);
    databaseName_text->setVisible(0);

    QComboBox* QCB = new QComboBox(menu);
    QCB->addItems({"RAM","SQLITE", "PostgreSQL"});
    QCB->setObjectName("QCB");
    QLabel* label_path = new QLabel("Enter the path in DataBase:", menu);
    label_path->setObjectName("label_path");
    QPushButton* select_bd = new QPushButton(menu);
    select_bd->setObjectName("select_bd");
    label_path->setVisible(0);
    select_bd->setVisible(0);

    QVector<QString> RAM = {"bnt_connect_to_RAM"};
    QVector<QString> SQLite = {"label_path", "select_bd"};
    QVector<QString> PostgreSQL = {"port_posgresql", "host_posgresql", "username", "password", "connect_posgresql", "port_posgresql_text",
                                   "host_posgresql_text", "username_text", "password_text", "databaseName_text", "databaseName"};

    connect(QCB, &QComboBox::currentIndexChanged, [QVBL, QCB, PostgreSQL, SQLite, RAM](){

       help_in_setting(QVBL, PostgreSQL + SQLite + RAM, false);

       QString now_text = QCB->currentText();
       if(now_text == "SQLITE"){
            help_in_setting(QVBL, SQLite, true);
       }else if(now_text == "PostgreSQL"){
            help_in_setting(QVBL, PostgreSQL, true);
       }else if(now_text == "RAM")
       {
            help_in_setting(QVBL, RAM, true);
       }
    });
    connect(bnt_connect_to_RAM, &QPushButton::clicked, [&result, &menu, this](){

        write_is_in_file({"0"});
        result = std::make_tuple(true, "RAM", QVector<QString>());
        menu->close();
    });
    connect(select_bd, &QPushButton::clicked, [&](){
       QString QFdial;
       if(QCB->currentText() == "SQLITE")
       {
           QFdial = QFileDialog::getOpenFileName(menu, "Select DB", "~", "SQLite (*.db *.sqlite)");
           qDebug() << QFdial;
           if(!QFdial.isEmpty())
           {
               result = std::make_tuple(true, "SQLITE", QVector<QString>(QFdial));

               if(is_user_want_save->isChecked())
               {
                    write_is_in_file({"SQLITE", QFdial});
               }else
               {
                    write_is_in_file({"0"});
               }

               menu->close();
           }
       }
    });
    connect(connect_posgresql, &QPushButton::clicked, [&](){
       bool valid_port_posgresql_text = non_duplicate_error_dorder(port_posgresql_text);
       bool valid_host_posgresql_text = non_duplicate_error_dorder(host_posgresql_text);
       bool valid_password_text = non_duplicate_error_dorder(username_text);
       bool valid_username_text = non_duplicate_error_dorder(password_text);
       bool valid_databaseName_text = non_duplicate_error_dorder(databaseName_text);
       if(valid_port_posgresql_text && valid_host_posgresql_text && valid_password_text
          && valid_username_text && valid_databaseName_text)
       {

           QVector<QString> text_vec = {port_posgresql_text->text(), host_posgresql_text->text(),
                                        username_text->text(), password_text->text(), databaseName_text->text()};
           result = std::make_tuple(true, "PostgreSQL", text_vec);
           text_vec.insert(0, "PostgreSQL");
           if(is_user_want_save->isChecked())
           {
                write_is_in_file(text_vec);
           }else
           {
                write_is_in_file({"0"});
           }
           menu->close();
       }
    });

    QVBL->addWidget(label_enter);
    QVBL->addWidget(QCB);
    QVBL->addWidget(bnt_connect_to_RAM);
    QVBL->addWidget(port_posgresql);
    QVBL->addWidget(port_posgresql_text);
    QVBL->addWidget(host_posgresql);
    QVBL->addWidget(host_posgresql_text);
    QVBL->addWidget(username);
    QVBL->addWidget(username_text);
    QVBL->addWidget(password);
    QVBL->addWidget(password_text);
    QVBL->addWidget(databaseName);
    QVBL->addWidget(databaseName_text);
    QVBL->addWidget(connect_posgresql);
    QVBL->addWidget(label_path);
    QVBL->addWidget(select_bd);

    QHBL->addWidget(is_user_want_save);

    QVBL->addLayout(QHBL);

    if(menu->exec() == QDialog::Accepted)
    {
        qDebug() << "exit set";
        return result;
    }
    return result;
}

void Tool_settings::write_is_in_file(const QVector<QString>& vec_attr)
{
    QFile file(QDir::currentPath() + "/user_data.txt");
    if(file.open(QFile::WriteOnly)){
        qDebug() << "file open!";
    }
    QTextStream reader(&file);
    if(vec_attr[0].size() == 1)
    {
        reader << 0;
    }
    else{
        reader << 1 << '\n';
        for(auto& i : vec_attr)
        {
            std::unique_ptr<Coder_save_information> coder = std::make_unique<Coder_save_information>(i, reader);
            coder->shifr();
        }
    }
    file.close();
}
