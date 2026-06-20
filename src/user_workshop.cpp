#include "user_workshop.h"

// helper_toUser_query нужно что-то разместить на поле. Думаю, как и хотел поставить поомщь ии.

// Auxiliary functions for a more simplified window setup.
inline void removeBorders(QWidget* widg)
{
    widg->setContentsMargins(0,0,0,0);
}
inline void removeBorders(QLayout* lay)
{
    lay->setContentsMargins(0,0,0,0);
}

#define SZ_fixed_button 30
#define SZ_Icon_Button 26

User_WorkShop::User_WorkShop(QWidget* parent)
    : QWidget(parent)
{

     // settings window //

     removeBorders(this);
     setStyleSheet("background-color: #ffffff;");

     // settings window //

    user_workshop_right_side = new QVBoxLayout();

    run_query = new QPushButton();
    run_query->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    run_query->setFixedSize(SZ_fixed_button, SZ_fixed_button);
    run_query->setIconSize(QSize(SZ_Icon_Button, SZ_Icon_Button));
    run_query->setFlat(true);
    run_query->setCursor(Qt::PointingHandCursor);
    run_query->setToolTip("Start query (ctrl + F5)");
    run_query->setShortcut(QKeySequence("Ctrl+F5"));
    run_query->setStyleSheet(R"(
    QPushButton {
        background-color: transparent;
        border: none;
        padding: 6px;
        border-radius: 6px;
        color: #333333;
    }
    QPushButton:hover {
        background-color: rgba(173, 216, 230, 80);
        border: 1px solid rgba(135, 206, 235, 150);
    }
    QPushButton:pressed {
        background-color: rgba(173, 216, 230, 120);
        border: 1px solid rgba(100, 180, 220, 180);
    }
    QPushButton:disabled {
        background-color: transparent;
        color: #999999;
    }
)");

    clear_text_user = new QPushButton();
    clear_text_user->setIcon(QIcon(QDir::currentPath() + "/image/bin.png"));
    clear_text_user->setFixedSize(SZ_fixed_button, SZ_fixed_button);
    clear_text_user->setIconSize(QSize(SZ_Icon_Button, SZ_Icon_Button));
    clear_text_user->setFlat(true);
    clear_text_user->setCursor(Qt::PointingHandCursor);
    clear_text_user->setToolTip("Clear is text all (ctrl + F6)");
    clear_text_user->setShortcut(QKeySequence("Ctrl+F6"));
    clear_text_user->setStyleSheet(R"(
    QPushButton {
        background-color: transparent;
        border: none;
        padding: 6px;
        border-radius: 6px;
        color: #333333;
    }
    QPushButton:hover {
        background-color: rgba(173, 216, 230, 80);
        border: 1px solid rgba(135, 206, 235, 150);
    }
    QPushButton:pressed {
        background-color: rgba(173, 216, 230, 120);
        border: 1px solid rgba(100, 180, 220, 180);
    }
    QPushButton:disabled {
        background-color: transparent;
        color: #999999;
    }
)");
    generation_table = new QPushButton();
    generation_table->setIcon(QIcon(QDir::currentPath() + "/image/generation.png"));
    generation_table->setFixedSize(SZ_fixed_button, SZ_fixed_button);
    generation_table->setIconSize(QSize(SZ_Icon_Button, SZ_Icon_Button));
    generation_table->setFlat(true);
    generation_table->setCursor(Qt::PointingHandCursor);
    generation_table->setToolTip("Custom table is generation(ctrl + F7)");
    generation_table->setShortcut(QKeySequence("Ctrl+F7"));
    generation_table->setStyleSheet(R"(
    QPushButton {
        background-color: transparent;
        border: none;
        padding: 6px;
        border-radius: 6px;
        color: #333333;
    }
    QPushButton:hover {
        background-color: rgba(173, 216, 230, 80);
        border: 1px solid rgba(135, 206, 235, 150);
    }
    QPushButton:pressed {
        background-color: rgba(173, 216, 230, 120);
        border: 1px solid rgba(100, 180, 220, 180);
    }
    QPushButton:disabled {
        background-color: transparent;
        color: #999999;
    }
)");

    user_workshop = new QHBoxLayout();
    user_tools_helper = new QHBoxLayout();
    widget_user_tools_helper = new QWidget();
    helper_toUser_query = new QTextEdit();
    query_user_text = new Textedituser();

    removeBorders(widget_user_tools_helper);
    removeBorders(user_workshop);
    removeBorders(user_tools_helper);
    removeBorders(helper_toUser_query);

    user_tools_helper->setSpacing(0);
    user_workshop->setSpacing(0);
    helper_toUser_query->setStyleSheet("border:none;");
    helper_toUser_query->setReadOnly(true);

    helper_toUser_query->setStyleSheet
            (
                "border: none;"
                "border-right: 1px solid black;"
            );
    user_workshop->addWidget(helper_toUser_query);

    user_tools_helper->setAlignment(Qt::AlignLeft);
    user_tools_helper->addWidget(run_query);
    user_tools_helper->addWidget(clear_text_user);
    user_tools_helper->addWidget(generation_table);

    widget_user_tools_helper->setLayout(user_tools_helper);

    widget_user_tools_helper->setStyleSheet
            (
                "background-color: #f5f7fa;"
            );


    user_workshop_right_side->setSpacing(0);
    user_workshop_right_side->addWidget(widget_user_tools_helper, 0, Qt::AlignTop);
    query_user_text->setStyleSheet
            (
                "border: none;"
                "border-top: 1px solid black;"
            );
    user_workshop_right_side->addWidget(query_user_text);
    user_workshop->addLayout(user_workshop_right_side, 0);

    setLayout(user_workshop);



    // ------------------------------------ //
    // Create information to generate table //


    custom_query = {
       {"create table custom_user(id int)"},
       {"create table custom_user(id int, FirstName varchar(15))"},
       {"create table custom_user(id int, FirstName varchar(15), LastName varchar(15))"},
       {"create table custom_user(id int, FirstName varchar(15), LastName varchar(15), City varchar(15))"},
       {"create table custom_user(id int, FirstName varchar(15), LastName varchar(15), City varchar(15), Address varchar(20))"}
    };
    custom_FirstName = {
        "James", "John", "Robert", "Michael", "William",
        "Emma", "Olivia", "Ava", "Isabella", "Sophia",
        "Liam", "Noah", "Oliver", "Elijah", "William",
        "Charlotte", "Amelia", "Mia", "Harper", "Evelyn"
    };
    custom_LastName = {
        "Smith", "Johnson", "Williams", "Brown", "Jones",
        "Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
        "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson",
        "Thomas", "Taylor", "Moore", "Jackson", "Martin",
        "Lee", "Perez", "Thompson", "White", "Harris"
    };

    custom_Address = {
        "Main St", "Elm St", "Oak Ave", "Pine Rd", "Maple Dr",
        "Cedar Ln", "Washington Blvd", "Lake St", "Hill St", "Park Ave",
        "Forest Rd", "River Dr", "Sunset Blvd", "Church St", "Willow Ln",
        "Meadow Ln", "High St", "Broadway", "King St", "Queen St"
    };

    custom_City = {
        "New York", "Los Angeles", "Chicago", "Houston", "Phoenix",
        "Philadelphia", "San Antonio", "San Diego", "Dallas", "San Jose",
        "Austin", "Jacksonville", "Fort Worth", "Columbus", "Charlotte",
        "Seattle", "Denver", "Boston", "Portland", "Las Vegas"
    };


}
Textedituser* User_WorkShop::get_query_user_text()
{
    return query_user_text;
}

