#include "sql_working.h"

/*
A common static function that serves to connect to the database in almost the entire application (except for individual threads)
In it, we determine where the user wants to connect to and, depending on this, connect him using the previously specified data.

Save connections is an std::tuple that saves data about the current connection and is actively used to connect to the database.
*/
bool static unique_connection_always(QSqlDatabase& connect_to_database)
{
    if(connect_to_database.isOpen())
        connect_to_database.close();

    bool is_need_open = false;
    if(std::get<0>(save_connections) == "SQLITE")
    {
        if(QSqlDatabase::contains("QSQLITE_connect")){
            connect_to_database = QSqlDatabase::database("QSQLITE_connect");
        }
        else
        {
            connect_to_database = QSqlDatabase::addDatabase("QSQLITE", "QSQLITE_connect");
            is_need_open=true;
        }
        connect_to_database.setDatabaseName(std::get<1>(save_connections)[0]);
    }
    else if(std::get<0>(save_connections) == "PostgreSQL")
    {
        if(QSqlDatabase::contains("QPSQL_connect")){
            connect_to_database = QSqlDatabase::database("QPSQL_connect");
        }
        else
        {
            connect_to_database = QSqlDatabase::addDatabase("QPSQL", "QPSQL_connect");
            is_need_open=true;
        }
        connect_to_database.setPort((std::get<1>(save_connections)[0]).toInt());
        connect_to_database.setHostName(std::get<1>(save_connections)[1]);
        connect_to_database.setUserName(std::get<1>(save_connections)[2]);
        connect_to_database.setPassword(std::get<1>(save_connections)[3]);
        connect_to_database.setDatabaseName(std::get<1>(save_connections)[4]);
    }
    else if(std::get<0>(save_connections) == "RAM")
    {
        if(QSqlDatabase::contains("myUniqueConnection")){
            connect_to_database = QSqlDatabase::database("myUniqueConnection");
        }
        else
        {
            connect_to_database = QSqlDatabase::addDatabase("QSQLITE", "myUniqueConnection");
            is_need_open=true;
        }
    }
    if(is_need_open && !connect_to_database.open()){
        qDebug() << connect_to_database.lastError().text();
        return false;
    }
    return true;
}

Sql_working::Sql_working(QWidget *parent, User_WorkShop* user_WorkShop)
    : QWidget(parent), user_WorkShop(user_WorkShop)
{
    //  settings window  //

    table_user = new QTableView(this);
    table_user->setStyleSheet("background-color: #eef6ff;");
    table_user->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_user->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_user->verticalHeader()->setVisible(false);

    //  settings window  //

    QSTM = new QSqlTableModel(this, connect_to_database);
    QSQM = new QSqlQueryModel(this);


    /*
    We are implementing an additional thread for executing all requests except select.
    All executable queries are executed in a different thread.

    The implementation is taken from the Qt6 documentation at the link:
    https://doc.qt.io/qt-6/qthread.html
    */
    Worker* working_thread = new Worker();
    QThread* main_working_thread = new QThread();
    working_thread->moveToThread(main_working_thread);

    connect(main_working_thread, &QThread::finished, working_thread, &QObject::deleteLater);
    connect(this, &Sql_working::call_worker, working_thread, &Worker::asy_working);
    connect(working_thread, &Worker::result_Ready, this, &Sql_working::handleResults);

    main_working_thread->start();
}
// used in director_work in lambda-function for generation table
QSqlTableModel* Sql_working::get_QSTM()
{
    return QSTM;
}
// used in director_work in lambda-function for generation table and location_bd for gets tables
QSqlDatabase& Sql_working::get_connect()
{
    return connect_to_database;
}
// used in director_work as the main table.
QTableView* Sql_working::get_table_user()
{
    return table_user;
}
/*
We check the user_data file to see if the user has saved his past work.
If so, you first need to decode it, then open the database that he saved and return that everything is successful.

To do this, we use additional functions that saves save_connections, read above about this variable,
then it starts the connection and, depending on it, it returns the success or failure of the connection.
*/
QString Sql_working::last_connect()
{
    QString data_was_txt;

    QFile file(QCoreApplication::applicationDirPath() + "/user_data.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "file is open";
    }
    if(file.size() == 0)
    {
        file.close();
        return(connecting_database(std::make_tuple<QString, QVector<QString>>("RAM", {":memory:"})));
    }else{
        QTextStream reader(&file);
        QVector<QString> settings_user_is_for_connect;
        QChar need_is = reader.readLine()[0];
        if(need_is == '1')
        {
            std::vector<bool> vec_bol;
            QString line = reader.readLine();
            for(const QChar s : line)
            {
                if(s == '1') vec_bol.emplace_back(1);
                else{vec_bol.emplace_back(0);}
            }
            std::unique_ptr<Decoder_save_information> coder = std::make_unique<Decoder_save_information>(std::move(vec_bol));
            QString Database = coder->decode();
            qDebug() << "last_connect_access";
            QString data;
            vec_bol.clear();

            while (!reader.atEnd()) {
                line = reader.readLine();
                for(const QChar s : line)
                {
                    if(s == '1') vec_bol.emplace_back(1);
                    else{vec_bol.emplace_back(0);}
                }
                coder->replace_data(std::move(vec_bol));
                data = coder->decode();
                settings_user_is_for_connect.append(data.trimmed());
            }
            file.close();
            return(connecting_database(std::make_tuple(Database, settings_user_is_for_connect)));
        }else{
            file.close();
            return(connecting_database(std::make_tuple<QString, QVector<QString>>("RAM", {":memory:"})));
        }
    }
    file.close();
    return "";
}
// Auxiliary method for the last_connect
QString Sql_working::connecting_database(std::tuple<QString, QVector<QString>> as)
{
    save_connections = as;
    if(unique_connection_always(connect_to_database))
        return("Database: " + std::get<0>(save_connections) + " uploaded successfully");
    return ("Some error :(");
}
// A method for splitting all queries into separate queries so that they are supported by QSqlQuery
void Sql_working::splitBySemicolonSafe(const QString& str)
{
    QString help;
    for(qsizetype position = 0; position < str.length(); ++position)
    {
        if(str[position] == ';') {
            qu_query.push_back(help);
            help = "";
        }
        else if(str[position] == '-' && str[position+1] == '-')
        {
            while(str[position] != '\n' && position < str.length()-1)  position++;
        }
        else if(str[position] == '\t' || str[position] == '\n' || str[position] == '\r') continue;

        else{help+=str[position];}
    }

    if(!help.isEmpty()
            &&
       (qu_query.empty() || qu_query.back() != help))
    {
        qu_query.push_back(help);
    }
}

