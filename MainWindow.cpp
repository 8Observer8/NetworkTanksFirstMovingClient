#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ConnectionDialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize( this->size() );
    connect( this, SIGNAL( signalConnection( QString,QString ) ),
             ui->widget, SLOT( slotConnection( QString,QString ) ) );

    label = new QLabel( this );
    ui->statusBar->addPermanentWidget( label, 1 );
    connect( ui->widget, SIGNAL( signalShowId( int ) ),
             this, SLOT( slotShowId( int ) ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnection_triggered()
{
    ConnectionDialog dialog;
    dialog.setModal( true );
    connect( &dialog, SIGNAL( signalConnection( QString,QString ) ),
             this, SLOT( slotConnection( QString, QString ) ) );
    dialog.exec();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::slotConnection( const QString &hostName, const QString &portName )
{
    ui->actionConnection->setEnabled( false );
    emit signalConnection( hostName, portName );
}

void MainWindow::slotShowId( int id )
{
    label->setText( QString( "Connected id = %1" ).arg( id ) );
}
