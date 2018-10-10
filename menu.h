#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QMenuBar>
#include <QMessageBox>

struct file_t{
    QAction* Open;
    QAction* Save;
    QAction* Exit;
};

struct help_t{
    QAction* About;
};

struct action_t{
    file_t file;
    help_t help;
};

class menu : public QWidget
{
    Q_OBJECT
public:
    explicit menu(QWidget *parent = nullptr);
    QMenuBar* bar;

signals:

public slots:
void slot_about();
void slot_exit();

private slots:


private:
    QMenu* File;
    QMenu* Settings;
    QMenu* Help;
    QWidget* mainWindow;

};

#endif // MENU_H
