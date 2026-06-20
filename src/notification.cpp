#include "notification.h"

#define sz_widget_information 30
// Auxiliary functions for a more simplified window setup.
inline void removeBorders(QWidget* widg)
{
    widg->setContentsMargins(0,0,0,0);
}
inline void removeBorders(QLayout* lay)
{
    lay->setContentsMargins(0,0,0,0);
}

void view_event(const QString& c, QWidget*parent)
{
    if(!main_parent_window)
        main_parent_window = parent;

    Notification* cav = nullptr;

    for(qsizetype pos = 1; pos < QVec_Notification.size(); ++pos)
    {
        if(QVec_Notification[pos] == 0)
        {
            cav = new Notification(std::move(sz_widget_information + (sz_widget_information * pos)), main_parent_window);
            QVec_Notification[pos] = 1;
            break;
        }
    }
    if(!cav)
    {
        cav = new Notification(std::move(sz_widget_information), main_parent_window);
        cav->create_event("Stop spamming pls");
    }
    else{
        cav->create_event(c);
    }
    QTimer* tim = new QTimer();
    tim->setSingleShot(true);
    tim->start(6000);
    QObject::connect(tim, &QTimer::timeout, [tim, cav](){
        QVec_Notification[(cav->local_size_y - sz_widget_information) / sz_widget_information] = 0;
        cav->deleteLater();
        tim->deleteLater();
    });

}

Notification::Notification(qsizetype&& size_y, QWidget* parent) : QWidget(parent), local_size_y(std::move(size_y))
{
    // settings window //

    setGeometry(-100, 100, 100, 100);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //removeBorders(this);

    setStyleSheet(
        "QWidget {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ff7e5f, stop:1 #feb47b);"
        "   border-radius: 20px;"
        "   border: none;"
        "}"
    );

    // settings window //


    QVBL = new QVBoxLayout();
    lab = new QLabel(this);
    lab->setAlignment(Qt::AlignCenter);

    QVBL->addWidget(lab);

    setLayout(QVBL);

    show();
}

/*
It is used to animate the window that appears to inform the user about the events
that have ended \ about the problem with the request.
*/

void Notification::create_event(const QString &c) noexcept
{
    lab->setText(c);
    resize(lab->sizeHint().width()+24, lab->sizeHint().height()+24);

    /*
    сделать прозрачность для красоты, подделать квадрта до правильной формы.
    */

    QPropertyAnimation* anim = new QPropertyAnimation(this, "pos");
    anim->setEasingCurve(QEasingCurve::OutCubic);

    anim->setDuration(3000);
    anim->setStartValue(QPoint(-(lab->sizeHint().width()+24), local_size_y));
    anim->setEndValue(QPoint(150, local_size_y));
    anim->start();

    connect(anim, &QPropertyAnimation::finished, this, [this](){
        QPropertyAnimation* anima = new QPropertyAnimation(this, "pos");
        anima->setEasingCurve(QEasingCurve::OutCubic);
        anima->setDuration(3000);
        anima->setStartValue(QPoint(150, local_size_y));
        anima->setEndValue(QPoint(-(lab->sizeHint().width()+24), local_size_y));
        anima->start();

    });
}
