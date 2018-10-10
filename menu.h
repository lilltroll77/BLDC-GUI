#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QMenuBar>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QComboBox>
#include "currentgague.h"

struct file_t{
    QAction* Open;
    QAction* Save;
    QAction* Exit;
};

struct help_t{
    QAction* About;
};

struct settings_t{
    QAction* serial;
};

struct action_t{
    file_t file;
    help_t help;
    settings_t settings;
};

class menu : public QWidget
{
    Q_OBJECT
public:
    explicit menu(QWidget *parent = nullptr);
    QMenuBar* bar;

signals:
    void COMportSelected(QSerialPortInfo port);

public slots:
void slot_about();
void slot_exit();
void slot_serialPort();
void slot_setPort();

private slots:


private:
    QMenu* File;
    QMenu* Settings;
    QMenu* Help;
    QWidget* mainWindow;
    QList<QSerialPortInfo> ports;
    QDialog msg;
    QComboBox comboBox;
    currentGague* currentgague;

};

#endif // MENU_H