// A common method for assembling all the steps from checks to query execution.
void Sql_working::run_query()
{

    QString string_user_query = user_WorkShop->get_query_user_text()->toPlainText();

    qDebug() << string_user_query;

    if(!string_user_query.isEmpty())
    {
        // -------------- splitting the request by -------------- //

        splitBySemicolonSafe(string_user_query);
        if(qu_query.empty())
        {
            view_event("Where is query?");
            return;
        }

        if(qu_query.front().contains("Error with parse:"))
        {
            QString error_parse = qu_query.front();
            qu_query.pop_front();
            view_event(error_parse);
            return;
        }
        // -------------- splitting the request by -------------- //

        emit call_worker(connect_to_database);

    }else{
         view_event("Where is text?");
         return;
    }
    qDebug() << "qu_qeury: ";
    for(auto& sagfd: qu_query) qDebug() << sagfd << '\n';
}

Worker::Worker(QObject* parent) : QObject(parent)
{}
// The main method for executing all requests. All select calls are redirected to the main thread.
void Worker::asy_working(QSqlDatabase& connect_to_database)
{
    QString query;

    while(!qu_query.empty())
    {
        query = qu_query.front();
        if(query.contains("select", Qt::CaseInsensitive))
        {
            query = "select";
            emit result_Ready(query);
            return;
        }
        QSqlQuery QSQ(connect_to_database);

        if(QSQ.exec(query))
        {
            qu_query.pop_front();
        }else
        {

            while(!qu_query.empty())
            {
                qu_query.pop_front();
            }
            query = "Error: ";
            emit result_Ready(query + QSQ.lastError().text());
            return;
        }
    }
    emit result_Ready("");
    return;
}
// Method for executing select requests
void Sql_working::handleResults(const QString& recieve_result)
{
    if(recieve_result == "select")
    {
        QSqlQuery qsq(connect_to_database);

        if(qsq.exec(qu_query.front()))
        {
            qDebug() << "good select";
            QSQM = new QSqlQueryModel(this);
            QSQM->setQuery(qu_query.front(), connect_to_database);
            table_user->setModel(QSQM);
            qu_query.pop_front();
            emit call_worker(connect_to_database);
        }
        else{
            view_event(qsq.lastError().text());
            while(!qu_query.empty())
            {
                qu_query.pop_front();
            }
        }
    }
    else if (recieve_result.contains("Error:"))
    {
        view_event(recieve_result);
    }
    else
    {
       view_event("Query is right !");
    }
}


