// Slot Machine Game
// Author: Duy Khoa Nguyen
// email: nguyen29@student.tut.fi
// student number: 272580

#include "game_over.hh"

GameOverWindow::GameOverWindow()
{
    std::string msg = "You do not have any credits left!";
    prompt_ = new QLabel(QString::fromUtf8(msg.c_str()));

    playAgainButton_ = new QPushButton("Play Again");

    quitButton_ = new QPushButton("Quit");
    quitButton_->setToolTip("Quit game anyway");

    QHBoxLayout *buttonLayout_ = new QHBoxLayout();
    buttonLayout_->addWidget(playAgainButton_);
    buttonLayout_->addWidget(quitButton_);

    QVBoxLayout *mainLayout_ = new QVBoxLayout();
    mainLayout_->addWidget(prompt_);
    mainLayout_->addLayout(buttonLayout_);
    setLayout(mainLayout_);

    setWindowTitle("Quit");
    setFixedHeight(sizeHint().height());

    connect(playAgainButton_, SIGNAL(clicked()), this, SLOT(playAgainClicked()));
    connect(quitButton_, SIGNAL(clicked()), this, SLOT(quitClicked()));
}

void GameOverWindow::playAgainClicked()
{
    emit playAgain();
    close();
}

void GameOverWindow::quitClicked()
{
    emit quit();
    close();
}
