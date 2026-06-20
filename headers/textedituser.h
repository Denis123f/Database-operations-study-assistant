#ifndef TEXTEDITUSER_H
#define TEXTEDITUSER_H

#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpressionMatchIterator>
#include <QTimer>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
class SyntaxHighlighter;

class Textedituser : public QTextEdit
{
    Q_OBJECT
    SyntaxHighlighter* colours;
    QTimer* time_before_colour;


public:
    Textedituser(QWidget* parent = nullptr);

    bool detected_empty(const QString&);
private slots:

    void highlither_words();

};

class SyntaxHighlighter : public QSyntaxHighlighter
{
private:
    QString sqlGroup1;
    QString sqlGroup2;
    QString sqlGroup3;
    QString sqlGroup4;
    QString sqlGroup5;
    QString sqlComments;

    QTextCharFormat myClassFormat_group[6];
    QVector<QString> sql_groups;
    QVector<QColor> colours;
    QVector<QRegularExpression> expression;

public:
    SyntaxHighlighter( QTextDocument* parent );

    void highlightBlock( const QString& text );

    void setHighlightedString( const QString& str );

private:
    QString m_highlightedString;

};

#endif // TEXTEDITUSER_H
