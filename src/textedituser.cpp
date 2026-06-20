#include "textedituser.h"

// Сделать адекватную обработку ошибок
// починить появление окна на экране

Textedituser::Textedituser(QWidget* parent) : QTextEdit(parent)
{
    // settings window //

    QFont font("TimesNewRoman", 14);
    font.setStyleHint(QFont::TypeWriter);

    QTextCharFormat format;
    format.setForeground(Qt::black);
    format.setBackground(Qt::transparent);

    setFont(font);
    setCurrentCharFormat(format);
    setAcceptRichText(false);

    // settings window //

    // Spacing is responsible for checking keywords in the text to color them.
    time_before_colour = new QTimer(this);
    time_before_colour->setInterval(400);


    colours = new SyntaxHighlighter(this->document());
    connect(this, &QTextEdit::textChanged, this, &Textedituser::highlither_words);
}

bool Textedituser::detected_empty(const QString& test_str)
{   
    QMessageBox::information(window(), "Info", test_str);
    return false;
}


void Textedituser::highlither_words()
{
    colours->setHighlightedString(this->toPlainText());
    time_before_colour->start();
}


// ---------------------------------------- //

SyntaxHighlighter::SyntaxHighlighter( QTextDocument* parent ) :
    QSyntaxHighlighter( parent )
{
    // Объект 1: DDL + DML + DQL
    sqlGroup1 = "\\b(CREATE|ALTER|DROP|TABLE|INDEX|VIEW|TRIGGER|"
                        "DATABASE|TEMP|TEMPORARY|IF|EXISTS|NOT|NULL|"
                        "PRIMARY|KEY|FOREIGN|UNIQUE|CHECK|DEFAULT|"
                        "AUTOINCREMENT|WITHOUT|ROWID|"
                        "INSERT|UPDATE|DELETE|REPLACE|INTO|VALUES|SET|"
                        "ON|CONFLICT|DO|NOTHING|ROLLBACK|ABORT|FAIL|IGNORE|"
                        "SELECT|FROM|WHERE|GROUP|BY|HAVING|ORDER|"
                        "LIMIT|OFFSET|DISTINCT|ALL|AS|JOIN|INNER|"
                        "LEFT|RIGHT|FULL|CROSS|NATURAL|USING|UNION|"
                        "INTERSECT|EXCEPT|WITH|RECURSIVE|DESK|ASK)\\b";

    // Объект 2: TCL
    sqlGroup2 = "\\b(BEGIN|START|TRANSACTION|COMMIT|END|ROLLBACK|"
                        "SAVEPOINT|RELEASE)\\b";

    // Объект 3: Логические операторы и условия
    sqlGroup3 = "\\b(AND|OR|NOT|BETWEEN|CASE|WHEN|THEN|ELSE|"
                        "END|IN|IS|LIKE|GLOB|MATCH|REGEXP|ESCAPE|"
                        "TRUE|FALSE|UNKNOWN|EXISTS|ISNULL|NOTNULL)\\b";

    // Объект 4: Функции и агрегаты
    sqlGroup4 = "\\b(COUNT|SUM|TOTAL|AVG|MIN|MAX|GROUP_CONCAT|"
                        "ABS|CHANGES|CHAR|COALESCE|GLOB|HEX|IFNULL|"
                        "IIF|INSTR|LAST_INSERT_ROWID|LENGTH|LIKE|LIKELIHOOD|"
                        "LIKELY|LOAD_EXTENSION|LOWER|LTRIM|MAX|MIN|NULLIF|"
                        "PRINTF|QUOTE|RANDOM|RANDOMBLOB|REPLACE|ROUND|"
                        "RTRIM|SIGN|SOUNDEX|SQLITE_COMPILEOPTION_GET|"
                        "SQLITE_COMPILEOPTION_USED|SQLITE_OFFSET|SQLITE_SOURCE_ID|"
                        "SQLITE_VERSION|SUBSTR|TOTAL_CHANGES|TRIM|TYPEOF|"
                        "UNICODE|UPPER|ZEROBLOB)\\b";

    // Объект 5: Прочие ключевые слова
    sqlGroup5 = "\\b(ABORT|ACTION|ANALYZE|ATTACH|AUTOINCREMENT|BEFORE|"
                        "CASCADE|CONFLICT|CURRENT_DATE|CURRENT_TIME|"
                        "CURRENT_TIMESTAMP|DATABASE|DEFERRABLE|DEFERRED|"
                        "DETACH|EACH|FAIL|FILTER|FOLLOWING|GENERATED|"
                        "GLOB|IGNORE|IMMEDIATE|INITIALLY|INSTEAD|KEY|"
                        "MATCH|NO|OVER|PARTITION|PLAN|PRAGMA|QUERY|"
                        "RAISE|REINDEX|RENAME|REPLACE|RESTRICT|ROW|"
                        "ROWS|SCHEMA|SEQUENCE|STATEMENT|TABLE|TEMP|"
                        "TEMPORARY|TIES|UNBOUNDED|VACUUM|VIRTUAL|WITHOUT)\\b";

    sqlComments = "(#|--)[^\\r\\n]*"
                  "|/\\*.*?\\*/";

    sql_groups.push_back(sqlGroup1);
    sql_groups.push_back(sqlGroup2);
    sql_groups.push_back(sqlGroup3);
    sql_groups.push_back(sqlGroup4);
    sql_groups.push_back(sqlGroup5);
    sql_groups.push_back(sqlComments);

    colours.push_back(QColor("#0000FF")); // 1 Salmon
    colours.push_back(QColor("#FF69B04")); // 2 HotPink
    colours.push_back(QColor("#00FF00")); // 3 Lime
    colours.push_back(QColor("#EE82EE")); // 4 Violet
    colours.push_back(QColor("#D2691E")); // 5 Chocolate
    colours.push_back(QColor("gray"));
    // Красный, черный, синий\голубой, зеленый, фиол\роз

    for(size_t i = 0; i < 6; ++i)
    {
        myClassFormat_group[i].setForeground(colours[i]);
    }
    for(int i = 0; i < 5; ++i)
    {
        expression.emplaceBack(QRegularExpression(sql_groups[i], QRegularExpression::CaseInsensitiveOption));
    }
    expression.emplaceBack(sqlComments);
}

void SyntaxHighlighter::highlightBlock( const QString& text )
{
    for(int i = 0; i < 5; ++i)
    {
        QRegularExpressionMatchIterator as = expression[i].globalMatch(text);
        while (as.hasNext()) {
            QRegularExpressionMatch match = as.next();
            setFormat(match.capturedStart(), match.capturedLength(), myClassFormat_group[i]);
        }
    }
    QRegularExpressionMatchIterator as = expression[5].globalMatch(text);
    while (as.hasNext()) {
        QRegularExpressionMatch match = as.next();
        setFormat(match.capturedStart(), match.capturedLength(), myClassFormat_group[5]);
    }

}

void SyntaxHighlighter::setHighlightedString( const QString& ) {

}
