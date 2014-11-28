#include "ConnectionDialog.h"
#include "ui_ConnectionDialog.h"
#include <QHostAddress>

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
    QString hostName = QHostAddress( QHostAddress::LocalHost ).toString();
    ui->hostNameLineEdit->setText( hostName );

    QString portName( "12345" );
    ui->portNameLineEdit->setText( portName );
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_connectButton_clicked()
{
    QString hostName = ui->hostNameLineEdit->text();
    QString portName = ui->portNameLineEdit->text();
    emit signalConnection( hostName, portName );
    this->close();
}
