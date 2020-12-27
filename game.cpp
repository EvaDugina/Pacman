#include <iostream>
#include <QDebug>
#include <QPainter>
#include <QCoreApplication>
#include <ctime>
#include <QMessageBox>
#include <QFile>

#include "game.h"
#include "ui_gamewindow.h"


Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game) {
    ui->setupUi(this);
    setFixedSize(DOT_WIDTH * FIELD_WIDTH, DOT_HEIGHT * FIELD_HEIGHT + 30);
    setWindowTitle("Pacman");
    setStyleSheet("background-color: rgb(50, 50, 70)");
    for (int i = 0; i < FIELD_HEIGHT; ++i ){
        _field [i] = new char [FIELD_WIDTH];
        for (int j = 0; j < FIELD_WIDTH; ++j ){
            _field [i][j] = '?';
        }
    }
    //_sound->setMedia(QUrl("qrc:/sounds/resources\\music/big_goblin_death.wav"));
    initGame();
}

Game::~Game(){
    delete ui;
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
    _time += TIMER_EVENT;
    if (_inGame){
        move(_pacman);
        move_bot(_bot_1);
        move_bot(_bot_2);
        move_bot(_bot_3);
        move_bot(_bot_4);
        this->repaint();
        isInGame();
        checkReabilitationBots();
    }
    else {
        _lives -= 1;
        if (_lives <= 0 && !_isGameOver){
            gameOver(false);
        }
        else if(!_isGameOver){
            QTime dieTime= QTime::currentTime().addSecs(1);
                while (QTime::currentTime() < dieTime)
                    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
            _inGame = true;
            redraw(Unit::Directions::left);
        }
    }

    if (_chet_time % 2 == 0)
        _chet_time = 1;
    else
        _chet_time = 0;
}



void Game::initGame(){
    _inGame = true;
    _lives = 3;
    _isGameOver = false;
    IMAGE_WALL->scaled(QSize (DOT_WIDTH, DOT_HEIGHT), Qt::KeepAspectRatio);
    IMAGE_PACMAN->scaled(QSize (DOT_WIDTH, DOT_HEIGHT), Qt::KeepAspectRatio);
    IMAGE_PACMAN_POWER->scaled(QSize (DOT_WIDTH, DOT_HEIGHT), Qt::KeepAspectRatio);
    _chet_time = 0;
    _time = 0;
    _points = 0;
    _pacman_power_beans = false;
    startTimer(TIMER_EVENT);
    srand(time(0));
    redraw(Unit::Directions::left);

    reloadField();
}

void Game::gameOver(bool flag){
    /*this->close();      // Закрываем окно
    //qDebug() << "Dead";
    emit firstWindow();*/
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Victory!", "Would you like to win again?",
                                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No){
            _isGameOver = true;
            _inGame = false;
            this->close();
        }
        else {
            _isGameOver = true;
            _inGame = false;
            this->close();
            emit firstWindow();
            //qDebug() << "Dead";
        }
}

void Game::isInGame(){
    if ( ( (isXYCrosses(_bot_1) && _bot_1->getAlive())
           || (isXYCrosses(_bot_2) && _bot_2->getAlive())
           || (isXYCrosses(_bot_3) && _bot_3->getAlive())
           || (isXYCrosses(_bot_4) && _bot_4->getAlive()))
         && !_pacman_power_beans)
        _inGame = false;
    else if (isXYCrosses(_bot_1) && _bot_1->getAlive()) {
        _points += KILL_PRICE * _cf_new_kill;
        _bot_1->setAlive(false);
        _bot_1->setReabilitationTime(_time + TIME_BOT_REABILITATION);
        _cf_new_kill *= CF_FOR_KILL;
    }
    else if (isXYCrosses(_bot_2) && _bot_2->getAlive()) {
        _points += KILL_PRICE * _cf_new_kill;
        _bot_2->setAlive(false);
        _bot_2->setReabilitationTime(_time + TIME_BOT_REABILITATION);
        _cf_new_kill *= CF_FOR_KILL;
    }
    else if (isXYCrosses(_bot_3) && _bot_3->getAlive()) {
        _points += KILL_PRICE * _cf_new_kill;
        _bot_3->setAlive(false);
        _bot_3->setReabilitationTime(_time + TIME_BOT_REABILITATION);
        _cf_new_kill *= CF_FOR_KILL;
    }
    else if (isXYCrosses(_bot_4) && _bot_4->getAlive()) {
        _points += KILL_PRICE * _cf_new_kill;
        _bot_4->setAlive(false);
        _bot_4->setReabilitationTime(_time + TIME_BOT_REABILITATION);
        _cf_new_kill *= CF_FOR_KILL;
    }
}

