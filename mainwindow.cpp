#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);
    setWindowTitle("Pacman");
    setStyleSheet("background-color: rgb(50, 50, 70)");
    setFixedSize(WIDTH, HEIGHT);

    button_begin = new QPushButton("&НАЧАТЬ ИГРУ", this);
    button_begin->setObjectName("button_start_game");
    button_begin->setGeometry(QRect(QPoint(WIDTH/2 - 200/2, HEIGHT/2 - 50/2), QSize(200, 50)));
    button_begin->setStyleSheet("background-color: white;");
    connect(button_begin, SIGNAL(clicked()), this, SLOT(on_button_start_game_clicked()));

}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_button_start_game_clicked(){

    // Инициализируем второе окно
    sWindow = new Game();
    // подключаем к слоту запуска главного окна по кнопке во втором окне
    connect(sWindow, &Game::firstWindow, this, &MainWindow::show);
    this->hide();    // Закрываем основное окно
    //MainWindow::close();
    sWindow->show();  // Показываем второе окно
}
