#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "game.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // Слоты от кнопок главного окна
    void on_button_start_game_clicked();

private:
    static const int WIDTH = 600;
    static const int HEIGHT = 600;

    Ui::MainWindow *ui;
    Game *sWindow;

    QPushButton *button_begin;
};

#endif // MAINWINDOW_H
