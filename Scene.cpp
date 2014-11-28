#include "Scene.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

Scene::Scene( QWidget *parent ) :
    QGLWidget( parent )
{
    this->setFocusPolicy( Qt::StrongFocus );

    tank.setXY( -500, -436 );

    connect( &drawingTimer, SIGNAL( timeout() ),
             this, SLOT( updateGL() ) );
    drawingTimer.start( 5 );

    connect( &client, SIGNAL( signalShowId( int ) ),
             this, SLOT( slotShowId( int ) ) );

    connect( &client, SIGNAL( signalUpdateAnotherTanks( QJsonDocument ) ),
             this, SLOT( slotUpdateAnotherTanks( QJsonDocument ) ) );

    connect( &client, SIGNAL( signalAddNewTank( QJsonDocument ) ),
             this, SLOT( slotAddNewTank( QJsonDocument) ) );

    connect( &client, SIGNAL( signalDeleteTank( QJsonDocument ) ),
             this, SLOT( slotDeleteTank( QJsonDocument ) ) );

    connect( &client, SIGNAL( signalAddAllTanks( QJsonDocument ) ),
             this, SLOT( slotAddAllTanks( QJsonDocument ) ) );

    connect( &client, SIGNAL( signalSetInitPosition( QJsonDocument ) ),
             this, SLOT( slotSetInitPosition( QJsonDocument ) ) );

//    connect( &animationTimer, SIGNAL( timeout() ),
//             this, SLOT( slotUpdate() ) );
//    animationTimer.start( 50 );
}

void Scene::initializeGL()
{
    // Цвет для очистки буфера изображения - будет просто фон окна
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

    // Устанавливает режим проверки глубины пикселей
    glEnable( GL_DEPTH_TEST );

    // Отключает режим сглаживания цветов
    glShadeModel( GL_FLAT );

    // Устанавливаем режим, когда строятся только внешние поверхности
    glEnable( GL_CULL_FACE );

    glEnable( GL_TEXTURE_2D);

    genTextures();

    // Активизация массива вершин
    glEnableClientState( GL_VERTEX_ARRAY );

    // Активизация массива текстурных координат
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
}

void Scene::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    tank.draw();

    for ( auto &tank : m_clientTanks ) {
        tank.second->draw();
    }
}

