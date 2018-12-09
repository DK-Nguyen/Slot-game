/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: main_window.cpp                                             #
# Description: Implements a UI for the game to be playable.         #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/

#include "main_window.hh"
#include <QPixmap>
#include <QString>
#include <QLabel>
#include <chrono>
#include <map>
#include <memory>
#include <random>
#include <utility>
#include <iostream>


// To avoid polluting the global namespace with all kinds of
// names, free functions and global variables / constants are
// usually placed inside a namespace. As we don't have a named
// namespace here, we'll use the anonymous namespace instead.
namespace {

/// \brief Get the fruit symbols and their frequencies.
///
/// \return A map of image names and corresponding images and frequencies.
///
Fruits loadFruits() {
    // Set frequencies for each symbol.
    // * You can freely play around with the values below.
    // * Values are relative to each other, higher == more common.
    // * Names of these fruits (and berries too) have to actually match
    //   images found on the disk. The images must be in the PNG format.
    const std::map<std::string, int> frequencies{
        {"cherries", 56},
        {"strawberry", 50},
        {"orange", 46},
        {"pear", 39},
        {"apple", 35},
        {"bananas", 30},
        {"tomato", 24},
        {"grapes", 15},
        {"eggplant", 5}
    };

    // Define where the images can be found and what kind of images they are.
    // * Here we assume that the build directory is next to Slots directory.
    const std::string directory(":/MainWindow/Resources/");
    const std::string extension(".png");

    // Load the images into QPixmaps and bundle them with their frequencies.
    Fruits symbols;
    for (const std::pair<std::string, int>& frequency : frequencies) {
        const std::string filename = directory + frequency.first + extension;
        const QPixmap image(QString::fromStdString(filename));
        QPixmap rescaled = image.scaled(int(FRUIT_WIDTH*SCALE),
                                        int(FRUIT_HEIGHT*SCALE));
        symbols[frequency.first] = std::make_pair(rescaled, frequency.second);
    }

    return symbols;
}

}  // namespace


MainWindow::MainWindow(QWidget* parent):
    QWidget(parent),
    fruits_(loadFruits())
{
    initUi();
//    std::cerr << "constructor done" << std::endl;
}

