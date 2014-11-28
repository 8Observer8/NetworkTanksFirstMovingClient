#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <memory>
#include <QGLWidget>
#include <QKeyEvent>
#include <QTimer>
#include <QJsonDocument>
#include "Tank.h"
#include "Client.h"

//enum Direction { Left, Down, Right, Up };

class Scene : public QGLWidget
{
    Q_OBJECT

public:
    Scene( QWidget *parent = 0 );

signals:
    void signalShowId( int id );

private slots:
    void slotUpdate();
    void slotConnection( const QString &hostName,
                         const QString &portName );
    void slotShowId( int id );
    void slotUpdateAnotherTanks( const QJsonDocument &doc );
    void slotAddNewTank( const QJsonDocument &doc );
    void slotDeleteTank( const QJsonDocument &doc );
    void slotAddAllTanks( const QJsonDocument &doc );
    void slotSetInitPosition( const QJsonDocument &doc );

private:
    void initializeGL();
    void paintGL();
    void resizeGL( int w, int h );

    void keyPressEvent( QKeyEvent *event );
    void genTextures();
    void genFrames();

    void loadTexturesOnTanks();

private:
    Tank tank;
    std::map< int, std::shared_ptr<Tank> > m_clientTanks;
    unsigned int textureID;
    std::vector<unsigned int> textureIdArrayToRight;
    std::vector<unsigned int> textureIdArrayToLeft;
    std::vector<QPixmap> framesToRight;
    std::vector<QPixmap> framesToLeft;
    std::vector<unsigned int> textureIdArrayToDown;
    std::vector<unsigned int> textureIdArrayToUp;
    std::vector<QPixmap> framesToDown;
    std::vector<QPixmap> framesToUp;
    QTimer drawingTimer;
    QTimer animationTimer;
    Client client;
};

#endif // SCENE_H
