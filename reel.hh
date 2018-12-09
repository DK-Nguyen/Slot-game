/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: reel.h                                                      #
# Description: Draws random fruits for labels to display.           #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/

#ifndef SLOTS_REEL_H
#define SLOTS_REEL_H

#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <map>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>
#include <iostream>
#include <set>
#include <QGraphicsProxyWidget>

// Fruits is an alias for a map which contains a fruit's name and
// corresponding image and likelyhood of appearance.
using Fruits = std::map<std::string, std::pair<QPixmap, int>>;

const int FRUIT_WIDTH = 100;
const int FRUIT_HEIGHT = 100;
const std::pair<int, int> LOCK_BUTTON_SIZE = std::make_pair(80,50);
const int REEL_WIDTH = 120;
const int REEL_HEIGHT = FRUIT_HEIGHT*3 + LOCK_BUTTON_SIZE.second;
const QPointF FIRST_FRUIT_POS = QPointF(0,0);
const QPointF SECOND_FRUIT_POS = FIRST_FRUIT_POS + QPointF(0,FRUIT_HEIGHT);
const QPointF THIRD_FRUIT_POS = SECOND_FRUIT_POS + QPointF(0,FRUIT_HEIGHT);
const QPointF LOCK_BUTTON_POS = THIRD_FRUIT_POS + QPointF(10,100);
/// \class Reel
/// \brief Implements reels which are used to determine if the player has won.
///
/// Reel spins for a random duration and draws random fruits to display on its
/// labels. Reel signals MainWindow with the name of the middlemost fruit, but
/// an ordered (top-to-bottom) list of all fruits can also be queried.
///
class Reel: public QGraphicsView {
    Q_OBJECT

public:

    /// \brief Construct a new Reel.
    ///
    /// \param[in] labels Labels which should be used to display the symbols.
    /// \param[in] lock_button Button whose state determines if Reel is locked.
    /// \param[in] fruits Images and frequencies of each fruit.
    /// \param[in] rng Random number generator used to determine the symbols.
    ///
    Reel(std::vector<QLabel*> labels,
         QPushButton* lock_button,
         const Fruits* fruits,
         std::shared_ptr<std::default_random_engine> rng);

    void clear();

    void removeLockButton();

signals:

    /// \brief Is emitted when the Reel stops.
    ///
    /// \param[in] Name of the middle symbol on the Reel.
    ///
    void stopped(const std::string& middle_sym);

    void lockSignal(bool isLocked);

public slots:
    void spinning();

    void lockButtonClicked();

    void gameIsOver();

    void playAgain();

private:
    const Fruits* fruits_;
    std::vector<QLabel*> labels_;
    QPushButton* lock_button_;
    QGraphicsProxyWidget* lockButtonProxy_;
    std::shared_ptr<std::default_random_engine> rng_;
    QGraphicsScene* scene_;
    bool isLocked_ = false;

};  // class Reel


#endif  // SLOTS_REEL_H