void MainWindow::initUi() {
    // Create one random number generator for all Reels to use.
    // * The seed value is obtained via the chrono library and
    //   represents the number of seconds since 1.1.1970.
    // * If you need to debug some pesky error, you can temporarily
    //   replace the seed with some literal. See Minesweeper or
    //   Waterdropgame versions 1 and 2.
    const auto time = std::chrono::high_resolution_clock::now();
    const auto seed = time.time_since_epoch().count();
    auto rng = std::make_shared<std::default_random_engine>(seed);

    // My Code
    // set background for the game
    this->setStyleSheet("background-image:url(:/MainWindow/Resources/bg2.png)");

    QGridLayout* layout = new QGridLayout();
    layout->setSpacing(0);

    payTableImage_ = new QLabel();
    QPixmap* payTablePixmap = new QPixmap(":/MainWindow/Resources/payTable2.png");
    QPixmap rescaled = payTablePixmap->scaled(PAYTABLE_WIDTH, PAYTABLE_HEIGHT);
    payTableImage_->setPixmap(rescaled);
    layout->addWidget(payTableImage_,0,0);

    // Reel layout
    QHBoxLayout* reelLayout = new QHBoxLayout();

    margin_ = makeReelMargin();
    // First Reel
    firstReel_ = makeReel();
    // Second Reel
    secondReel_ = makeReel();
    // Third Reel
    thirdReel_ = makeReel();
    // Bonus Reel
    bonusReel_ = makeReel();
    bonusReel_->removeLockButton();

    // Adding the reels onto the layout
    reelLayout->addWidget(margin_);
    reelLayout->addWidget(firstReel_);
    reelLayout->addWidget(secondReel_);
    reelLayout->addWidget(thirdReel_);
    reelLayout->addWidget(bonusReel_);
    reelLayout->setMargin(0);
    reelLayout->setSpacing(0);
    layout->addLayout(reelLayout, 1, 0);

    // Control Board
    controlBoard_ = new ControlBoard();
    layout->addWidget(controlBoard_,2,0);
    currentCredits_ = controlBoard_->getDefaultCredits();
    currentBets_ = controlBoard_->getDefaultBets();

    setLayout(layout);
    this->show();

    // connect the signals
    connect(firstReel_, SIGNAL(lockSignal(bool)), this, SLOT(firstReelLocked(bool)));
    connect(secondReel_, SIGNAL(lockSignal(bool)), this, SLOT(secondReelLocked(bool)));
    connect(thirdReel_, SIGNAL(lockSignal(bool)), this, SLOT(thirdReelLocked(bool)));

    connect(controlBoard_, SIGNAL(letspin()), firstReel_, SLOT(spinning()));
    connect(controlBoard_, SIGNAL(letspin()), secondReel_, SLOT(spinning()));
    connect(controlBoard_, SIGNAL(letspin()), thirdReel_, SLOT(spinning()));
    connect(controlBoard_, SIGNAL(letspin()), bonusReel_, SLOT(spinning()));
    connect(controlBoard_, SIGNAL(letspin()), this, SLOT(reelSpinning()));

    // when the player changes the number of credits or bets, we need to update
    connect(controlBoard_, SIGNAL(sendNewCredits(int)),
            this, SLOT(getCredits(int)));
    connect(controlBoard_, SIGNAL(sendNewBets(int)), this, SLOT(getBets(int)));

    connect(firstReel_,SIGNAL(stopped(const std::string&)),
            this, SLOT(firstReelStopped(const std::string&)));
    connect(secondReel_,SIGNAL(stopped(const std::string&)),
            this, SLOT(secondReelStopped(const std::string&)));
    connect(thirdReel_,SIGNAL(stopped(const std::string&)),
            this, SLOT(thirdReelStopped(const std::string&)));
//    connect(bonusReel,SIGNAL(stopped(const std::string&)), this, SLOT(reelStopped(const std::string&)));

    connect(firstReel_, SIGNAL(stopped(const std::string&)),
            this, SLOT(calculator()));
    connect(secondReel_, SIGNAL(stopped(const std::string&)),
            this, SLOT(calculator()));
    connect(thirdReel_, SIGNAL(stopped(const std::string&)),
            this, SLOT(calculator()));

    connect(this, SIGNAL(calculatingDone(int,int,int)),
            controlBoard_, SLOT(receiveCalculatingResult(int,int,int)));

    connect(controlBoard_, SIGNAL(gameOver()), this, SLOT(gameIsOver()));
    connect(controlBoard_, SIGNAL(gameOver()), firstReel_, SLOT(gameIsOver()));
    connect(controlBoard_, SIGNAL(gameOver()), secondReel_, SLOT(gameIsOver()));
    connect(controlBoard_, SIGNAL(gameOver()), thirdReel_, SLOT(gameIsOver()));

    connect(this, SIGNAL(letsPlayAgain()), controlBoard_, SLOT(playAgain()));
    connect(this, SIGNAL(letsPlayAgain()), firstReel_, SLOT(playAgain()));
    connect(this, SIGNAL(letsPlayAgain()), secondReel_, SLOT(playAgain()));
    connect(this, SIGNAL(letsPlayAgain()), thirdReel_, SLOT(playAgain()));
}

Reel *MainWindow::makeReel()
{
    std::vector<QLabel*> reelLabels;
    QPushButton* reelLock = new QPushButton();
    // making the random engine
    const auto time = std::chrono::high_resolution_clock::now();
    const auto seed = time.time_since_epoch().count();
    auto rng = std::make_shared<std::default_random_engine>(seed);
    // now we have enough information for making the reel
    Reel* reel = new Reel(reelLabels, reelLock, &fruits_, rng);
    return reel;
}