void Game::redraw(Unit::Directions dir){
    _pacman = new Unit (2, 40, 100, "pacman", Unit::Directions::sleep);
    _bot_1 = new Unit (2, 1280, 100, "bot_1", dir);
    _bot_2 = new Unit (2, 1280, 100, "bot_2", dir);
    _bot_3 = new Unit (2, 1280, 100, "bot_3", dir);
    _bot_4 = new Unit (5, 1280, 100, "bot_4", dir);
}

void Game::doDrawing(){
    QPainter qp(this);
    if (_inGame){

        QFont font;
        font.setPixelSize(15);
        font.setBold(1);

        qp.save();
        qp.setPen(Qt::white);
        qp.setFont(font);
        qp.drawText(20, DOT_HEIGHT * FIELD_HEIGHT + 15, "POINTS: " + QString::number(_points));
        qp.restore();

        for (int i = 0; i < _lives; ++i)
            qp.drawPixmap(170 + i * DOT_WIDTH + 10, DOT_HEIGHT * FIELD_HEIGHT + 5, DOT_WIDTH, DOT_HEIGHT, *IMAGE_PACMAN);

        int count_points = 0;

        for (int i = 0; i < FIELD_HEIGHT; ++i){
            for (int j = 0; j < FIELD_WIDTH; ++j){
                if (_field[i][j] == '-')
                    qp.drawPixmap(j * DOT_WIDTH, i * DOT_HEIGHT, DOT_WIDTH, DOT_HEIGHT, *IMAGE_WALL);
                else if (_field[i][j] == '1'){
                    qp.setBrush(Qt::red);
                    qp.drawEllipse(j * DOT_WIDTH + DOT_WIDTH/4 + DOT_HEIGHT/8, i * DOT_HEIGHT + DOT_HEIGHT/4 + DOT_HEIGHT/8, DOT_WIDTH / 4, DOT_HEIGHT / 4);
                    ++count_points;
                }
                else if (_field[i][j] == 'p'){
                    qp.drawPixmap(j * DOT_WIDTH + DOT_WIDTH/4, i * DOT_HEIGHT + DOT_HEIGHT/4, DOT_WIDTH / 2, DOT_HEIGHT / 2, *IMAGE_PACMAN_POWER);
                    ++count_points;
                }
            }
        }

        if (count_points == 0){
            gameOver(true);
        }

        drawUnit(_pacman);
        drawUnit(_bot_1);
        drawUnit(_bot_2);
        drawUnit(_bot_3);
        drawUnit(_bot_4);

    }
    else{
        //gameOver();
    }
}

void Game::drawUnit(Unit *unit){
    QPainter qp(this);
    QString path_image_unit;
    path_image_unit = ":/images/" + unit->getName();

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

        if (_chet_time % 2 == 0)
            path_image_unit += "1";

        else
            path_image_unit += "2";
    }
    else if(!unit->getAlive())
        path_image_unit = ":/images/bot_dead";
    else if (_pacman_power_beans){
        path_image_unit = ":/images/bot_fucked_";
        if (_chet_time % 2 == 0)
            path_image_unit += "1";
        else
            path_image_unit += "2";

    }
    path_image_unit += ".png";

    QPixmap *IMAGE_UNIT = new QPixmap(path_image_unit);
    IMAGE_UNIT->scaled(QSize (DOT_WIDTH, DOT_HEIGHT), Qt::KeepAspectRatio);
    qp.drawPixmap(unit->getX(), unit->getY(), DOT_WIDTH, DOT_HEIGHT, *IMAGE_UNIT);
}

