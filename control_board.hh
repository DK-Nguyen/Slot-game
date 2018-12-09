// Slot Machine Game
// Author: Duy Khoa Nguyen
// email: nguyen29@student.tut.fi
// student number: 2725

#ifndef CONTROL_BOARD_HH
#define CONTROL_BOARD_HH

#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPointF>
#include <QGraphicsProxyWidget>
#include <iostream>
#include <QString>
#include <QColor>

const int CONTROL_BOARD_WIDTH = 600;
const int CONTROL_BOARD_HEIGHT= 150;
const std::pair<int, int> BUTTON_SIZE = std::make_pair(70,35);
const std::pair<int, int> BIG_BUTTON_SIZE = std::make_pair(100,50);
const std::pair<int, int> SPIN_BUTTON_SIZE = std::make_pair(100,100);
const QPointF INSTRUCTION_BUTTON_POS = QPointF(10,10);
const QPointF AUDIO_BUTTON_POS = QPointF(90,10);
const QPointF CREDITS_POS = QPointF(200,10);
const QPointF CREDITS_GETTER_POS = QPointF(250,10);
const QPointF BETS_POS = QPointF(350,10);
const QPointF BETS_GETTER_POS = QPointF(385,10);
const QPointF WINS_POS = QPointF(500,10);
const QPointF ONELINE_POS = QPointF(10,75);
const QPointF THREELINES_POS = QPointF(150,75);
const QPointF FIVELINES_POS = QPointF(290,75);
const QPointF SPIN_POS = QPointF(430,50);

class ControlBoard : public QGraphicsView
{
    Q_OBJECT

public:
    ControlBoard(QWidget* parent=nullptr);

    ~ControlBoard();

    int getDefaultCredits();

    int getDefaultBets();

    void clear();

signals:
    /// \brief send this signal when the spin button is clicked
    void letspin();

    /// \brief send this signal with the new value of credits to main_window
    void sendNewCredits(int newCredits);

    /// \brief send this signal with the new value of bets to main_window
    void sendNewBets(int newBets);

    /// \brief if the player has no credits left,
    /// send this signal to main_window to say that the game is over
    void gameOver();

private slots:
    /// \brief called when the spin button is clicked
    void spinButtonClicked();

    /// \brief called when the credits spin box gets new value
    void creditsChanged(int newCredits);

    /// \brief called when the bets spin box gets new value
    void betsChanged(int newBets);

    /// \brief after main_window does the calculating, it sends the signal
    /// to this slot to update the amount of credits, bet and win
    /// then this function update the information onto the UI.
    /// \param[in] credits The amount of credits after calculating
    /// \param[in] bets The amount of bets after calculating
    /// \param[in] wins The amount of wins after calculating
    void receiveCalculatingResult(int credits, int bets, int wins);

    void playAgain();

private:
    /// \brief Responsible to hold the graphic items for control board
    QGraphicsScene* scene_;

    QPushButton* audioButton_;
    QPushButton* instructionsButton_;

    QGraphicsTextItem* creditsLabel_;
    QSpinBox* creditsSpinbox_;

    QGraphicsTextItem* betsLabel_;
    QSpinBox* betsSpinbox_;

    QGraphicsTextItem* winsGraphics_;

    /// \brief The one Line, Three lines ... option buttons
    QPushButton* oneLineButton_;
    QPushButton* threeLinesButton_;
    QPushButton* fiveLinesButton_;

    /// \brief The spin button
    QPushButton* spinButton_;

    /// \brief keeps track of the value of credits of each round
    /// after the player spins
    int credits_;
    /// \brief keeps track of the value of bets of each round
    /// after the player spins
    int bets_;
    /// \brief keeps track of the value of wins of each round
    /// after the player spins
    int wins_ = 0;
};

#endif // CONTROL_BOARD_HH
