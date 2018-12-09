/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: main_window.h                                               #
# Description: Implements a UI for the game to be playable.         #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/

#ifndef SLOTS_MAINWINDOW_H
#define SLOTS_MAINWINDOW_H

#include "reel.hh"
#include "control_board.hh"
#include "game_over.hh"
#include <QMainWindow>
#include <string>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QPointF>
#include <iostream>

const int PAYTABLE_WIDTH = 600;
const int PAYTABLE_HEIGHT = 200;

const double SCALE = 1;

/// \class MainWindow
/// \brief Implements the main window which is used to interact with the game.
///
/// MainWindow owns all the widgets used to display the game to the player
/// and to play the game. MainWindow contains the whole game, no other windows
/// are used.
///
class MainWindow: public QWidget {
    Q_OBJECT

public:

    /// \brief Construct a new MainWindow.
    ///
    /// \param[in] parent Points to this MainWindow's parent widget.
    ///
    explicit MainWindow(QWidget* parent = nullptr);

signals:
    /// \brief This signal is sent to control board to display the information
    /// onto the UI
    void calculatingDone(int newCurrents, int newBets, int newWins);

    /// \brief This signal is sent to Reels and Control board to activate
    /// the game to play again
    void letsPlayAgain();

private slots:

    /// \brief After the 1st reel stops, this slot is called to update the
    /// information needed for calculating the amount of wins and credits left
    /// \param[in] middle_sym Name of the fruit symbol on the middle lane.
    ///
    void firstReelStopped(const std::string& middle_sym);

    /// \brief After the 2nd reel stops, this slot is called to update the
    /// information needed for calculating the amount of wins and credits left
    /// \param[in] middle_sym Name of the fruit symbol on the middle lane.
    ///
    void secondReelStopped(const std::string& middle_sym);

    /// \brief After the 3rd reel stops, this slot is called to update the
    /// information needed for calculating the amount of wins and credits left
    /// \param[in] middle_sym Name of the fruit symbol on the middle lane.
    ///
    void thirdReelStopped(const std::string& middle_sym);

    /// \brief When the player changes the amount of credits in the spin box,
    /// this slot receives a signal from control board and updates the
    /// currentCredits_ variable.
    /// \param[in] newCredits The value of updated credits
    ///
    void getCredits(int newCredits);

    /// \brief When the player changes the amount of bets in the spin box,
    /// this slot receives a signal from control board and updates the
    /// currentBets_ variable.
    /// \param[in] newBets The value of updated bets
    ///
    void getBets(int newBets);

    void firstReelLocked(bool isLocked);

    void secondReelLocked(bool isLocked);

    void thirdReelLocked(bool isLocked);

    /// \brief Based on the information received from the reels and controlboard,
    /// this function calculates the amount of winning money and update
    /// currentCredits_ currentBets_ and currentWins_.
    /// Then it emit a signal containing theses information to controlboard.
    void calculator();

    /// \brief Use to set the boolean parameters to check the states of the reels.
    void reelSpinning();

    /// \brief This slot is called when the game is over
    /// (player has no credits left)
    void gameIsOver();

    /// \brief this slot is called when the player click the quit button
    /// on the game over window
    void quitGame();

    /// \brief this slot is called when the player click the play again button
    void playAgain();

private:
    /// \brief Variables
    ///< The game symbols and their rarities.
    Fruits fruits_;


    /// \brief Initialize the UI
    ///
    void initUi();

    /// \brief Initialize a reel
    ///
    Reel* makeReel();

    QLabel* payTableImage_;
    ControlBoard* controlBoard_;
    QGraphicsView* margin_;
    Reel* firstReel_;
    Reel* secondReel_;
    Reel* thirdReel_;
    Reel* bonusReel_;

    /// \brief Initialize the margin that contains "1 line",
    /// "3 line"... texts on the left of the reels
    QGraphicsView* makeReelMargin();

    int currentCredits_;
    int currentBets_;
    int currentWins_ = 0;

    std::string firstMiddleFruit_;
    std::string secondMiddleFruit_;
    std::string thirdMiddleFruit_;

    bool firstReelLocked_ = false;
    bool secondReelLocked_ = false;
    bool thirdReelLocked_ = false;

    bool firstReelSpinning_;
    bool secondReelSpinning_;
    bool thirdReelSpinning_;

    bool firstReelStopped_;
    bool secondReelStopped_;
    bool thirdReelStopped_;

};  // class MainWindow


#endif  // SLOTS_MAINWINDOW_H
