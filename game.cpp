#include <fstream>
#include <iostream>
#include <QDebug>
#include <QPainter>
#include <ctime>

#include "game.h"


Game::Game(){
    setFixedSize(DOT_WIDTH * FIELD_WIDTH, DOT_HEIGHT * FIELD_HEIGHT);
    setWindowTitle("Pacman");
    setStyleSheet("background-color: rgb(50, 50, 70)");
    for (int i = 0; i < FIELD_HEIGHT; ++i ){
        _field [i] = new char [FIELD_WIDTH];
        for (int j = 0; j < FIELD_WIDTH; ++j ){
            _field [i][j] = '?';
        }
    }
    initGame();
}

void Game::keyPressEvent(QKeyEvent *e){
    int key = e->key();
    switch (key){
    case Qt::Key_Left:
        if (_pacman->getFutureDirection() != Unit::Directions::left)
            _pacman->setFutureDirection(Unit::Directions::left);
        qDebug() << "LEFT";
        break;
    case Qt::Key_Up:
        if (_pacman->getFutureDirection() != Unit::Directions::up)
            _pacman->setFutureDirection(Unit::Directions::up);
        qDebug() << "UP";
        break;
    case Qt::Key_Right:
        if (_pacman->getFutureDirection() != Unit::Directions::right)
            _pacman->setFutureDirection(Unit::Directions::right);
        qDebug() << "RIGHT";
        break;
    case Qt::Key_Down:
        if (_pacman->getFutureDirection() != Unit::Directions::down)
            _pacman->setFutureDirection(Unit::Directions::down);
        qDebug() << "DOWN";
        break;
    }
}

void Game::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    doDrawing();
}

void Game::timerEvent(QTimerEvent *e){
    Q_UNUSED(e);
    if (_inGame){
        move(_pacman);
        move_bot(_bot_1);
        move_bot(_bot_2);
        move_bot(_bot_3);
        move_bot(_bot_4);
        this->repaint();
    }
    else {

    }
    //isInGame();
}

void Game::isInGame(){
    if (isXYCrosses(_bot_1) || isXYCrosses(_bot_2) || isXYCrosses(_bot_3) || isXYCrosses(_bot_4))
        _inGame = false;
}

void Game::initGame(){
    _inGame = true;
    _pacman = new Unit (2, 40, 100, "pacman", Unit::Directions::right);
    _bot_1 = new Unit (1, 1360, 100, "bot_1", Unit::Directions::left);
    _bot_2 = new Unit (1, 1360, 100, "bot_2", Unit::Directions::left);
    _bot_3 = new Unit (1, 1360, 100, "bot_3", Unit::Directions::left);
    _bot_4 = new Unit (2, 1360, 100, "bot_4", Unit::Directions::left);
    _chet_time = 0;
    startTimer(25);
    srand(time(0));

    IMAGE_WALL->scaled(QSize (DOT_WIDTH, DOT_HEIGHT), Qt::KeepAspectRatio);

    std::ifstream file_map("C:\\Users\\user\\QtProjects\\Pacman\\field.txt");

    if (!file_map.is_open())
        std::cout << "File could not be open!\n";
    else{
        std::string buff;
        int i = 0;
        int g = 0;
        while (getline(file_map, buff)){
                    for (int j = 0; j < FIELD_WIDTH; ++j){
                        _field[i][j] = buff[j];
                        if (buff[j] == '-'){
                            _points_wall.resize(g + 1);
                            _points_wall[g].rx() = j * DOT_WIDTH;
                            _points_wall[g].ry() = i * DOT_HEIGHT;
                            ++g;
                        }
                    }
                    ++i;
        }
        file_map.close();
    }
}

void Game::doDrawing(){

    QPainter qp(this);

    if (_inGame){

        for (int i = 0; i < FIELD_HEIGHT; ++i){
            for (int j = 0; j < FIELD_WIDTH; ++j){
                if (_field[i][j] == '-')
                    qp.drawPixmap(j * DOT_WIDTH, i * DOT_HEIGHT, DOT_WIDTH, DOT_HEIGHT, *IMAGE_WALL);
                else if (_field[i][j] == '1'){
                    qp.setBrush(Qt::red);
                    qp.drawEllipse(j * DOT_WIDTH + DOT_WIDTH/4 + DOT_HEIGHT/8, i * DOT_HEIGHT + DOT_HEIGHT/4 + DOT_HEIGHT/8, DOT_WIDTH / 4, DOT_HEIGHT / 4);
                }
                else if (_field[i][j] == 'p');
            }
        }

        drawUnit(_pacman, _chet_time);
        drawUnit(_bot_1, _chet_time);
        drawUnit(_bot_2, _chet_time);
        drawUnit(_bot_3, _chet_time);
        drawUnit(_bot_4, _chet_time);

    }
    else{
//        gameOver();
    }
}

