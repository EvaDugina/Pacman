#ifndef UNIT_H
#define UNIT_H

#include <QPixmap>

class Unit{

public:

    enum Directions{
        left, up, right, down, sleep
    };

    Unit(int speed, int x, int y, QString name, Directions direction = Directions::right);

    QString getName();
    int getSpeed();
    int getCountCell();
    int getX();
    int getY();
    bool getTryDirection();
    Directions getDirection();
    Directions getFutureDirection();
    Directions getLastDirection();
    QPixmap getImage();

    void setSpeed(int speed);
    void setCountCell(int count_cell);
    void setX(int x);
    void setY(int y);
    void setTryDirection(bool flag);
    void setDirection(Directions direction);
    void setFutureDirection(Directions future_direction);
    void setLastDirection(Directions last_direction);
    void setImage(QPixmap image);

private:

    Directions _direction, _future_direction, _last_direction;
    QString _name;
    int _speed;
    int _count_cell;
    bool _try_dir = false;
    int _x, _y;
    QPixmap *IMAGE_UNIT = new QPixmap("C:\\Users\\user\\QtProjects\\Pacman\\Images\\pacman.png");

};

#endif // UNIT_H