QGraphicsView *MainWindow::makeReelMargin()
{
    QGraphicsView* margin_view = new QGraphicsView();
    QGraphicsScene* margin_scene = new QGraphicsScene(margin_view);
    margin_scene->setSceneRect(0, 0, REEL_WIDTH, REEL_HEIGHT);
    margin_view->setFixedSize(REEL_WIDTH, REEL_HEIGHT);
    margin_view->setScene(margin_scene);
    margin_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    margin_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QFont serifFont("Times", 20, QFont::Bold);
    QGraphicsTextItem* oneLine = new QGraphicsTextItem(tr("1 Line"));
    oneLine->setPos(30, 150);
    oneLine->setFont(serifFont);
    QGraphicsTextItem* threeLines = new QGraphicsTextItem(tr("3 Lines"));
    threeLines->setPos(30, 270);
    threeLines->setFont(serifFont);
    QGraphicsTextItem* fiveLines = new QGraphicsTextItem(tr("5 Lines"));
    fiveLines->setPos(30, 30);
    fiveLines->setFont(serifFont);

    margin_scene->addItem(oneLine);
    margin_scene->addItem(threeLines);
    margin_scene->addItem(fiveLines);

    return margin_view;
}

void MainWindow::reelSpinning()
{
    firstReelSpinning_ = true;
    secondReelSpinning_ = true;
    thirdReelSpinning_ = true;
    firstReelStopped_ = false;
    secondReelStopped_ = false;
    thirdReelStopped_ = false;
}

void MainWindow::firstReelStopped(const std::string& middle_sym) {
    firstMiddleFruit_ = middle_sym;
    firstReelStopped_ = true;
    firstReelSpinning_ = false;
    std::cerr << "1st reel middle fruit: " << firstMiddleFruit_ << std::endl;
}

void MainWindow::secondReelStopped(const std::string &middle_sym)
{
    secondMiddleFruit_ = middle_sym;
    secondReelStopped_ = true;
    secondReelSpinning_ = false;
    std::cerr << "2nd reel middle fruit: " << secondMiddleFruit_ << std::endl;
}

void MainWindow::thirdReelStopped(const std::string &middle_sym)
{
    thirdMiddleFruit_ = middle_sym;
    thirdReelStopped_ = true;
    thirdReelSpinning_ = false;
    std::cerr << "3rd reel middle fruit: " << thirdMiddleFruit_ << std::endl;
}

void MainWindow::getCredits(int newCredits)
{
    currentCredits_ = newCredits;
    std::cerr << "updated credits: " << currentCredits_ << std::endl;
}

void MainWindow::getBets(int newBets)
{
    currentBets_ = newBets;
    std::cerr << "updated bets: " << currentBets_ << std::endl;
}

void MainWindow::firstReelLocked(bool isLocked)
{
    firstReelLocked_ = isLocked;
    if (firstReelLocked_) {
        std::cerr << "1st reel is locked." << std::endl;
    } else {
        std::cerr << "1st reel is unlocked." << std::endl;
    }

}

void MainWindow::secondReelLocked(bool isLocked)
{
    secondReelLocked_ = isLocked;
    if (secondReelLocked_) {
        std::cerr << "2nd reel is locked." << std::endl;
    } else {
        std::cerr << "2nd reel is unlocked." << std::endl;
    }
}

void MainWindow::thirdReelLocked(bool isLocked)
{
    thirdReelLocked_ = isLocked;
    if (thirdReelLocked_) {
        std::cerr << "3rd reel is locked." << std::endl;
    } else {
        std::cerr << "3rd reel is unlocked." << std::endl;
    }
}


