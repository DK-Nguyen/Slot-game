// Slot Machine Game
// Author: Duy Khoa Nguyen
// email: nguyen29@student.tut.fi
// student number: 272580

#include "control_board.hh"

ControlBoard::ControlBoard(QWidget* parent):QGraphicsView (parent)
{
    this->scene_ = new QGraphicsScene(this);
    scene_->setSceneRect(0, 0, CONTROL_BOARD_WIDTH, CONTROL_BOARD_HEIGHT);
    this->setScene(scene_);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    scene_->clear();
    this->setFixedSize(CONTROL_BOARD_WIDTH, CONTROL_BOARD_HEIGHT);

    // handle display of audio button
    audioButton_ = new QPushButton(tr("Audio"));
    audioButton_->setFixedSize(BUTTON_SIZE.first, BUTTON_SIZE.second);
    auto audioButtonProxy = scene_->addWidget(audioButton_);
    audioButtonProxy->setPos(AUDIO_BUTTON_POS);

    // handle display of instruction button
    instructionsButton_ = new QPushButton(tr("?"));
    instructionsButton_->setFixedSize(BUTTON_SIZE.first, BUTTON_SIZE.second);
    auto instructionButtonProxy = scene_->addWidget(instructionsButton_);
    instructionButtonProxy->setPos(INSTRUCTION_BUTTON_POS);

    // handle display of credits labels
    creditsLabel_ = new QGraphicsTextItem(tr("Credits"));
    creditsLabel_->setDefaultTextColor(QColor(255, 255, 255));
    creditsLabel_->setPos(CREDITS_POS);
    scene_->addItem(creditsLabel_);

    // handle display of the spin box to get the amount of credit
    creditsSpinbox_ = new QSpinBox();
    creditsSpinbox_->setMaximum(1000000);
    // default value
    creditsSpinbox_->setValue(100);
    credits_ = creditsSpinbox_->value();
    auto creditsGetterProxy = scene_->addWidget(creditsSpinbox_);
    creditsGetterProxy->setPos(CREDITS_GETTER_POS);

    // handle display of bets label
    betsLabel_ = new QGraphicsTextItem(tr("Bets"));
    betsLabel_->setDefaultTextColor(QColor(255, 255, 255));
    betsLabel_->setPos(BETS_POS);
    scene_->addItem(betsLabel_);

    // handle display of the spin box to get the amount of bets
    betsSpinbox_ = new QSpinBox();
    betsSpinbox_->setMaximum(credits_);
    // default value
    betsSpinbox_->setValue(5);
    bets_ = betsSpinbox_->value();
    auto betsGetterProxy = scene_->addWidget(betsSpinbox_);
    betsGetterProxy->setPos(BETS_GETTER_POS);

    // handle display of the amount of credits the player wins
    winsGraphics_ = new QGraphicsTextItem(tr("Wins: ")
                                          + QString::number(wins_));
    winsGraphics_->setDefaultTextColor(QColor(255, 255, 255));
    winsGraphics_->setPos(WINS_POS);
    winsGraphics_->adjustSize();
    scene_->addItem(winsGraphics_);

    // handle display of the "1 line" button
    oneLineButton_ = new QPushButton(tr("1 Line"));
    oneLineButton_->setFixedSize(BIG_BUTTON_SIZE.first,
                                 BIG_BUTTON_SIZE.second);
    auto oneLineProxy = scene_->addWidget(oneLineButton_);
    oneLineProxy->setPos(ONELINE_POS);

    // handle display of the "3 line" button
    threeLinesButton_ = new QPushButton(tr("3 Line"));
    threeLinesButton_->setFixedSize(BIG_BUTTON_SIZE.first,
                                    BIG_BUTTON_SIZE.second);
    auto threeLinesProxy = scene_->addWidget(threeLinesButton_);
    threeLinesProxy->setPos(THREELINES_POS);
    threeLinesButton_->setEnabled(false);

    // handle display of the "5 line" button
    fiveLinesButton_ = new QPushButton(tr("5 Line"));
    fiveLinesButton_->setFixedSize(BIG_BUTTON_SIZE.first,
                                   BIG_BUTTON_SIZE.second);
    auto fiveLinesProxy = scene_->addWidget(fiveLinesButton_);
    fiveLinesProxy->setPos(FIVELINES_POS);
    fiveLinesButton_->setEnabled(false);

    // handle display of the "spin" button
    spinButton_ = new QPushButton(tr("Spin"));
    spinButton_->setFixedSize(SPIN_BUTTON_SIZE.first,
                              SPIN_BUTTON_SIZE.second);
    auto spinProxy = scene_->addWidget(spinButton_);
    spinProxy->setPos(SPIN_POS);

    std::cerr << "done putting things on control board" << std::endl;

    // connecting the signals and slots
    connect(spinButton_, SIGNAL(clicked()),
            this, SLOT(spinButtonClicked()));

    connect(creditsSpinbox_, SIGNAL(valueChanged(int)),
            this, SLOT(creditsChanged(int)));

    connect(betsSpinbox_, SIGNAL(valueChanged(int)),
            this, SLOT(betsChanged(int)));
}

ControlBoard::~ControlBoard()
{
    return;
}

int ControlBoard::getDefaultCredits()
{
    return credits_;
}

int ControlBoard::getDefaultBets()
{
    return bets_;
}

void ControlBoard::spinButtonClicked()
{
    emit letspin();
}

void ControlBoard::creditsChanged(int newCredits)
{
    credits_ = creditsSpinbox_->value();
    newCredits = creditsSpinbox_->value();
    betsSpinbox_->setMaximum(newCredits);
    emit sendNewCredits(newCredits);
}

void ControlBoard::betsChanged(int newBets)
{
    bets_ = betsSpinbox_->value();
    newBets = betsSpinbox_->value();
    emit sendNewBets(newBets);
}

void ControlBoard::receiveCalculatingResult(int credits, int bets, int wins)
{
    credits_ = credits;
    bets_ = bets;
    wins_ = wins;

    // check if the game is over? (if the player has no credits left)
    if (credits_ <= 0) {
        this->audioButton_->setEnabled(false);
        this->instructionsButton_->setEnabled(false);
        this->spinButton_->setEnabled(false);
        this->oneLineButton_->setEnabled(false);
        this->threeLinesButton_->setEnabled(false);
        this->fiveLinesButton_->setEnabled(false);
        this->creditsSpinbox_->setEnabled(false);
        this->betsSpinbox_->setEnabled(false);
        emit gameOver();
        return;
    }

    // update the amount of wins on the screen
    winsGraphics_->setPlainText(tr("Wins: ") + QString::number(wins_));
    creditsSpinbox_->setValue(credits_);

    std::cerr << "wins: " << wins_ << std::endl;
    std::cerr << "credits: " << credits_ << std::endl;
    std::cerr << "bets: " << bets_ << std::endl;
}

void ControlBoard::playAgain()
{
    this->creditsSpinbox_->setValue(100);
    this->betsSpinbox_->setValue(5);
    this->audioButton_->setEnabled(true);
    this->instructionsButton_->setEnabled(true);
    this->spinButton_->setEnabled(true);
    this->oneLineButton_->setEnabled(true);
    this->threeLinesButton_->setEnabled(true);
    this->fiveLinesButton_->setEnabled(true);
    this->creditsSpinbox_->setEnabled(true);
    this->betsSpinbox_->setEnabled(true);
}

void ControlBoard::clear()
{
    this->scene_->clear();

}