QPushButton* User_WorkShop::get_run_query()
{
    return run_query;
}
QPushButton* User_WorkShop::get_generation_table()
{
    return generation_table;
}
QPushButton* User_WorkShop::get_clear_text_user()
{
    return clear_text_user;
}

bool User_WorkShop::user_generation_table(QSqlTableModel* QSTM, QSqlDatabase& connect_to_database)
{
    QDialog dialog(this);
    dialog.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    dialog.setWindowModality(Qt::ApplicationModal);
    QLabel* txt = new QLabel("The numbers can be no more than 5 columns and 1000 rows.");
    QLineEdit *input1 = new QLineEdit(&dialog);
    QLineEdit *input2 = new QLineEdit(&dialog);

    std::unique_ptr<QRegularExpressionValidator> validator_colomn =
            std::make_unique<QRegularExpressionValidator>(QRegularExpression("([1-5])"));
    std::unique_ptr<QRegularExpressionValidator> validator_row =
            std::make_unique<QRegularExpressionValidator>(QRegularExpression("([1-9]|[1-9][0-9]|[1-9][0-9][0-9]|[1][0][0][0])"));

    input1->setValidator(validator_colomn.get());
    input2->setValidator(validator_row.get());

    QFormLayout *form = new QFormLayout();

    form->addRow("Value columns:", input1);
    form->addRow("Value rows:", input2);

    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog
    );
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->addWidget(txt);
    layout->addLayout(form);
    layout->addWidget(buttons);

    dialog.resize(250, 120);
    if (dialog.exec() == QDialog::Accepted) {
            QSTM->setTable("");
            int num1_colomn = input1->text().toInt();
            int num2_row = input2->text().toInt();
            if(!connect_to_database.isOpen())
                if(!connect_to_database.open()){
                    qDebug() << connect_to_database.lastError().text();
                    return false;
                }
            QSqlQuery query_user(connect_to_database);

            if (num1_colomn && num2_row) {

                if(query_user.exec("DROP TABLE IF EXISTS custom_user"))
                {
                    qDebug() << "dropper";
                }
                if(query_user.exec(custom_query.at(num1_colomn-1)))
                {

                    for(int i = 1; i < num2_row+1; ++i)
                    {
                            switch (num1_colomn)
                            {
                                case 1:
                                    query_user.prepare("INSERT INTO custom_user (id) VALUES (?)");
                                    break;
                                case 2:
                                    query_user.prepare("INSERT INTO custom_user (FirstName, id) VALUES (?, ?)");
                                    break;
                                case 3:
                                    query_user.prepare("INSERT INTO custom_user (LastName, FirstName, id) VALUES (?, ?, ?)");
                                    break;
                                case 4:
                                    query_user.prepare("INSERT INTO custom_user (City, LastName, FirstName, id) VALUES (?, ?, ?, ?)");
                                    break;
                                case 5:
                                    query_user.prepare("INSERT INTO custom_user (Address, City, LastName, FirstName, id) VALUES (?, ?, ?, ?, ?)");
                                    break;
                            }
                            switch(num1_colomn){
                                case 5:
                                    query_user.addBindValue(custom_Address[QRandomGenerator::global()->bounded(custom_Address.size())]
                                           + ' ' + QString::number(QRandomGenerator::global()->bounded(1, 345)));
                                    [[fallthrough]];
                                case 4:
                                    query_user.addBindValue(custom_City[QRandomGenerator::global()->bounded(custom_City.size())]);
                                    [[fallthrough]];
                                case 3:
                                    query_user.addBindValue(custom_LastName[QRandomGenerator::global()->bounded(custom_LastName.size())]);
                                    [[fallthrough]];
                                case 2:
                                    query_user.addBindValue(custom_FirstName[QRandomGenerator::global()->bounded(custom_FirstName.size())]);
                                    [[fallthrough]];
                                case 1:
                                    query_user.addBindValue(i);
                            }
                            if(query_user.exec())
                            {}
                    }
                    query_user_text->setText("select * from custom_user");
                    QSTM->setTable("custom_user");
                    QSTM->select();
                    delete form;
                    return true;
                }
            }
            qDebug() << query_user.lastError();
        }
    connect_to_database.close();
    delete form;
    return false;
}