void MainWindow::calculator()
{
    // first check the states of all the reels
    // if all of them have stopped, we check the middle fruits and
    // make the calculations.
    if (!firstReelSpinning_ && !secondReelSpinning_ && !thirdReelSpinning_
            && firstReelStopped_ && secondReelStopped_ && thirdReelStopped_) {

        // Win cases
        if (firstMiddleFruit_ == "cherries" &&
                firstMiddleFruit_ == secondMiddleFruit_ &&
                secondMiddleFruit_ == thirdMiddleFruit_)
        {
            std::cerr << "3 cherries, X3" << std::endl;
            currentWins_ = currentBets_ * 3;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "strawberry"  &&
                   firstMiddleFruit_ == secondMiddleFruit_  &&
                   secondMiddleFruit_ == thirdMiddleFruit_)
        {
            std::cerr << "3 straberries, X3" << std::endl;
            currentWins_ = currentBets_ * 3;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "orange"  &&
                   firstMiddleFruit_ == secondMiddleFruit_  &&
                   secondMiddleFruit_ == thirdMiddleFruit_)
        {
            std::cerr << "3 oranges, X3" << std::endl;
            currentWins_ = currentBets_ * 3;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "pear"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 secondMiddleFruit_ == thirdMiddleFruit_)
        {
            std::cerr << "3 pears, X4" << std::endl;
            currentWins_ = currentBets_ * 4;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "apple"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 secondMiddleFruit_ == thirdMiddleFruit_)
        {
            std::cerr << "3 apples, X4" << std::endl;
            currentWins_ = currentBets_ * 4;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "bananas"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 secondMiddleFruit_ == thirdMiddleFruit_)
        {
            std::cerr << "3 bananas, X4" << std::endl;
            currentWins_ = currentBets_ * 4;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "tomato"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 secondMiddleFruit_ == thirdMiddleFruit_)
        {
            std::cerr << "3 tomatoes, X5" << std::endl;
            currentWins_ = currentBets_ * 5;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "grapes"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 secondMiddleFruit_ == thirdMiddleFruit_)
        {
            std::cerr << "3 grapes, X5" << std::endl;
            currentWins_ = currentBets_ * 5;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "eggplant"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 secondMiddleFruit_ == thirdMiddleFruit_)
        {
            std::cerr << "3 eggplants, X6" << std::endl;
            currentWins_ = currentBets_ * 6;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "cherries"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 thirdMiddleFruit_ == "strawberry") {
            std::cerr << "2 cherries + 1 strawberry, X2" << std::endl;
            currentWins_ = currentBets_ * 2;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "orange"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 thirdMiddleFruit_ == "strawberry") {
            std::cerr << "2 orange + 1 strawberry, X2" << std::endl;
            currentWins_ = currentBets_ * 2;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "orange"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 thirdMiddleFruit_ == "pear") {
            std::cerr << "2 orange + 1 pear, X2" << std::endl;
            currentWins_ = currentBets_ * 2;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "pear"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 thirdMiddleFruit_ == "apple") {
            std::cerr << "2 pear + 1 apple, X2.5" << std::endl;
            currentWins_ = int(currentBets_ * 2.5);
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "apple"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 thirdMiddleFruit_ == "bananas") {
            std::cerr << "2 apple + 1 bananas, X2.5" << std::endl;
            currentWins_ = int(currentBets_ * 2.5);
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "tomato"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 thirdMiddleFruit_ == "grapes") {
            std::cerr << "2 tomato + 1 grapes, X2.5" << std::endl;
            currentWins_ = int(currentBets_ * 2.5);
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "grapes"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 thirdMiddleFruit_ == "cherries") {
            std::cerr << "2 grapes + 1 cherries, X3" << std::endl;
            currentWins_ = currentBets_ * 3;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "grapes"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 thirdMiddleFruit_ == "eggplant") {
            std::cerr << "2 grapes + 1 eggplant, X5" << std::endl;
            currentWins_ = currentBets_ * 5;
            currentCredits_ += currentWins_;
        }
        else if (firstMiddleFruit_ == "eggplant"  &&
                 firstMiddleFruit_ == secondMiddleFruit_  &&
                 thirdMiddleFruit_ == "bananas") {
            std::cerr << "2 eggplants + 1 bananas, X5" << std::endl;
            currentWins_ = currentBets_ * 5;
            currentCredits_ += currentWins_;
        }
        else {
            std::cerr << "you lose this turn!" << std::endl;
            currentCredits_ -= currentBets_;
            currentWins_ = 0;
        }

        emit calculatingDone(currentCredits_, currentBets_, currentWins_);
    }
    // if one of the reels is still spinning, return
    else {
        return;
    }

}

void MainWindow::gameIsOver()
{
    auto gameOverWindow = new GameOverWindow();
    connect(gameOverWindow, SIGNAL(quit()), this, SLOT(quitGame()));
    connect(gameOverWindow, SIGNAL(playAgain()), this, SLOT(playAgain()));
    gameOverWindow->show();
}

void MainWindow::quitGame()
{
    payTableImage_->clear();
    payTableImage_->close();
    margin_->close();
    firstReel_->clear();
    firstReel_->close();
    secondReel_->clear();
    secondReel_->close();
    thirdReel_->clear();
    thirdReel_->close();
    bonusReel_->clear();
    bonusReel_->close();
    controlBoard_->clear();
    controlBoard_->close();
    this->close();
}

void MainWindow::playAgain()
{
    emit letsPlayAgain();
}
