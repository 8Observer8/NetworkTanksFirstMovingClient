#include <QNetworkConfigurationManager>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include "Client.h"

Client::Client() :
    networkSession( 0 ),
    socket( 0 ),
    nextBlockSize( 0 ),
    m_id( 1000 )
{
    QNetworkConfigurationManager manager;
    if ( manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired ) {
        // Get saved network configuration
        QSettings settings( QSettings::UserScope, QLatin1String( "QtProject" ) );
        settings.beginGroup( QLatin1String( "QtNetwork" ) );
        const QString id = settings.value( QLatin1String("DefaultNetworkConfiguration" ) ).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier( id );
        if ( ( config.state() & QNetworkConfiguration::Discovered ) !=
             QNetworkConfiguration::Discovered ) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession( config, this );
        connect( networkSession, SIGNAL( opened() ), this, SLOT( sessionOpened()) );

        //statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    } else {
        sessionOpened();
    }
}

void Client::connectToHost(const QString &hostName, const QString &portName)
{
    socket = new QTcpSocket( this );

    connect( socket, SIGNAL( readyRead() ), this, SLOT( slotReadyRead() ) );
    connect( socket, SIGNAL( error( QAbstractSocket::SocketError ) ),
             this, SLOT( slotDisplayError( QAbstractSocket::SocketError ) ) );

    socket->connectToHost( hostName, portName.toInt() );
}

void Client::sendInfo( int x, int y , int direction )
{
    if ( socket == 0 ) {
        return;
    }

    QByteArray  arrBlock;
    QDataStream out( &arrBlock, QIODevice::WriteOnly );
    out.setVersion( QDataStream::Qt_5_3 );

    QJsonObject info;
    info["name"] = QString( "NewCoordinates" );
    info["id"] = m_id;
    info["x"] = x;
    info["y"] = y;
    info["direction"] = direction;
    QJsonDocument doc( info );
    out << quint16( 0 ) << doc.toBinaryData();

    // Write a block size
    out.device()->seek( 0 );
    out << quint16( arrBlock.size() - sizeof( quint16 ) );

    // Send data
    socket->write( arrBlock );
}

void Client::setId( int id )
{
    m_id = id;
}

int Client::id() const
{
    return m_id;
}

void Client::slotReadyRead()
{
    QTcpSocket* clientSocket = ( QTcpSocket* )sender();
    QDataStream in( clientSocket );
    in.setVersion( QDataStream::Qt_5_3 );
    for ( ;; ) {
        if ( !nextBlockSize ) {
            if ( clientSocket->bytesAvailable() < ( int )sizeof( quint16 ) ) {
                break;
            }
            in >> nextBlockSize;
        }

        if ( clientSocket->bytesAvailable() < nextBlockSize ) {
            break;
        }

        // Set an id
        QByteArray data;
        in >> data;

        QJsonDocument doc;
        doc = doc.fromBinaryData( data );

        QJsonObject obj;
        obj = doc.object();
        QString name = obj["name"].toString();
        if ( name == QString( "id" ) ) {
            int id = obj["id"].toInt();
            setId( id );
            emit signalShowId( id );
        } if ( name == QString( "AddNewTank" ) ) {
            emit signalAddNewTank( doc );
        } if ( name == QString( "DeleteTank" ) ) {
            emit signalDeleteTank( doc );
        } if ( name == QString( "AllTanks" ) ) {
            emit signalAddAllTanks( doc );
        } if ( name == QString( "InitPosition" ) ) {
            emit signalSetInitPosition( doc );
        } if ( name == QString( "NewCoordinates" ) ) {
            emit signalUpdateAnotherTanks( doc );
        }

        nextBlockSize = 0;
    }
}

void Client::slotDisplayError( QAbstractSocket::SocketError socketError )
{
    switch ( socketError ) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug() << "The host was not found. Please check the "
                        "host name and port settings.";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug() << "The connection was refused by the peer. "
                        "Make sure the fortune server is running, "
                        "and check that the host name and port "
                        "settings are correct.";
            break;
        default:
            qDebug() << QString( "The following error occurred: %1." )
                        .arg( socket->errorString() );
    }
}

void Client::sessionOpened()
{
    // Save the used configuration
    if ( networkSession ) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if ( config.type() == QNetworkConfiguration::UserChoice ) {
            id = networkSession->sessionProperty( QLatin1String( "UserChoiceConfiguration" ) ).toString();
        } else {
            id = config.identifier();
        }

        QSettings settings( QSettings::UserScope, QLatin1String( "QtProject" ) );
        settings.beginGroup( QLatin1String( "QtNetwork" ) );
        settings.setValue( QLatin1String( "DefaultNetworkConfiguration" ), id );
        settings.endGroup();
    }
}
