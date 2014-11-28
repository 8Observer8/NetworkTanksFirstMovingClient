#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QNetworkSession>
#include <QTcpSocket>
#include <QJsonDocument>

class Client : public QObject
{
    Q_OBJECT

public:
    Client();
    void connectToHost( const QString &hostName,
                        const QString &portName );
    void sendInfo( int x, int y, int direction );
    void setId( int id );
    int id() const;

signals:
    void signalShowId( int id );
    void signalUpdateAnotherTanks( const QJsonDocument &doc );
    void signalAddNewTank( const QJsonDocument &doc );
    void signalDeleteTank( const QJsonDocument &doc );
    void signalAddAllTanks( const QJsonDocument &doc );
    void signalSetInitPosition( const QJsonDocument &doc );

private slots:
    void sessionOpened();
    void slotReadyRead();
    void slotDisplayError( QAbstractSocket::SocketError socketError );

private:
    QNetworkSession *networkSession;
    QTcpSocket *socket;
    quint16 nextBlockSize;
    int m_id;
};

#endif // CLIENT_H