void Game::drawUnit(Unit *unit, int chet_time){
    QPainter qp(this);
    QString path_image_unit;
    path_image_unit = "C:\\Users\\user\\QtProjects\\Pacman\\Images\\" + unit->getName();

    if (unit->getName() == "pacman"){
        switch (unit->getDirection()){
        case Unit::Directions::left:
            path_image_unit += "_left_";
            break;
        case Unit::Directions::up:
            path_image_unit += "_up_";
            break;
        case Unit::Directions::right:
            path_image_unit += "_right_";
            break;
        case Unit::Directions::down:
            path_image_unit += "_down_";
            break;
        default:
            path_image_unit += "_sleep_";
            break;
        }

        if (chet_time % 2 == 0){
            path_image_unit += "1.png";
            _chet_time = 1;
        }
        else{
            path_image_unit += "2.png";
            _chet_time = 0;
        }
    }
    else
        path_image_unit += ".png";

    QPixmap *IMAGE_UNIT = new QPixmap(path_image_unit);
    IMAGE_UNIT->scaled(QSize (DOT_WIDTH, DOT_HEIGHT), Qt::KeepAspectRatio);
    qp.drawPixmap(unit->getX(), unit->getY(), DOT_WIDTH, DOT_HEIGHT, *IMAGE_UNIT);
}

void Game::move_bot(Unit *unit){
    if (isNewWay(unit)){
        Unit::Directions lastDir = unit->getDirection();
        do {
            int i = rand() % 4;
            //std::cout << i << std::endl;
            unit->setDirection(intToDirections(i));
        }
        while (isWall(unit->getDirection(), unit) && unit->getDirection() == lastDir);
        move(unit);
    }
    else {
        if (!isWall(unit->getDirection(), unit))
            move(unit);
        else {
            while (isWall(unit->getDirection(), unit)){
                int i = rand() % 4;
                unit->setDirection(intToDirections(i));
            }
            move(unit);
        }
    }
}

void Game::move(Unit *unit){

    bool isMoved = false;
    if (!isWall(unit->getFutureDirection(), unit) && unit->getFutureDirection() != Unit::Directions::sleep){
        switch (unit->getFutureDirection()){
        case Unit::Directions::left:{
            unit->setX(((DOT_WIDTH * FIELD_WIDTH) + (unit->getX() - unit->getSpeed())) % (DOT_WIDTH * FIELD_WIDTH));
            isMoved = true;
            unit->setDirection(unit->getFutureDirection());
            unit->setLastDirection(unit->getFutureDirection());
            std::cout << "(" << unit->getX() << ", " << unit->getY() << ")" << std::endl << std::endl;
            break;
        }
        case Unit::Directions::up:{
            unit->setY(unit->getY() - unit->getSpeed());
            isMoved = true;
            unit->setDirection(unit->getFutureDirection());
            unit->setLastDirection(unit->getFutureDirection());
            std::cout << "(" << unit->getX() << ", " << unit->getY() << ")" << std::endl;
            break;
        }
        case Unit::Directions::right: {
           unit->setX((unit->getX() + unit->getSpeed()) % (DOT_WIDTH * FIELD_WIDTH));
           isMoved = true;
           unit->setDirection(unit->getFutureDirection());
           unit->setLastDirection(unit->getFutureDirection());
           std::cout << "(" << unit->getX() << ", " << unit->getY() << ")" << std::endl << std::endl;
           break;
        }
        case Unit::Directions::down: {
            unit->setY(unit->getY() + unit->getSpeed());
            isMoved = true;
            unit->setDirection(unit->getFutureDirection());
            unit->setLastDirection(unit->getFutureDirection());
            std::cout << "(" << unit->getX() << ", " << unit->getY() << ")" << std::endl << std::endl;
            break;
        }
        default:
            break;
        }
    }

    else {
        switch (unit->getDirection()){
        case Unit::Directions::left:{
            if (!isWall(unit->getDirection(), unit)){
                unit->setX(((DOT_WIDTH * FIELD_WIDTH) + (unit->getX() - unit->getSpeed())) % (DOT_WIDTH * FIELD_WIDTH));
                isMoved = true;
                unit->setLastDirection(unit->getDirection());
            }
            else {
                unit->setDirection(Unit::Directions::sleep);
                unit->setLastDirection(Unit::Directions::sleep);
            }
            std::cout << "(" << unit->getX() << ", " << unit->getY() << ")" << std::endl << std::endl;
            break;
        }
        case Unit::Directions::up:{
            if (!isWall(unit->getDirection(), unit)){
                unit->setY(unit->getY() - unit->getSpeed());
                isMoved = true;
                unit->setLastDirection(unit->getDirection());
            }
            else {
                unit->setDirection(Unit::Directions::sleep);
                unit->setLastDirection(Unit::Directions::sleep);
            }

            std::cout << "(" << unit->getX() << ", " << unit->getY() << ")" << std::endl;
            break;
        }
        case Unit::Directions::right: {
           if (!isWall(unit->getDirection(), unit)) {
               unit->setX((unit->getX() + unit->getSpeed()) % (DOT_WIDTH * FIELD_WIDTH));
               isMoved = true;
               unit->setLastDirection(unit->getDirection());
           }
           else {
               unit->setDirection(Unit::Directions::sleep);
               unit->setLastDirection(Unit::Directions::sleep);
           }

            std::cout << "(" << unit->getX() << ", " << unit->getY() << ")" << std::endl << std::endl;
            break;
        }
        case Unit::Directions::down: {
            if (!isWall(unit->getDirection(), unit)) {
                unit->setY(unit->getY() + unit->getSpeed());
                isMoved = true;
                unit->setLastDirection(unit->getDirection());
            }
            else {
                unit->setDirection(Unit::Directions::sleep);
                unit->setLastDirection(Unit::Directions::sleep);
            }
            std::cout << "(" << unit->getX() << ", " << unit->getY() << ")" << std::endl << std::endl;
            break;
        }
        default:
            break;
        }
    }

    if (isPoint(unit) && isMoved) {
        _points += 16;
    }
}

