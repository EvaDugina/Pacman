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
#include <QMediaPlayer>

namespace Ui {
    class Game;
}

class Game : public QWidget{
    Q_OBJECT

public:
    enum Directions{
        left, right, up, down, sleep
    };

    explicit Game(QWidget *parent = 0);
    ~Game();

signals:
    void firstWindow();  // Сигнал для первого окна на открытие

protected:
    void timerEvent(QTimerEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:

    Ui::Game *ui;

    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    static const int DOT_SIZE = 20;
    static const int FIELD_WIDTH = 70;
    static const int FIELD_HEIGHT = 11;
    static const int CF_FOR_KILL = 2;
    static const int POINTS_PRICE = 15;
    static const int KILL_PRICE = 200;
    static const int FAST_SPEED_BOT = 4;
    static const int TIMER_EVENT = 50;
    static const int TIME_PACMAN_POWER = TIMER_EVENT * 40 * 10;
    static const int TIME_BOT_REABILITATION = TIMER_EVENT * 40 * 5;
    static const int TIME_BOT_PRE_REABILITATION = TIMER_EVENT * 40 * 2;

    QPixmap *IMAGE_WALL = new QPixmap(":/images/wall.png");
    QPixmap *IMAGE_PACMAN = new QPixmap(":/images/pacman_right_2.png");
    QPixmap *IMAGE_PACMAN_POWER = new QPixmap(":/images/pacman_power.png");

    QMediaPlayer *_sound = new QMediaPlayer();

    bool _isGameOver;
    bool _inGame;
    int _chet_time;
    bool _pacman_power_beans;
    int _end_pacman_power;
    int _time;
    int _lives;
    int _cf_new_kill = 1;
    Unit *_pacman;
    Unit *_bot_1, *_bot_2, *_bot_3, *_bot_4;
    int _points;
    char** _field = new char *[FIELD_HEIGHT]; // "" - пустая клетка, "-" - стена, "1" - клетка с монетой, "2" - клетка с супер монетой
    QVector <QPoint> _points_wall;

    void initGame();
    void gameOver(bool flag);
    void isInGame();
    void redraw(Unit::Directions dir);
    void doDrawing();
    void drawUnit(Unit *unit);
    void move_bot(Unit *unit);
    void move(Unit *unit);
    void reloadField();
    void checkReabilitationBots();
    bool isXYCrosses(Unit *bot);
    bool isWall(Unit::Directions dir, Unit *unit);
    bool isPoint(Unit *unit);
    bool isPointPacmanPower(Unit *unit);
    bool isNewWay(Unit *unit);
    Unit::Directions intToDirections(int i);
    int directionToInt(Unit::Directions dir);
    Unit::Directions oppositDirection(Unit::Directions dir);
};

#endif // GAME_H
