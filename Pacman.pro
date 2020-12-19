QT += widgets \
      multimedia

SOURCES += \
        game.cpp \
        main.cpp \
        mainwindow.cpp \
        unit.cpp

HEADERS += \
    game.h \
    mainwindow.h \
    unit.h

DISTFILES += \
    .gitignore \
    field.txt \
    resources/images/bot_1.png \
    resources/images/bot_2.png \
    resources/images/bot_3.png \
    resources/images/bot_4.png \
    resources/images/bot_dead.png \
    resources/images/bot_fucked_1.png \
    resources/images/bot_fucked_2.png \
    resources/images/bot_pre_normal_1.png \
    resources/images/bot_pre_normal_2.png \
    resources/images/floor.png \
    resources/images/pacman.psd \
    resources/images/pacman_down_1.png \
    resources/images/pacman_down_2.png \
    resources/images/pacman_left_1.png \
    resources/images/pacman_left_2.png \
    resources/images/pacman_power.png \
    resources/images/pacman_right_1.png \
    resources/images/pacman_right_2.png \
    resources/images/pacman_sleep_1.png \
    resources/images/pacman_sleep_2.png \
    resources/images/pacman_up_1.png \
    resources/images/pacman_up_2.png \
    resources/images/wall.png \
    resources/music/big_goblin_death.wav

FORMS += \
    gamewindow.ui \
    mainwindow.ui

RESOURCES += \
    res.qrc