void Game::move_bot(Unit *unit){
    if (isNewWay(unit)){
        do {
            int i = rand() % 4;
            unit->setDirection(intToDirections(i));
        }
        while (isWall(unit->getDirection(), unit));
        move(unit);
    }
    else {
        if (!isWall(unit->getDirection(), unit))
            move(unit);
        else {
            int j = -1;
            do {
                ++j;
                int i = rand() % 4;
                unit->setDirection(intToDirections(i));
            }
            while (isWall(unit->getDirection(), unit)
                   || (unit->getDirection() == oppositDirection(unit->getLastDirection()) && j < 5));
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

    if (isPoint(unit) && isMoved){
        if (_pacman_power_beans)
            _points += 2 * POINTS_PRICE;
        else
            _points += POINTS_PRICE;
    }
    isPointPacmanPower(unit);
}

void Game::reloadField(){
    QFile file_map(":/field.txt");
    if (!file_map.open(QIODevice::ReadOnly | QIODevice::Text))
        std::cout << "File could not be open!\n";
    else{
        QTextStream in(&file_map);
        std::string buff;
        int i = 0;
        int g = 0;
        while (!in.atEnd()){
            QString line = in.readLine();
            for (int j = 0; j < FIELD_WIDTH; ++j){
                _field[i][j] = line.toStdString().c_str()[j];
                if (_field[i][j] == '-'){
                    _points_wall.resize(g + 1);
                    _points_wall[g].rx() = j * DOT_WIDTH;
                    _points_wall[g].ry() = i * DOT_HEIGHT;
                    ++g;
                }
            }
            ++i;
        }
    }
    file_map.close();
}

void Game::checkReabilitationBots(){
    if (_bot_1->getReabilitationTime() == _time)
        _bot_1->setAlive(true);

    if (_bot_2->getReabilitationTime() == _time)
        _bot_2->setAlive(true);

    if (_bot_3->getReabilitationTime() == _time)
        _bot_3->setAlive(true);

    if (_bot_4->getReabilitationTime() == _time)
        _bot_4->setAlive(true);


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

bool Game::isPointPacmanPower(Unit *unit) {
    if (_time == _end_pacman_power){
        _pacman_power_beans = false;
        _cf_new_kill = 1;
    }

    if (_pacman->getSpeed() != 2 && _pacman->getX() % DOT_WIDTH == 0 && _pacman->getY() % DOT_HEIGHT == 0 && !_pacman_power_beans)
        _pacman->setSpeed(2);

    if (_pacman->getSpeed() != 5 && _pacman->getX() % DOT_WIDTH == 0 && _pacman->getY() % DOT_HEIGHT == 0 && _pacman_power_beans)
        _pacman->setSpeed(5);

    int x = unit->getX();
    x += (DOT_WIDTH * FIELD_WIDTH);
    x %= (DOT_WIDTH * FIELD_WIDTH);
    int y = unit->getY();
    if (unit->getName() != "pacman")
        return false;
    if (_field[y / 20][x / 20] == 'p') {
        _field[y / 20][x / 20] = '0';
        _end_pacman_power = _time + TIME_PACMAN_POWER;
        _pacman_power_beans = true;
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
        return Unit::Directions::right;
        break;
    case 2:
        return Unit::Directions::up;
        break;
    case 3:
        return Unit::Directions::down;
        break;
    default:
        return Unit::Directions::sleep;
        break;
    }
}

int Game::directionToInt(Unit::Directions dir){
    switch (dir){
    case Unit::Directions::left:
        return 0;
        break;
    case Unit::Directions::right:
        return 1;
        break;
    case Unit::Directions::up:
        return 2;
        break;
    case Unit::Directions::down:
        return 3;
        break;
    default:
        return 4;
        break;
    }
}

Unit::Directions Game::oppositDirection(Unit::Directions dir){
    switch (dir){
    case Unit::Directions::left:
        return Unit::Directions::right;
    case Unit::Directions::right:
        return Unit::Directions::left;
    case Unit::Directions::up:
        return Unit::Directions::down;
    case Unit::Directions::down:
        return Unit::Directions::up;
    default:
        return Unit::Directions::sleep;
    }
}