void Scene::resizeGL( int w, int h )
{
    glViewport( 0, 0, w, h );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    static const int size = 500;

    GLfloat aspectRatio = ( GLfloat )w / ( GLfloat )h;
    if ( w <= h ) {
        glOrtho( -size, size, -size / aspectRatio, size / aspectRatio,
                  1, -1);
    } else {
        glOrtho( -size * aspectRatio, size * aspectRatio, -size, size,
                 1, -1 );
    }

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void Scene::keyPressEvent( QKeyEvent *event )
{
    static const unsigned int step = 3;
    switch( event->key() ) {
        case Qt::Key_Left:
            tank.setDirection( Tank::Left );
            tank.move( -step );
            client.sendInfo( tank.x(), tank.y(), static_cast<int>( tank.direction() ) );
            break;
        case Qt::Key_Right:
            tank.setDirection( Tank::Right );
            tank.move( step );
            client.sendInfo( tank.x(), tank.y(), static_cast<int>( tank.direction() ) );
            break;
        case Qt::Key_Down:
            tank.setDirection( Tank::Down );
            tank.move( -step );
            client.sendInfo( tank.x(), tank.y(), static_cast<int>( tank.direction() ) );
            break;
        case Qt::Key_Up:
            tank.setDirection( Tank::Up );
            tank.move( step );
            client.sendInfo( tank.x(), tank.y(), static_cast<int>( tank.direction() ) );
            break;
    }

    slotUpdate();
}

void Scene::genTextures()
{
    genFrames();

    for( size_t i = 0; i < framesToRight.size(); ++i ) {
        textureIdArrayToRight.push_back( bindTexture( framesToRight[i], GL_TEXTURE_2D ) );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }

    for( size_t i = 0; i < framesToLeft.size(); ++i ) {
        textureIdArrayToLeft.push_back( bindTexture( framesToLeft[i], GL_TEXTURE_2D ) );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }

    for( size_t i = 0; i < framesToDown.size(); ++i ) {
        textureIdArrayToDown.push_back( bindTexture( framesToDown[i], GL_TEXTURE_2D ) );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }

    for( size_t i = 0; i < framesToUp.size(); ++i ) {
        textureIdArrayToUp.push_back( bindTexture( framesToUp[i], GL_TEXTURE_2D ) );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }

    if ( framesToUp.size() == 4 ) {
        textureID = textureIdArrayToUp[0];
    }

    loadTexturesOnTanks();
}

void Scene::genFrames()
{
    QPixmap pixmap = QPixmap( QString( ":Textures/pla1.bmp" ) );
    const unsigned int step = pixmap.width() / 4;
    QPixmap copyPixmap;
    for ( unsigned int i = 0; i < 4; ++i ) {
        copyPixmap = pixmap.copy( i * step, 0, step, step );
        framesToRight.push_back( copyPixmap );
    }

    for( unsigned int i = 0; i < framesToRight.size(); ++i ) {
        QImage image = framesToRight[i].toImage();
        framesToLeft.push_back( QPixmap::fromImage( image.mirrored( true, false ) ) );
    }

    for( unsigned int i = 0; i < framesToRight.size(); ++i ) {
        QPixmap rotatedPixmap( framesToRight[i] );
        QMatrix rm;
        rm.rotate( 90 );
        framesToDown.push_back( rotatedPixmap.transformed( rm ) );
    }

    for( unsigned int i = 0; i < framesToDown.size(); ++i ) {
        QImage image = framesToDown[i].toImage();
        framesToUp.push_back( QPixmap::fromImage( image.mirrored( false, true ) ) );
    }
}

void Scene::loadTexturesOnTanks()
{
    tank.setTextureIdArrayToUp( textureIdArrayToUp );
    tank.setTextureIdArrayToRight( textureIdArrayToRight );
    tank.setTextureIdArrayToDown( textureIdArrayToDown );
    tank.setTextureIdArrayToLeft( textureIdArrayToLeft );

    for ( auto &tank : m_clientTanks ) {
        tank.second->setTextureIdArrayToUp( textureIdArrayToUp );
        tank.second->setTextureIdArrayToRight( textureIdArrayToRight );
        tank.second->setTextureIdArrayToDown( textureIdArrayToDown );
        tank.second->setTextureIdArrayToLeft( textureIdArrayToLeft );
    }
}

void Scene::slotUpdate()
{
//    static int i = 0;

//    if ( i == 4 ) {
//        i = 0;
//    }

//    switch( tank.direction() ) {
//        case Tank::Left:
//            textureID = textureIdArrayToLeft[i];
//            break;
//        case Tank::Up:
//            textureID = textureIdArrayToUp[i];
//            break;
//        case Tank::Right:
//            textureID = textureIdArrayToRight[i];
//            break;
//        case Tank::Down:
//            textureID = textureIdArrayToDown[i];
//            break;
//    }

//    ++i;

    // Animate tanks
    tank.animate();

    updateGL();
}

void Scene::slotConnection( const QString &hostName, const QString &portName )
{
    client.connectToHost( hostName, portName );
}

void Scene::slotShowId( int id )
{
    tank.setHandle( id );
    emit signalShowId( id );
}

void Scene::slotUpdateAnotherTanks( const QJsonDocument &doc )
{
    QJsonObject obj = doc.object();
    int id = obj["id"].toInt();
    int x = obj["x"].toInt();
    int y = obj["y"].toInt();
    int direction = obj["direction"].toInt();

    if ( tank.handle() == id ) {
        return;
    }

    m_clientTanks[id]->setXY( x, y );
    m_clientTanks[id]->setDirection( static_cast<Tank::Direction>( direction ) );
    m_clientTanks[id]->animate();
}

void Scene::slotAddNewTank( const QJsonDocument &doc )
{
    QJsonObject obj = doc.object();
    int id = obj["id"].toInt();
    int x = obj["x"].toInt();
    int y = obj["y"].toInt();
    if ( ( id != tank.handle() ) && ( m_clientTanks.count( id ) == 0 ) ) {
        m_clientTanks[id] = std::make_shared<Tank>( id, x, y );
        m_clientTanks[id]->setTextureIdArrayToUp( textureIdArrayToUp );
        m_clientTanks[id]->setTextureIdArrayToRight( textureIdArrayToRight );
        m_clientTanks[id]->setTextureIdArrayToDown( textureIdArrayToDown );
        m_clientTanks[id]->setTextureIdArrayToLeft( textureIdArrayToLeft );
    }
}

void Scene::slotDeleteTank( const QJsonDocument &doc )
{
    QJsonObject obj = doc.object();
    int handle = obj["id"].toInt();

    if ( tank.handle() != handle ) {
        m_clientTanks.erase( handle );
    }
}

void Scene::slotAddAllTanks( const QJsonDocument &doc )
{
    QJsonObject allTanksObj = doc.object();
    QJsonArray tankArray = allTanksObj["tanks"].toArray();
    for ( int tankIndex = 0; tankIndex < tankArray.size(); ++tankIndex ) {
        QJsonObject tankObject = tankArray[tankIndex].toObject();
        int id = tankObject["id"].toInt();
        if ( id == tank.handle() ) {
            continue;
        }
        int x = tankObject["x"].toInt();
        int y = tankObject["y"].toInt();
        m_clientTanks[id] = std::make_shared<Tank>( id, x, y );
        m_clientTanks[id]->setTextureIdArrayToUp( textureIdArrayToUp );
        m_clientTanks[id]->setTextureIdArrayToRight( textureIdArrayToRight );
        m_clientTanks[id]->setTextureIdArrayToDown( textureIdArrayToDown );
        m_clientTanks[id]->setTextureIdArrayToLeft( textureIdArrayToLeft );
    }
}

void Scene::slotSetInitPosition( const QJsonDocument &doc )
{
    QJsonObject positionObj = doc.object();
    int x = positionObj["x"].toInt();
    int y = positionObj["y"].toInt();
    tank.setXY( x, y );
}
