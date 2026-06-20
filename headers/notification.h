#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QAbstractAnimation>

static QWidget* main_parent_window;
static QVector<bool> QVec_Notification(9, 0);

void view_event(const QString& c, QWidget*parent=main_parent_window);

class Notification : public QWidget
{
    Q_OBJECT
    QVBoxLayout* QVBL;
    QLabel* lab;
    QPropertyAnimation* anim;

    void create_event(const QString& c) noexcept;
public:
    qsizetype local_size_y;
    Notification(qsizetype&& size_y, QWidget* parent = nullptr);
    friend void view_event(const QString& c, QWidget*parent);

};

#endif // NOTIFICATION_H
