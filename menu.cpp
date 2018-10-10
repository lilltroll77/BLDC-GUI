#include "menu.h"
#include <QDebug>
#include <QList>
#include <QtWidgets>
#include "serial_com.h"

extern QSerialPort serial;

menu::menu(QWidget *parent) : QWidget(parent)
{
    mainWindow = parent;

    bar = new QMenuBar(parent);
    File = new QMenu("File", parent);
    Settings = new QMenu("Settings" , parent);
    Help = new QMenu("Help" , parent);
    action_t action;

    action.file.Open = new QAction("Open");
    action.file.Open->setEnabled(false);
    File->addAction(action.file.Open);

    action.file.Save = new QAction("Save");
    action.file.Save->setEnabled(false);
    File->addAction(action.file.Save);

    action.file.Exit = new QAction("Exit");
    connect(action.file.Exit , SIGNAL(triggered()) , this , SLOT(slot_exit()));
    File->addAction(action.file.Exit);

    action.settings.serial = new QAction("Serial port");
    connect(action.settings.serial , SIGNAL(triggered()) , this , SLOT(slot_serialPort()));
    Settings->addAction(action.settings.serial);

    action.help.About = new QAction("About");
    connect(action.help.About , SIGNAL(triggered()) , this , SLOT(slot_about()));
    Help->addAction(action.help.About);



    bar->addMenu(File);
    bar->addMenu(Settings);
    bar->addMenu(Help);




}

void menu::slot_about(){
    qDebug() << "about";
    QMessageBox msgBox;
    msgBox.setWindowTitle("About");
    QString str = QString("Written by Mikael Bohman\nCompiled: %1 \nThis program is provided AS IS with NO WARRANTY OF ANY KIND").arg(__DATE__);
    msgBox.setText(str);
    //msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Discard);
    msgBox.exec();
}

void menu::slot_serialPort(){
    QSerialPortInfo info;
    ports =info.availablePorts();
    msg.setWindowTitle("Set COM port");

    for(int i=0; i<ports.length() ; i++)
        comboBox.addItem(ports[i].portName());
    QVBoxLayout layout;
    QLabel label;
    QPushButton button;
    button.setMaximumWidth(60);
    button.setText("OK");
    label.setText("Choose XMOS virtual COM port");
    layout.addWidget(&label);
    layout.addWidget(&comboBox);
    layout.addWidget(&button);
    msg.setLayout(&layout);
    msg.setMinimumWidth(200);
    comboBox.setMaximumWidth(120);
    msg.setMinimumHeight(100);
    connect(&button , SIGNAL(released()) , this , SLOT(slot_setPort()));
    msg.exec();


}

void menu::slot_setPort(){
    msg.close();
    int i = comboBox.currentIndex();
    emit COMportSelected(ports[i]);
}

void menu::slot_exit(){
    mainWindow->close();
}
