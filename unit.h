#ifndef UNIT_H
#define UNIT_H

#include <QPixmap>

class Unit{

public:

    enum Directions {
        left, right, up, down, sleep
    };

    Unit(int speed, int x, int y, QString name, Directions direction = Directions::right);

    QString getName();
    int getSpeed();
    int getX();
    int getY();
    int getReabilitationTime();
    bool getAlive();
    Directions getDirection();
    Directions getFutureDirection();
    Directions getLastDirection();

    void setSpeed(int speed);
    void setX(int x);
    void setY(int y);
    void setReabilitationTime(int time);
    void setAlive(bool flag);
    void setDirection(Directions direction);
    void setFutureDirection(Directions future_direction);
    void setLastDirection(Directions last_direction);

private:

    Directions _direction, _future_direction, _last_direction;
    QString _name;
    int _speed;
    int _x, _y;
    bool _alive;
    int _reabilitation_time;

};

#endif // UNIT_H
