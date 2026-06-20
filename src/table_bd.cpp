#include "table_bd.h"

// Auxiliary functions for a more simplified window setup.
inline void removeBorders(QWidget* widg)
{
    widg->setContentsMargins(0,0,0,0);
}
inline void removeBorders(QLayout* lay)
{
    lay->setContentsMargins(0,0,0,0);
}
/*
The basic configuration of the window is performed in the constructor,
a table is created with the name of the table itself and the rows in it.
*/
Table_BD::Table_BD(const QSqlRecord&& move, QWidget* parent) : QWidget(parent)
{
    // settings window //

    removeBorders(this);
    this->setStyleSheet("");
    this->resize(200, 200);

    // settings window //

    QVBL = new QVBoxLayout(this);
    QVBL->setContentsMargins(0, 0, 0, 0);
    QVBL->setSpacing(0);


    table = new QTableView(this);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->resizeColumnsToContents();
    table->setTextElideMode(Qt::ElideNone);
    table->setAttribute(Qt::WA_TransparentForMouseEvents);

    QSqlField tst = move.field(0);
    QStandardItemModel* obj = new QStandardItemModel(move.count(), 1, this);
    obj->setHeaderData(0, Qt::Horizontal, tst.tableName());

    for(int j = 0; j < move.count(); j++)
    {
        tst = move.field(j);
        obj->setItem(j, 0, new QStandardItem(tst.name()));
    }
    table->setModel(obj);
    QVBL->addWidget(table);
}

// ДОДЕЛАТЬ СТРЕЛОЧКИ !

// Processing the movement of tables inside the window
void Table_BD::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        dragOffset = event->position();
    }
}
// Processing the movement of tables inside the window
void Table_BD::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
            QPointF parentLocalPos = parentWidget()->mapFromGlobal(event->globalPosition()),
                    newPosF = parentLocalPos - dragOffset;

            QRect parentRect = parentWidget()->rect(),
                  widgetRect(newPosF.toPoint(), size());

            if (widgetRect.left() < parentRect.left()) {
                newPosF.setX(parentRect.left());
            } else if (widgetRect.right() > parentRect.right()) {
                newPosF.setX(parentRect.right() - width() + 1);
            }

            if (widgetRect.top() < parentRect.top()) {
                newPosF.setY(parentRect.top());
            } else if (widgetRect.bottom() > parentRect.bottom()) {
                newPosF.setY(parentRect.bottom() - height() + 1);
            }

            move(newPosF.toPoint());
    }
}

// Processing the movement of tables inside the window
void Table_BD::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
    }
}

// Minimum window setting
Viewing_the_tree::Viewing_the_tree(QWidget *parent) : QWidget(parent)
{
    // settings window //

    resize(400, 400);
    setWindowTitle("Tables");
    removeBorders(this);

    // settings window //
}
/*
The method is responsible for the location of all the windows with their tables.
They are placed in a plate of 5.
*/
void Viewing_the_tree::show_replase()
{
    int x_pos = 0, y_pos = 0;
    for(int i = 0; i < replase.size(); ++i)
    {
        if(y_pos == 5)
        {
            y_pos=0;
            x_pos+=200;
        }
        replase[i]->setGeometry(x_pos, y_pos++ * 200, 200, 200);
    }
}
// Creating windows obtained from director work
void Viewing_the_tree::add_table(const QSqlRecord&& move)
{
    Table_BD* obj = new Table_BD(std::move(move), this);

    replase.push_back(obj);

}