bool Game::isXYCrosses(Unit *bot){
    int x_pacman = _pacman->getX();
    int y_pacman = _pacman->getY();
    int x_bot = bot->getX();
    int y_bot = bot->getY();
    if (y_pacman / 20 == y_bot / 20 && x_pacman / 20 == x_bot / 20)
        return true;
    return false;
}

bool Game::isWall(Unit::Directions dir, Unit *unit) {
    int x = unit->getX();
    int y = unit->getY();

    if (x % DOT_SIZE == 0 && y % DOT_SIZE == 0){
        std::cout << "x % DOT_WIDTH == 0 && y % DOT_HEIGHT == 0" << std::endl;
        switch (dir) {
        case Unit::Directions::left:
            x -= DOT_SIZE;
            //std::cout << "---left---" << std::endl;
            if (_field[y / 20][x / 20] == '-') {
                //std::cout << "[" << y / 20 << "][" << x / 20<< "] == '-'" << std::endl;
                return true;
            }
            break;
        case Unit::Directions::up:
            y -= DOT_SIZE;
            //std::cout << "---up---" << std::endl;
            if (_field[y / 20][x / 20] == '-') {
                //std::cout << "[" << y / 20 << "][" << x / 20<< "] == '-'" << std::endl;
                return true;
            }
            break;
        case Unit::Directions::right:
            x += DOT_SIZE;
            x %= (DOT_SIZE * FIELD_WIDTH);
            //std::cout << "---right---" << std::endl;
            if (_field[y / 20][x / 20] == '-') {
                //std::cout << "[" << y / 20 << "][" << x / 20<< "] == '-'" << std::endl;
                return true;
            }
            break;
        case Unit::Directions::down:
            y += DOT_SIZE;
            //std::cout << "---down---" << std::endl;
            if (_field[y / 20][x / 20] == '-') {
                //std::cout << "[" << y / 20 << "][" << x / 20<< "] == '-'" << std::endl;
                return true;
            }
            break;
        default:
            break;
        }
    }
    else {
        switch (unit->getLastDirection()) {
        case Unit::Directions::left:{
            switch (dir){
            case Unit::Directions::left:
                x += 20 - (x % 20);
                x -= DOT_SIZE;
                if (_field[y / 20][x / 20] == '-')
                  return true;
            case Unit::Directions::right:
                return false;
            default:
                return true;
            }
        break;
        }
        case Unit::Directions::up:{
            switch (dir){
            case Unit::Directions::up:
                y += 20 - (y % 20);
                y -= DOT_SIZE;
                if (_field[y / 20][x / 20] == '-')
                    return true;
            case Unit::Directions::down:
                return false;
            default:
                return true;
            }
            break;
        }
        case Unit::Directions::right:{
            switch (dir){
            case Unit::Directions::right:
                x -= x % 20;
                x += DOT_SIZE;
                if (_field[y / 20][x / 20] == '-')
                    return true;
            case Unit::Directions::left:
                return false;
            default:
                return true;
            }
            break;
        }
        case Unit::Directions::down:{
            switch (dir){
            case Unit::Directions::down:
                y -= y % 20;
                y += DOT_SIZE;
                if (_field[y / 20][x / 20] == '-')
                    return true;
            case Unit::Directions::up:
                return false;
            default:
                return true;
            }
            break;
        }
        default:
            break;
        }
    }
    return false;
}

bool Game::isPoint(Unit *unit) {
    int x = unit->getX();
    x += (DOT_WIDTH * FIELD_WIDTH);
    x %= (DOT_WIDTH * FIELD_WIDTH);
    int y = unit->getY();
    if (unit->getName() != "pacman")
        return false;
    if (_field[y / 20][x / 20] == '1') {
        _field[y / 20][x / 20] = '0';
        return true;
    }
    return false;
}

bool Game::isNewWay(Unit *unit){
    int schet = 0;
    for (int i=0; i<4; ++i)
        if (!isWall(intToDirections(i), unit))
            ++schet;
    std::cout << "---" << unit->getName().toStdString() << "   schet = " << schet << std::endl << std::endl;
    if (schet > 2)
        return true;
    return false;
}

Unit::Directions Game::intToDirections(int i){
    switch (i){
    case 0:
        return Unit::Directions::left;
        break;
    case 1:
        return Unit::Directions::up;
        break;
    case 2:
        return Unit::Directions::right;
        break;
    case 3:
        return Unit::Directions::down;
        break;
    default:
        return Unit::Directions::sleep;
        break;
    }
}
