#include "unit.h"

Unit::Unit(int speed, int x, int y, /*std::string image_path,*/ QString name, Directions direction){
    this->_name = name;
    this->_speed = speed;
    this->_x = x;
    this->_y = y;
    this->_count_cell = 0;
    this->_direction = direction;
    if (name == "pacman")
        this->_future_direction = direction;
    else
        this->_future_direction = Directions::sleep;
    this->_last_direction = this->_direction;
}

QString Unit::getName(){
    return _name;
}

int Unit::getSpeed(){
    return _speed;
}

int Unit::getCountCell(){
    return _count_cell;
}

int Unit::getX(){
    return _x;
}

int Unit::getY(){
    return _y;
}

bool Unit::getTryDirection(){
    return _try_dir;
}

Unit::Directions Unit::getDirection(){
    return _direction;
}

Unit::Directions Unit::getFutureDirection(){
    return _future_direction;
}

Unit::Directions Unit::getLastDirection(){
    return _last_direction;
}

QPixmap Unit::getImage(){
    return *IMAGE_UNIT;
}

void Unit::setSpeed(int speed){
    _speed = speed;
}

void Unit::setCountCell(int count_cell){
    _count_cell = count_cell;
}

void Unit::setX(int x){
    _x = x;
}

void Unit::setY(int y){
    _y = y;
}

void Unit::setTryDirection(bool flag){
    _try_dir = flag;
}

void Unit::setDirection(Unit::Directions direction){
    _direction = direction;
}

void Unit::setFutureDirection(Unit::Directions future_direction){
    _future_direction = future_direction;
}

void Unit::setLastDirection(Unit::Directions last_direction){
    _last_direction = last_direction;
}

void Unit::setImage(QPixmap image){
    *IMAGE_UNIT = image;
}
