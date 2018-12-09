/*
#####################################################################
# TIE-02207 Programming 2: Basics, F2018                            #
# Project4: Slots                                                   #
# Program description: Implements a virtual slots game.             #
# File: reel.cpp                                                    #
# Description: Draws random fruits for labels to display.           #
# Notes: * This is an exercise program.                             #
#        * Students need to modify this file.                       #
#####################################################################
*/

#include "reel.hh"

#include <iostream>

// a reel is a QGraphicScene
Reel::Reel(std::vector<QLabel*> labels,
           QPushButton* lock_button,
           const Fruits* fruits,
           std::shared_ptr<std::default_random_engine> rng) {
    this->rng_ = rng;
    this->labels_ = labels;
    this->fruits_ = fruits;
    this->lock_button_ = lock_button;
    this->scene_ = new QGraphicsScene(this);
    scene_->setSceneRect(0, 0, REEL_WIDTH, REEL_HEIGHT);
    this->setScene(scene_);
    scene_->clear();
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setFixedSize(REEL_WIDTH, REEL_HEIGHT);


    // push the labels with fruit pixmaps into the labels_ vector;
    for (auto fruit : *fruits_) {
        QLabel* fruitLabel = new QLabel();
        fruitLabel->setPixmap(fruit.second.first);
        labels_.push_back(fruitLabel);
    }

    // lock button
    lock_button_->setFixedSize(LOCK_BUTTON_SIZE.first, LOCK_BUTTON_SIZE.second);
    lock_button_->setText("LOCK");
//    std::cerr << "reel constructor isLocked: " << isLocked_ << std::endl;
    lockButtonProxy_ = scene_->addWidget(lock_button_);
    lockButtonProxy_->setPos(LOCK_BUTTON_POS);


    spinning();

    // Connect all signals & slots
    connect(lock_button_, SIGNAL(clicked()), this, SLOT(lockButtonClicked()));

}

void Reel::clear()
{
    scene_->clear();
}

void Reel::removeLockButton()
{
    scene_->removeItem(lockButtonProxy_);
}

void Reel::spinning()
{
    if (isLocked_) {
//        std::cerr << "I am locked" << std:: endl;
        return;
    }

//    std::cerr << "I am spinning" << std:: endl;

    std::set<std::pair<double, std::string>> fruitWithProb;

    // The middle fruit (the one with biggest prob) will be the last one
    // of the fruitWithProb set because it is ordered ascendingly based on
    // the first element in the pair.
    for (auto fruit : *fruits_) {
        auto zeroToOne = double(rng_->operator()()) / double(rng_->max());
        auto fruitName = fruit.first;
        auto frequency = fruit.second.second;
        auto prob = double(zeroToOne * frequency);
        fruitWithProb.insert(std::make_pair(prob, fruitName));
    }

    // print out the set
//    for (auto setItem : fruitWithProb) {
//        std::cerr << setItem.first << ": " << setItem.second << std::endl;
//    }

    auto last_it = fruitWithProb.rbegin();
    // middle fruit lies at the end of the set
    auto middle_fruit = last_it->second;
//    std::cerr << "middle fruit: " << middle_fruit << std::endl;

    auto next_to_last_it = ++last_it;
    auto first_fruit = next_to_last_it->second;
//    std::cerr << "first fruit: " << first_fruit << std::endl;

    auto two_to_last_it = ++next_to_last_it;
    auto third_fruit = two_to_last_it->second;
//    std::cerr << "third fruit: " << third_fruit << std::endl;

    // put the fruit labels onto the scene (3 fruits for now, can be more)
    auto firstFruitIt = fruits_->find(first_fruit);
    QPixmap firstFruitPixmap = firstFruitIt->second.first;
    QLabel* firstFruitLabel = new QLabel();
    firstFruitLabel->setPixmap(firstFruitPixmap);
    QGraphicsProxyWidget* firstFruitProxy = scene_->addWidget(firstFruitLabel);
    firstFruitProxy->setPos(FIRST_FRUIT_POS);

    auto middleFruitIt = fruits_->find(middle_fruit);
    QPixmap middleFruitPixmap = middleFruitIt->second.first;
    QLabel* middleFruitLabel = new QLabel();
    middleFruitLabel->setPixmap(middleFruitPixmap);
    QGraphicsProxyWidget* middleFruitProxy = scene_->addWidget(middleFruitLabel);
    middleFruitProxy->setPos(SECOND_FRUIT_POS);

    auto thirdFruitIt = fruits_->find(third_fruit);
    QPixmap thirdFruitPixmap = thirdFruitIt->second.first;
    QLabel* thirdFruitLabel = new QLabel();
    thirdFruitLabel->setPixmap(thirdFruitPixmap);
    QGraphicsProxyWidget* thirdFruitProxy = scene_->addWidget(thirdFruitLabel);
    thirdFruitProxy->setPos(THIRD_FRUIT_POS);

    // Now after finishing picking the middle fruit, emit the signal that contains
    // the middle fruit to mainwindow
    emit stopped(middle_fruit);
}

void Reel::lockButtonClicked()
{
    if (!isLocked_) {
        isLocked_ = true;
        lock_button_->setText("UNLOCK");
    } else {
        isLocked_ = false;
        lock_button_->setText("LOCK");
    }

    emit lockSignal(isLocked_);
}

void Reel::gameIsOver()
{
    this->lock_button_->setEnabled(false);
}

void Reel::playAgain()
{
    this->lock_button_->setEnabled(true);
}













