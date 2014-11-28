#include <QString>
#include "Tank.h"

Tank::Tank( int handle , int x, int y ) :
    size( 32 ),
    m_direction( Tank::Up ),
    m_handle( handle ),
    m_x( x ),
    m_y( y )
{
    init();
    setXY( m_x, m_y );
}

void Tank::draw( ) const
{
    glBindTexture( GL_TEXTURE_2D, m_textureID );

    // Указываем, откуда нужно извлечь данные о массиве вершин
    glVertexPointer( 3, GL_INT, 0, vertices.data() );

    // Указываем, откуда нужно извлечь данные о текстурных координатах
    glTexCoordPointer( 2, GL_FLOAT, 0, textures.data() );

    // Используя массивы вершин и индексов, строим поверхности
    glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data() );
}

void Tank::move( int step )
{
    switch ( m_direction ) {
        case Up:
            for ( size_t i = 1; i < vertices.size(); i += 3 ) {
                vertices[i] += step;
            }
            break;
        case Left:
            for ( size_t i = 0; i < vertices.size(); i += 3 ) {
                vertices[i] += step;
            }
            break;
        case Down:
            for ( size_t i = 1; i < vertices.size(); i += 3 ) {
                vertices[i] += step;
            }
            break;
        case Right:
            for ( size_t i = 0; i < vertices.size(); i += 3 ) {
                vertices[i] += step;
            }
            break;
    }
}

void Tank::moveTo( int x, int y )
{
    // Первый треугольник
    // 0
    vertices[0] = x;                // X
    vertices[1] = y - size * 2;     // Y

    // 1
    vertices[3] = x + size * 2;     // X
    vertices[4] = y - size * 2;     // Y

    // 2
    vertices[6] = x;                // X
    vertices[7] = y;                // Y

    // Второй треугольник
    // 3
    vertices[9] = x;                // X
    vertices[10] = y;               // Y

    // 4
    vertices[12] = x + size * 2;     // X
    vertices[13] = y - size * 2;     // Y

    // 5
    vertices[15] = x + size * 2;    // X
    vertices[16] = y;               // Y
}

void Tank::init()
{
    // Первый треугольник
    // 0
    vertices.push_back( -size );    // X
    vertices.push_back( -size );    // Y
    vertices.push_back( 0 );        // Z

    // 1
    vertices.push_back( size );     // X
    vertices.push_back( -size );    // Y
    vertices.push_back( 0 );        // Z

    // 2
    vertices.push_back( -size );    // X
    vertices.push_back( size );     // Y
    vertices.push_back( 0 );        // Z

    // Второй треугольник
    // 3
    vertices.push_back( -size );    // X
    vertices.push_back( size );     // Y
    vertices.push_back( 0 );        // Z

    // 4
    vertices.push_back( size );     // X
    vertices.push_back( -size );    // Y
    vertices.push_back( 0 );        // Z

    // 5
    vertices.push_back( size );     // X
    vertices.push_back( size );     // Y
    vertices.push_back( 0 );        // Z

    for ( unsigned int i = 0; i < 6; ++i ) {
        indices.push_back( i );
    }

    textures.push_back( 0 );    // S
    textures.push_back( 0 );    // T

    textures.push_back( 1 );    // S
    textures.push_back( 0 );    // T

    textures.push_back( 0 );    // S
    textures.push_back( 1 );    // T

    textures.push_back( 0 );    // S
    textures.push_back( 1 );    // T

    textures.push_back( 1 );    // S
    textures.push_back( 0 );    // T

    textures.push_back( 1 );    // S
    textures.push_back( 1 );    // T
}

void Tank::setDirection( Tank::Direction direction )
{
    m_direction = direction;
}

Tank::Direction Tank::direction() const
{
    return m_direction;
}

void Tank::setXY( int x, int y )
{
    moveTo( x, y );
}

int Tank::x() const
{
    return vertices[6];
}

int Tank::y() const
{
    return vertices[7];
}

void Tank::animate()
{
    static int i = 0;

    switch( m_direction ) {
        case Tank::Up:
            m_textureID = m_textureIdArrayToUp[i];
            break;
        case Tank::Right:
            m_textureID = m_textureIdArrayToRight[i];
            break;
        case Tank::Down:
            m_textureID = m_textureIdArrayToDown[i];
            break;
        case Tank::Left:
            m_textureID = m_textureIdArrayToLeft[i];
            break;
    }

    ++i;
    if ( i >= 4 ) {
        i = 0;
    }
}

void Tank::setHandle( int handle )
{
    m_handle = handle;
}

int Tank::handle() const
{
    return m_handle;
}

//void Tank::genFrames()
//{
//    QPixmap pixmap = QPixmap( QString( ":Textures/pla1.bmp" ) );
//    const unsigned int step = pixmap.width() / 4;
//    QPixmap copyPixmap;
//    for ( unsigned int i = 0; i < 4; ++i ) {
//        copyPixmap = pixmap.copy( i * step, 0, step, step );
//        framesToRight.push_back( copyPixmap );
//    }

//    for( unsigned int i = 0; i < framesToRight.size(); ++i ) {
//        QImage image = framesToRight[i].toImage();
//        framesToLeft.push_back( QPixmap::fromImage( image.mirrored( true, false ) ) );
//    }

//    for( unsigned int i = 0; i < framesToRight.size(); ++i ) {
//        QPixmap rotatedPixmap( framesToRight[i] );
//        QMatrix rm;
//        rm.rotate( 90 );
//        framesToDown.push_back( rotatedPixmap.transformed( rm ) );
//    }

//    for( unsigned int i = 0; i < framesToDown.size(); ++i ) {
//        QImage image = framesToDown[i].toImage();
//        framesToUp.push_back( QPixmap::fromImage( image.mirrored( false, true ) ) );
//    }
//}

void Tank::setTextureIdArrayToRight( const std::vector<unsigned int> &array )
{
    m_textureIdArrayToRight = array;
}

void Tank::setTextureIdArrayToLeft( const std::vector<unsigned int> &array )
{
    m_textureIdArrayToLeft = array;
}

void Tank::setTextureIdArrayToUp( const std::vector<unsigned int> &array )
{
    m_textureIdArrayToUp = array;
    m_textureID = m_textureIdArrayToUp[0];
}

void Tank::setTextureIdArrayToDown( const std::vector<unsigned int> &array )
{
    m_textureIdArrayToDown = array;
}
