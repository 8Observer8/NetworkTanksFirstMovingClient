#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

signals:
    void signalConnection( const QString &hostName,
                           const QString &portName );

private slots:
    void on_actionConnection_triggered();
    void on_actionExit_triggered();
    void slotConnection( const QString &hostName,
                         const QString &portName );
    void slotShowId( int id );

private:
    Ui::MainWindow *ui;
    QLabel *label;
};

#endif // MAINWINDOW_H
