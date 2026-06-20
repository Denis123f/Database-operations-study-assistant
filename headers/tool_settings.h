#ifndef TOOL_SETTINGS_H
#define TOOL_SETTINGS_H

#include <decoder_save_information.h>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <tuple>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QCheckBox>

class Tool_settings : public QWidget
{
    Q_OBJECT
    QPushButton* tool_settings;
    QPushButton* tree_database;
    QHBoxLayout* tools_bar;
    QString base_border;
    QString error_border;

public:
    Tool_settings(QWidget* parent = nullptr);

    QPushButton* get_tree_database();
    QPushButton* get_settings();
    void location_BD();
    [[nodiscard]] std::tuple<bool, QString, QVector<QString>> setting_menu();
public slots:
    bool non_duplicate_error_dorder(QLineEdit*);
    void write_is_in_file(const QVector<QString>&);
};



#endif // TOOL_SETTINGS_H
