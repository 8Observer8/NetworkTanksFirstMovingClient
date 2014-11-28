#ifndef TANK_H
#define TANK_H

#include <vector>
#include <GL/gl.h>
//#include <QPixmap>

class Tank
{
public:
    enum Direction { Left, Down, Right, Up };
    Tank( int handle = -1, int x = 0, int y = 0 );
    void draw() const;
    void move( int step );
    void moveTo( int x, int y );
    void init();
    void setDirection( Direction direction );
    Direction direction() const;
    void setXY( int x, int y );
    int x() const;
    int y() const;
//    void genFrames();
    void animate();
    void setHandle( int handle );
    int handle() const;

    void setTextureIdArrayToRight( const std::vector<unsigned int> &array );
    void setTextureIdArrayToLeft( const std::vector<unsigned int> &array );
    void setTextureIdArrayToUp( const std::vector<unsigned int> &array );
    void setTextureIdArrayToDown( const std::vector<unsigned int> &array );

//    std::vector<QPixmap> framesToRight;
//    std::vector<QPixmap> framesToLeft;
//    std::vector<QPixmap> framesToDown;
//    std::vector<QPixmap> framesToUp;

    std::vector<int> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> textures;
    const size_t size;

private:
    Direction m_direction;
    unsigned int m_textureID;
    std::vector<unsigned int> m_textureIdArrayToRight;
    std::vector<unsigned int> m_textureIdArrayToLeft;
    std::vector<unsigned int> m_textureIdArrayToDown;
    std::vector<unsigned int> m_textureIdArrayToUp;
    int m_handle;
    int m_x;
    int m_y;
};

#endif // TANK_H
