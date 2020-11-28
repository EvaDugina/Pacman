#ifndef GAME_H
#define GAME_H

#include "unit.h"

#include <QWidget>
#include <QLabel>
#include <QSize>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QTimer>
#include <QTime>

class Game : public QWidget{
public:

    enum Directions{
        left, up, right, down, sleep
    };

    Game();

protected:
    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:

    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    static const int DOT_SIZE = 20;
    static const int FIELD_WIDTH = 70;
    static const int FIELD_HEIGHT = 11;
    static const int FAST_SPEED_BOT  = 4;

    QPixmap *IMAGE_WALL = new QPixmap("C:\\Users\\user\\QtProjects\\Pacman\\Images\\wall.png");


    bool _inGame;
    int _chet_time;
    Unit *_pacman;
    Unit *_bot_1, *_bot_2, *_bot_3, *_bot_4;
    int _points;
    char** _field = new char *[FIELD_HEIGHT]; // "" - пустая клетка, "-" - стена, "1" - клетка с монетой, "2" - клетка с супер монетой
    QVector <QPoint> _points_wall;

    void initGame();
    void isInGame();
    void doDrawing();
    void drawUnit(Unit *unit, int _chet_time);
    void move_bot(Unit *unit);
    void move(Unit *unit);
    bool isXYCrosses(Unit *bot);
    bool isWall(Unit::Directions dir, Unit *unit);
    bool isPoint(Unit *unit);
    bool isNewWay(Unit *unit);
    Unit::Directions intToDirections(int i);
};

#endif // GAME_H
