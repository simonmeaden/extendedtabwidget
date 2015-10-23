#include "mainwindow.h"
#include "../extendedtabwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {

    tabs = new ExtendedTabWidget(this);

    // set simplelogin password.
    tabs->setPassword("password");

    // set standard login passwords.
    tabs->addPassword("op", "operator");
    tabs->addPassword("eng", "engineer");
    tabs->addPassword("man", "manager");

    connect(tabs, SIGNAL(loggedIn()), this, SLOT(setLoginText()));
    connect(tabs, SIGNAL(loggedOut()), this, SLOT(setLogoutText()));
    connect(tabs, SIGNAL(passwordBad()), this, SLOT(setPasswordBadText()));
    connect(tabs, SIGNAL(usernameBad()), this, SLOT(setUsernameBadText()));

    QFrame *f1 = new QFrame(this);
    QGridLayout *l = new QGridLayout;
    f1->setLayout(l);

    int row = 0;

    tabs->addTab(f1, "Test");

    label = new QLabel(tr("Not logged in"), this);
    l->addWidget(label, row++, 0);

    pLogin = new QCheckBox(tr("Enable Login"), this);
    connect(pLogin, SIGNAL(clicked(bool)), this, SLOT(enableLogin(bool)));
    l->addWidget(pLogin, row, 0);

    pLoginType = new QCheckBox(tr("Check for Simple Login, not for Standard Login"), this);
    connect(pLoginType, SIGNAL(clicked(bool)), this, SLOT(setLoginType(bool)));
    l->addWidget(pLoginType, row++, 1, 1, 3);

    pFrame = new QCheckBox(tr("Hide Frame"), this);
    connect(pFrame, SIGNAL(clicked(bool)), this, SLOT(showFrame(bool)));
    l->addWidget(pFrame, row++, 0);

    pClock = new QCheckBox(tr("Show Clock"), this);
    connect(pClock, SIGNAL(clicked(bool)), this, SLOT(showClock(bool)));
    l->addWidget(pClock, row, 0);

    pSeconds = new QCheckBox(tr("Show Seconds"), this);
    connect(pSeconds, SIGNAL(clicked(bool)), this, SLOT(setSeconds(bool)));
    l->addWidget(pSeconds, row++, 1, 1, 2);

    pMessage = new QCheckBox(tr("Show Messages"), this);
    connect(pMessage, SIGNAL(clicked(bool)), this, SLOT(showMessages(bool)));
    l->addWidget(pMessage, row++, 0);

    pMarquee = new QCheckBox(tr("Show Marquee"), this);
    connect(pMarquee, SIGNAL(clicked(bool)), this, SLOT(setMarquee(bool)));
    l->addWidget(pMarquee, row, 0);

    l->addWidget(new QLabel(tr("Marquee Speed : ")), row, 1);

    pMarqueeSpeed = new QSpinBox(this);
    pMarqueeSpeed->setMinimum(1);
    pMarqueeSpeed->setMaximum(100);
    pMarqueeSpeed->setValue(10);
    connect(pMarqueeSpeed, SIGNAL(valueChanged(int)), this, SLOT(setMarqueeSpeed(int)));
    l->addWidget(pMarqueeSpeed, row++, 2);

    pMessageEdit = new QLineEdit(this);
    l->addWidget(pMessageEdit, row, 0, 1, 2);

    pSendMsgBtn = new QPushButton(tr("Send Message"), this);
    connect(pSendMsgBtn, SIGNAL(clicked()), this, SLOT(sendMessage()));
    l->addWidget(pSendMsgBtn, row++, 2);

    QFrame *f2 = new QFrame(this);
    QGridLayout *fL = new QGridLayout;
    f2->setLayout(fL);
    l->addWidget(f2, row++, 0, 1, 3);

    fL->addWidget(new QLabel(tr("Temporary Color :")), 0, 0);

    QStringList list;
    list << tr("No Colour") << tr("Red") << tr("Green") << tr("Blue") << tr("Orange") << tr("Pink");
    pTempColorBox = new QComboBox(this);
    pTempColorBox->addItems(list);
    connect(pTempColorBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setTemporaryColor(int)));
    fL->addWidget(pTempColorBox, 0, 1);

    pChoosePermColorBtn = new QPushButton(tr("Choose Permanent Colour"), this);
    connect(pChoosePermColorBtn, SIGNAL(clicked()), this, SLOT(setPermanentColor()));
    fL->addWidget(pChoosePermColorBtn, 0, 2);

    fL->addWidget(new QLabel(tr("Temporary Background :")), 1, 0);

    pTempBackBox = new QComboBox(this);
    pTempBackBox->addItems(list);
    connect(pTempBackBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setTemporaryBack(int)));
    fL->addWidget(pTempBackBox, 1, 1);

    pChoosePermBackBtn = new QPushButton(tr("Choose Permanent Background"), this);
    connect(pChoosePermBackBtn, SIGNAL(clicked()), this, SLOT(setPermanentBack()));
    fL->addWidget(pChoosePermBackBtn, 1, 2);


    setCentralWidget(tabs);
}

