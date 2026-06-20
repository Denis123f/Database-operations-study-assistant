#include "director_work.h"

// Auxiliary functions for a more simplified window setup.
inline void removeBorders(QWidget* widg)
{
    widg->setContentsMargins(0,0,0,0);
}
inline void removeBorders(QLayout* lay)
{
    lay->setContentsMargins(0,0,0,0);
}

Director_work::Director_work(QWidget *parent)
    : QWidget(parent)
{
    // settings main window

    setWindowTitle("TogWork");

    // settings main window

    user_WorkShop = new User_WorkShop(this);
    main_window = new QVBoxLayout();
    tool_settings = new Tool_settings(this);
    jbd=nullptr;

    sql_main_work = new Sql_working(this, user_WorkShop);

    removeBorders(main_window);
    main_window->setSpacing(0);

    setting_window(); // Setting up size and parameters the window

    setLayout(main_window);
}

void Director_work::setting_window()
{
    // Location and size

    QSize size_user_screen = QApplication::primaryScreen()->size();

    int pos_axis_y = size_user_screen.height() / 10.5;
    int pos_axis_x = size_user_screen.width() / 5;
    int size_axys_y =  pos_axis_y * 5;
    int size_axys_x = pos_axis_x * 3;
    setGeometry(pos_axis_x, pos_axis_y, size_axys_x, size_axys_y);

    // Create tools bar and setting widgets

    main_window->addWidget(tool_settings, 0);
    tool_settings->raise();

    // Create user_workshop and his is next setting

    main_window->addWidget(user_WorkShop);

    // placement and configuration database

    main_window->addWidget(sql_main_work->get_table_user());

    // check was work
    view_event(sql_main_work->last_connect(), this);

    // Blocks connects of buttons

    connect(tool_settings->get_settings(), &QPushButton::clicked, this, [this](){
        std::tuple<bool, QString, QVector<QString>> as = tool_settings->setting_menu();
        qDebug() << std::get<0>(as);
        if(std::get<0>(as)){
            view_event(sql_main_work->connecting_database(std::make_pair(std::get<1>(as), std::get<2>(as))));
        }
        });

    // Get a view of all tables in an open database
    connect(tool_settings->get_tree_database(), &QPushButton::clicked, this, &Director_work::location_BD);

    connect(user_WorkShop->get_run_query(), &QPushButton::clicked, sql_main_work, [this]()
    {
        sql_main_work->run_query();
    });
    connect(user_WorkShop->get_clear_text_user(), &QPushButton::clicked, this, [this]()
    {
        user_WorkShop->get_query_user_text()->clear();
        view_event("all text cleaned!");

    });

    // Generate a test database for training purposes or to test the operation of the application /debugging.
    connect(user_WorkShop->get_generation_table(), &QPushButton::clicked,this, [this]()
    {
        if(user_WorkShop->user_generation_table(sql_main_work->get_QSTM(), sql_main_work->get_connect()))
        {
            view_event("table is generationing!");
        }
    });
}
// The method handles the construction of all database tables.
void Director_work::location_BD()
{
    QStringList name_tables_bd = sql_main_work->get_connect().tables();
    if(name_tables_bd.size() == 0) return;

    if(!jbd)
    {
        jbd = new Viewing_the_tree();
        connect(jbd, &QWidget::destroyed, [this](){jbd = nullptr;});
    }else
    {
        jbd->raise();
        return;
    }
    jbd->setAttribute(Qt::WA_DeleteOnClose);
    for(int i = 0; i < name_tables_bd.size(); ++i)
    {
        jbd->add_table(std::move(sql_main_work->get_connect().record(name_tables_bd[i])));
    }
    jbd->show_replase();
    jbd->show();
}
Director_work::~Director_work()
{
}





