// Slot Machine Game
// Author: Duy Khoa Nguyen
// email: nguyen29@student.tut.fi
// student number: 272580

#ifndef GAME_OVER_HH
#define GAME_OVER_HH

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>

class GameOverWindow: public QDialog
{
    Q_OBJECT
public:
    GameOverWindow();

signals:
    // emit signal to play again in main_window
    void playAgain();

    // emit signal to quit in main_window
    void quit();

private slots:
    // handle play_again button
    void playAgainClicked();

    // handle quit button
    void quitClicked();

private:
    QLabel* prompt_;
    QPushButton *playAgainButton_, *quitButton_;
};

#endif // GAME_OVER_HH