MainWindow::~MainWindow() {

}

// General controls ======================================================

void MainWindow::showFrame(bool checked) {
    if (checked) {
        tabs->showFrame(false);
        pFrame->setText("Show Frame");
    } else {
        tabs->showFrame(true);
        pFrame->setText("Hide Frame");
    }
}

// Login controls ======================================================

void MainWindow::enableLogin(bool checked) {
    if (checked) {
        tabs->showLogin(true);
        pLogin->setText("Disable Login");
    } else {
        tabs->showLogin(false);
        pLogin->setText("Enable Login");
    }
}

void MainWindow::setLoginType(bool type) {
    if (type)
        tabs->setSimpleLogin(true);
    else
        tabs->setLogin(true);
}

// Message controls ======================================================

void MainWindow::showMessages(bool checked) {
    if (checked)
        tabs->showMessages(true);
    else
        tabs->showMessages(false);
}

void MainWindow::setMarquee(bool checked) {
    if (checked)
        tabs->setMarquee(true);
    else
        tabs->setMarquee(false);
}

void MainWindow::setMarqueeSpeed(int value) {
    tabs->setMarqueeSpeed(value);
}

void MainWindow::sendMessage() {
    if (mTempColor.isValid() && mTempBack.isValid())
        tabs->setMessage(mTempColor, mTempBack, pMessageEdit->text());
    else if (mTempColor.isValid() && !mTempBack.isValid())
        tabs->setMessage(mTempColor, pMessageEdit->text());
    else
        tabs->setMessage(pMessageEdit->text());
}

void MainWindow::setTemporaryColor(int index) {
    switch (index) {
    case 0: // no colour
        mTempColor = QColor();
        break;
    case 1:
        mTempColor = QColor("red");
        break;
    case 2:
        mTempColor = QColor("green");
        break;
    case 3:
        mTempColor = QColor("blue");
        break;
    case 4:
        mTempColor = QColor("orange");
        break;
    case 5:
        mTempColor = QColor("pink");
        break;
    default: // no colour
        mTempColor = QColor();
        break;
    }
}

void MainWindow::setPermanentColor() {
    QColor color = QColorDialog::getColor(
                       QColor("black"),
                       this,
                       tr("Choose Permanent Color")
                   );
    tabs->setMessageColor(color);
}

void MainWindow::setTemporaryBack(int index) {
    switch (index) {
    case 0: // no colour
        mTempBack = QColor();
        break;
    case 1:
        mTempBack = QColor("red");
        break;
    case 2:
        mTempBack = QColor("green");
        break;
    case 3:
        mTempBack = QColor("blue");
        break;
    case 4:
        mTempBack = QColor("orange");
        break;
    case 5:
        mTempBack = QColor("pink");
        break;
    default: // no colour
        mTempBack = QColor();
        break;
    }
}

void MainWindow::setPermanentBack() {
    QColor color = QColorDialog::getColor(
                       QColor("black"),
                       this,
                       tr("Choose Permanent Background")
                   );
    tabs->setMessageBackground(color);
}

// Clock controls ======================================================

void MainWindow::showClock(bool checked) {
    if (checked) {
        tabs->showClock(true);
        pClock->setText("Hide Clock");
    } else {
        tabs->showClock(false);
        pClock->setText("Show Clock");
    }
}

void MainWindow::setSeconds(bool checked) {
    if (checked) {
        tabs->showSeconds(true);
        pSeconds->setText("Hide Seconds");
    } else {
        tabs->showSeconds(false);
        pSeconds->setText("Show Seconds");
    }
}

// Signal results ======================================================

void MainWindow::setLoginText() {
    if (!pLoginType->isChecked())
        label->setText(tr("Standard Login Success"));
    else
        label->setText(tr("Simple Login Success"));

}

void MainWindow::setLogoutText() {
    if (!pLoginType->isChecked())
        label->setText(tr("Standard Logout Success"));
    else
        label->setText(tr("Simple Logout Success"));
}

void MainWindow::setUsernameBadText() {
    label->setText(tr("The supplied username was not available"));
}

void MainWindow::setPasswordBadText() {
    label->setText(tr("The supplied password was not correct"));
}
// Signal results ======================================================
