#include "extendedtabwidget_p.h"
#include "extendedtabwidget.h"
#include "osdab/base64.h"

LoginDialogPrivate::LoginDialogPrivate(LoginDialog *parent) :
    q_ptr(parent) {

    initBuild(false);
}

void LoginDialogPrivate::initBuild(bool largeText) {
    Q_Q(LoginDialog);

    QFont f = q_ptr->font();
    if (largeText) {
        f.setPointSize(20);
        f.setWeight(QFont::Bold);
        q_ptr->setFont(f);
    }

    QLabel *lbl1 = new QLabel("User name :", q_ptr);
    lbl1->setFont(f);
    q->userEdit = new QLineEdit(q_ptr);
    QLabel *lbl2 = new QLabel("Password :", q_ptr);
    lbl2->setFont(f);
    q->passEdit = new QLineEdit(q_ptr);
    q->passEdit->setEchoMode(QLineEdit::Password);

    QGridLayout *layout = new QGridLayout(q_ptr);
    q->setLayout(layout);

    layout->addWidget(lbl1, 0, 0);
    layout->addWidget(q->userEdit, 0, 1);
    layout->addWidget(lbl2, 1, 0);
    layout->addWidget(q->passEdit, 1, 1);

    QDialogButtonBox* buttons = new QDialogButtonBox( q_ptr );
    buttons->addButton( QDialogButtonBox::Ok );
    buttons->addButton( QDialogButtonBox::Cancel );
    buttons->button(QDialogButtonBox::Ok)->setText( q->tr("Login") );
    buttons->button(QDialogButtonBox::Ok)->setFont(f);
    buttons->button(QDialogButtonBox::Cancel)->setText( q->tr("Abort") );
    buttons->button(QDialogButtonBox::Cancel)->setFont(f);
    layout->addWidget(buttons, 2, 0, 1, 2);

    // close on abort
    q->connect(buttons->button(QDialogButtonBox::Cancel),
               SIGNAL(clicked()),
               q_ptr,
               SLOT(close())
              );

    // acceptLogin and abort on accept.
    // should acceptLogin first.
    q->connect(buttons->button(QDialogButtonBox::Ok),
               SIGNAL(clicked()),
               q_ptr,
               SLOT(acceptLogin())
              );
    q->connect(buttons->button(QDialogButtonBox::Ok),
               SIGNAL(clicked()),
               q_ptr,
               SLOT(close())
              );

}

void LoginDialogPrivate::acceptLogin() {
    Q_Q(LoginDialog);
    emit q->loginData(q->userEdit->text(), q->passEdit->text());
}

SimpleLoginDialogPrivate::SimpleLoginDialogPrivate(SimpleLoginDialog *parent) :
    q_ptr(parent) {

    initBuild(false);
}

void SimpleLoginDialogPrivate::initBuild(bool largeText) {
    Q_Q(SimpleLoginDialog);

    QFont f = q_ptr->font();
    if (largeText) {
        f.setPointSize(20);
        f.setWeight(QFont::Bold);
        q_ptr->setFont(f);
    }

    QLabel *lbl1 = new QLabel("Password :", q_ptr);
    lbl1->setFont(f);

    q->passEdit = new QLineEdit(q_ptr);
    q->passEdit->setFont(f);
    q->passEdit->setEchoMode(QLineEdit::Password);

    QGridLayout *layout = new QGridLayout(q_ptr);
    q->setLayout(layout);

    layout->addWidget(lbl1, 0, 0);
    layout->addWidget(q->passEdit, 0, 1);

    QDialogButtonBox* buttons = new QDialogButtonBox( q_ptr );
    buttons->addButton( QDialogButtonBox::Ok );
    buttons->addButton( QDialogButtonBox::Cancel );
    buttons->button(QDialogButtonBox::Ok)->setText( q->tr("Login") );
    buttons->button(QDialogButtonBox::Ok)->setFont(f);
    buttons->button(QDialogButtonBox::Cancel)->setText( q->tr("Abort") );
    buttons->button(QDialogButtonBox::Cancel)->setFont(f);
    layout->addWidget(buttons, 1, 0, 1, 2);

    // close on abort
    q->connect(buttons->button(QDialogButtonBox::Cancel),
               SIGNAL(clicked()),
               q_ptr,
               SLOT(close())
              );

    // acceptLogin and abort on accept.
    // should acceptLogin first.
    q->connect(buttons->button(QDialogButtonBox::Ok),
               SIGNAL(clicked()),
               q_ptr,
               SLOT(acceptLogin())
              );
    q->connect(buttons->button(QDialogButtonBox::Ok),
               SIGNAL(clicked()),
               q_ptr,
               SLOT(close())
              );

}

void SimpleLoginDialogPrivate::acceptLogin() {
    Q_Q(SimpleLoginDialog);
    emit q->simpleLoginData(q->passEdit->text());
}

const QString ExtendedTabWidgetPrivate::LOGIN = "Log In";
const QString ExtendedTabWidgetPrivate::LOGOUT = "Log Out";
const QString ExtendedTabWidgetPrivate::WITHSECONDS = "00:00:00";
const QString ExtendedTabWidgetPrivate::NOSECONDS = "00:00";

ExtendedTabWidgetPrivate::ExtendedTabWidgetPrivate(ExtendedTabWidget *parent) :
    q_ptr(parent),
    bLoggedIn(false), // control flags for extensions
    bShowLogin(false),
    bLogin(true),
    bSimplelogin(false),
    bShowClock(false),
    bShowMessages(false),
    pLoginRect(NULL),
    pClockRect(NULL),
    pMessageRect(NULL),
    mNow(0), // Clock time
    bShowSeconds(false), // show seconds in clock
    bShowFrame(true), // show frames around extensions
    bIsInframe(false), // mouse test flags for login
    bButtonPressed(false),
    bIgnoreCase(false), // case in login names and passwords.
    pClockTimer(NULL),
    pMarqueeTimer(NULL),
    pTimeoutTimer(NULL),
    mTextColor(QColor("black")),
    bMarquee(false),
    mMarqueeSpeed(MARQUEE_TIME) {

    mPalette =  QGuiApplication::palette();
    mBackground = mPalette.window();
    sLoginText = LOGIN;
    mTimeout = 0;
    mMarqueePos = 0;

}

ExtendedTabWidgetPrivate::~ExtendedTabWidgetPrivate() {
    clearFrames();
}

void ExtendedTabWidgetPrivate::clearFrames() {
    if (pLoginRect) {
        delete pLoginRect;
        pLoginRect = NULL;
    }
    if (pClockRect) {
        delete pClockRect;
        pClockRect = NULL;
    }
    if (pMessageRect) {
        delete pMessageRect;
        pMessageRect = NULL;
        delete pMessageClip;
        pMessageClip = NULL;
    }
   q_ptr->update();
}

void ExtendedTabWidgetPrivate::showFrame(bool frame) {
    bShowFrame = frame;
    q_ptr->update();
}


void ExtendedTabWidgetPrivate::showClock(bool clock) {
    bShowClock = clock;
    if (bShowClock) {
        if (pClockTimer) {
            if (pClockTimer->isActive()) {
                return;
            } else {
                pClockTimer->start(CLOCK_TIME);
            }
        } else {
            pClockTimer = new QTimer(q_ptr);
            q_ptr->connect(pClockTimer, SIGNAL(timeout()), q_ptr, SLOT(nextSecond()), Qt::UniqueConnection);
            pClockTimer->start(CLOCK_TIME);
        }
    } else {
        if (pClockTimer && pClockTimer->isActive()) {
            pClockTimer->stop();
            pClockTimer->deleteLater();
            pClockTimer = NULL;
        }
    }

    clearFrames();

    q_ptr->update();
}

void ExtendedTabWidgetPrivate::showMessages(bool messages) {
    bShowMessages = messages;
    if (bShowMessages) {
        if (bMarquee) {
            if (pMarqueeTimer)
                startMarqueeTimerIfRequired();
            else
                stopMarqueeTimer();
        }
    } else {
        stopMarqueeTimer();
    }

    clearFrames();

    q_ptr->update();
}

void ExtendedTabWidgetPrivate::setMarquee(bool marquee) {
    bMarquee = marquee;
    if (bShowMessages) {
        if (bMarquee)
            startMarqueeTimerIfRequired();
        else
            stopMarqueeTimer();
    }
}

void ExtendedTabWidgetPrivate::setMarqueeSpeed(int speed) {
    mMarqueeSpeed = 1000 / speed;
    pMarqueeTimer->setInterval(mMarqueeSpeed);
    q_ptr->update();
}

void ExtendedTabWidgetPrivate::stopMarqueeTimer() {
    if (pMarqueeTimer) {
        pMarqueeTimer->stop();
        pMarqueeTimer->deleteLater();
        pMarqueeTimer = NULL;
    }
}

void ExtendedTabWidgetPrivate::startMarqueeTimerIfRequired() {
    if (pMarqueeTimer) {
        if (pMarqueeTimer->isActive()) {
            return;
        } else {
            pMarqueeTimer->start(MARQUEE_TIME);
        }
    } else {
        pMarqueeTimer = new QTimer(q_ptr);
        q_ptr->connect(pMarqueeTimer, SIGNAL(timeout()), q_ptr, SLOT(updateMarquee()), Qt::UniqueConnection);
        pMarqueeTimer->start(MARQUEE_TIME);
    }
}

QBrush ExtendedTabWidgetPrivate::messageBackgroundBrush() {
    return mBackground;
}

QColor ExtendedTabWidgetPrivate::messageTextColor() {
    return mTextColor;
}

void ExtendedTabWidgetPrivate::updateMarquee() {
    mMarqueePos++;
    q_ptr->update();
}

void ExtendedTabWidgetPrivate::showLogin(bool login) {
    bShowLogin = login;

    clearFrames();

    q_ptr->update();
}

void ExtendedTabWidgetPrivate::setLogin(bool login) {
    bLogin = login;
    bSimplelogin = !login;
}

void ExtendedTabWidgetPrivate::setSimpleLogin(bool login) {
    bLogin = !login;
    bSimplelogin = login;
}

bool ExtendedTabWidgetPrivate::mousePressEvent(QMouseEvent *event) {
//    Q_Q(ExtendedTabWidget);
    int x = event->x();
    int y = event->y();

    if (isInLoginFrame(x, y)) {
        bIsInframe = true;
        bButtonPressed = true;
    } else {
        bIsInframe = false;
        bButtonPressed = false;
    }

    return bIsInframe;
}

bool ExtendedTabWidgetPrivate::mouseReleaseEvent(QMouseEvent */*event*/) {
    Q_Q(ExtendedTabWidget);
    if (bLogin || bSimplelogin) {
        if (bIsInframe) {
            bButtonPressed = false;
            if (isLoggedIn()) { // if it's logged in already then log out.
                bLoggedIn = false;
                displayLogin();
                emit q->loggedOut();
            } else {
                if (bSimplelogin) {
                    SimpleLoginDialog *pDlg = new SimpleLoginDialog(q_ptr);
                    pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
                    q->connect(pDlg,
                               SIGNAL(simpleLoginData(QString)),
                               q_ptr,
                               SLOT(checkPassword(QString)));
                    pDlg->exec();
                    if (isLoggedIn()) {
                        displayLogout();
                    }
                } else {
                    LoginDialog *pDlg = new LoginDialog(q_ptr);
                    pDlg->setAttribute(Qt::WA_DeleteOnClose, true);
                    q->connect(pDlg,
                               SIGNAL(loginData(QString,QString)),
                               q_ptr,
                               SLOT(checkPassword(QString, QString)));
                    pDlg->exec();
                    if (isLoggedIn()) {
                        displayLogout();
                    }
                }
            }
        }
        return true;
    } else
        return false;
    q_ptr->update();
}

/*!
 * \internal
 *
 * \brief checks whether the click is inside the login frame.
 */
bool ExtendedTabWidgetPrivate::isInLoginFrame(int x, int y) {
    if (pLoginRect)
        return pLoginRect->contains(x, y, false);
    return false;
}

void ExtendedTabWidgetPrivate::resize() {
    clearFrames();
    q_ptr->update();
}

void ExtendedTabWidgetPrivate::paint(QPainter *painter, QPaintEvent *event) {

    int w = event->rect().width() - 10;
    int h = q_ptr->tabBar()->height();
    int tw = q_ptr->tabBar()->width();
    int sh = h * 0.6; // set the font height to be 60% of the total height.
    int frameWidth;

    QFont font = painter->font();
    font.setPixelSize(sh);
    painter->setFont(font);
    QFontMetrics fm = painter->fontMetrics();
    mFmh = fm.height();

    if (bShowLogin) {

        if (!pLoginRect) {
            pLoginRect = new QRect();
            mSw = fm.width(LOGOUT);
            frameWidth = mSw + 20;
            pLoginRect->setX(w - frameWidth);
            pLoginRect->setY((h - (mFmh + 6)) / 2);
            pLoginRect->setWidth(frameWidth);
            pLoginRect->setHeight(mFmh + 2);

            w -= (frameWidth + 2);
        } else
            w -= (pLoginRect->width() + 2);

        if (bShowFrame) {
            if (bButtonPressed)
                painter->setPen(mPalette.color(QPalette::Light));
            else
                painter->setPen(mPalette.color(QPalette::Dark));
            painter->drawLine(pLoginRect->x(), pLoginRect->y(), pLoginRect->x(), pLoginRect->y() + pLoginRect->height());
            painter->drawLine(pLoginRect->x() + 1, pLoginRect->y() + 1, pLoginRect->x() + 1, pLoginRect->y() + pLoginRect->height() - 1);
            painter->drawLine(pLoginRect->x() + 1, pLoginRect->y(), pLoginRect->x() + pLoginRect->width(), pLoginRect->y());
            painter->drawLine(pLoginRect->x() + 2, pLoginRect->y() + 1, pLoginRect->x() + pLoginRect->width() - 1, pLoginRect->y() + 1);

            painter->setPen(mPalette.color(QPalette::Light));
            painter->drawLine(pLoginRect->x() + 1, pLoginRect->y() + pLoginRect->height(), pLoginRect->x() + pLoginRect->width(), pLoginRect->y() + pLoginRect->height());
            painter->drawLine(pLoginRect->x() + 2, pLoginRect->y() + pLoginRect->height() - 1, pLoginRect->x() + pLoginRect->width() - 1, pLoginRect->y() + pLoginRect->height() - 1);
            painter->drawLine(pLoginRect->x() + pLoginRect->width(), pLoginRect->y() + 1, pLoginRect->x() + pLoginRect->width(), pLoginRect->y() + pLoginRect->height() - 1);
            painter->drawLine(pLoginRect->x() + pLoginRect->width() - 1, pLoginRect->y() + 2, pLoginRect->x() + pLoginRect->width() - 1, pLoginRect->y() + pLoginRect->height() - 2);
        }

        if (bButtonPressed)
            painter->setPen(mPalette.color(QPalette::Dark));
        else
            painter->setPen(mPalette.color(QPalette::Light));
        painter->setPen(mPalette.color(QPalette::WindowText));
        painter->drawText(pLoginRect->x() + 10, pLoginRect->y() + mFmh - 4, sLoginText);
    }

    if (bShowClock) {

        if (!pClockRect) {
            pClockRect = new QRect();
            if (bShowSeconds)
                mSw = fm.width(WITHSECONDS);
            else
                mSw = fm.width(NOSECONDS);
            frameWidth = mSw + 20;
// not used unless another thing is added before message
            pClockRect->setX(w - frameWidth);
            pClockRect->setY((h - (mFmh + 6)) / 2);
            pClockRect->setWidth(frameWidth);
            pClockRect->setHeight(mFmh + 2);

            w -= (frameWidth + 2);
        } else
            w -= (pClockRect->width() + 2);

        if (bShowFrame) {
            painter->setPen(mPalette.color(QPalette::Dark));
            painter->drawLine(pClockRect->x(), pClockRect->y(), pClockRect->x(), pClockRect->y() + pClockRect->height());
            painter->drawLine(pClockRect->x() + 1, pClockRect->y() + 1, pClockRect->x() + 1, pClockRect->y() + pClockRect->height() - 1);
            painter->drawLine(pClockRect->x() + 1, pClockRect->y(), pClockRect->x() + pClockRect->width(), pClockRect->y());
            painter->drawLine(pClockRect->x() + 2, pClockRect->y() + 1, pClockRect->x() + pClockRect->width() - 1, pClockRect->y() + 1);

            painter->setPen(mPalette.color(QPalette::Light));
            painter->drawLine(pClockRect->x() + 1, pClockRect->y() + pClockRect->height(), pClockRect->x() + pClockRect->width(), pClockRect->y() + pClockRect->height());
            painter->drawLine(pClockRect->x() + 2, pClockRect->y() + pClockRect->height() - 1, pClockRect->x() + pClockRect->width() - 1, pClockRect->y() + pClockRect->height() - 1);
            painter->drawLine(pClockRect->x() + pClockRect->width(), pClockRect->y() + 1, pClockRect->x() + pClockRect->width(), pClockRect->y() + pClockRect->height() - 1);
            painter->drawLine(pClockRect->x() + pClockRect->width() - 1, pClockRect->y() + 2, pClockRect->x() + pClockRect->width() - 1, pClockRect->y() + pClockRect->height() - 2);
        }

        painter->setPen(mPalette.color(QPalette::WindowText));
        painter->drawText(pClockRect->x() + 10, pClockRect->y() + mFmh - 4, sNow);

    }

    if (bShowMessages) {

        QString text;

        if (!pMessageRect) {


            pMessageRect = new QRect();
            mMessageWidth = fm.width(sMessageText);
            frameWidth = w - tw - 2;
            pMessageRect->setX(w - frameWidth);
            pMessageRect->setY((h - (mFmh + 6)) / 2);
            pMessageRect->setWidth(frameWidth);
            pMessageRect->setHeight(mFmh + 2);
            pMessageClip = new QRect();
            pMessageClip->setX(pMessageRect->x() + 4);
            pMessageClip->setY(pMessageRect->y() + 4);
            pMessageClip->setWidth(pMessageRect->width() - 8);
            pMessageClip->setHeight(pMessageRect->height() - 8);

        }

        if (mMessageWidth > pMessageRect->width()) { // message is too long.

            if (bMarquee) { // text is marqueed

                QString messageText = sMessageText + "\u2026 "; // ellipsis ... character followed by a space.
                int pos = mMarqueePos % messageText.length();
                text = messageText.mid(pos).append(messageText.left(pos));

            } else  // text is not marqueed so just put an ellipsis at the end
                text = fm.elidedText(sMessageText, Qt::ElideRight, pMessageRect->width());

        } else {

            text = sMessageText;

        }


        QPalette messagePalette(mPalette);

        if (bUseTempColor)
            messagePalette.setColor(QPalette::WindowText, mTempColor);
        else
            messagePalette.setColor(QPalette::WindowText, mTextColor);

        if (bUseTempBack && mTempBackground.color().isValid())
            painter->fillRect(*pMessageRect, mTempBackground);
        else if (mBackground.color().isValid())
            painter->fillRect(*pMessageRect, mBackground);

        if (bShowFrame) {
            painter->setPen(mPalette.color(QPalette::Dark));
            painter->drawLine(pMessageRect->x(), pMessageRect->y(), pMessageRect->x(), pMessageRect->y() + pMessageRect->height());
            painter->drawLine(pMessageRect->x() + 1, pMessageRect->y() + 1, pMessageRect->x() + 1, pMessageRect->y() + pMessageRect->height() - 1);
            painter->drawLine(pMessageRect->x() + 1, pMessageRect->y(), pMessageRect->x() + pMessageRect->width(), pMessageRect->y());
            painter->drawLine(pMessageRect->x() + 2, pMessageRect->y() + 1, pMessageRect->x() + pMessageRect->width() - 1, pMessageRect->y() + 1);

            painter->setPen(mPalette.color(QPalette::Light));
            painter->drawLine(pMessageRect->x() + 1, pMessageRect->y() + pMessageRect->height(), pMessageRect->x() + pMessageRect->width(), pMessageRect->y() + pMessageRect->height());
            painter->drawLine(pMessageRect->x() + 2, pMessageRect->y() + pMessageRect->height() - 1, pMessageRect->x() + pMessageRect->width() - 1, pMessageRect->y() + pMessageRect->height() - 1);
            painter->drawLine(pMessageRect->x() + pMessageRect->width(), pMessageRect->y() + 1, pMessageRect->x() + pMessageRect->width(), pMessageRect->y() + pMessageRect->height() - 1);
            painter->drawLine(pMessageRect->x() + pMessageRect->width() - 1, pMessageRect->y() + 2, pMessageRect->x() + pMessageRect->width() - 1, pMessageRect->y() + pMessageRect->height() - 2);
        }

        painter->setClipRect(*pMessageClip);

        painter->setPen(messagePalette.color(QPalette::WindowText));
        painter->drawText(pMessageRect->x() + 10, pMessageRect->y() + mFmh - 6, text);

    }
}

void ExtendedTabWidgetPrivate::displayLogin() {
    sLoginText = LOGIN;
}

void ExtendedTabWidgetPrivate::displayLogout() {
    sLoginText = LOGOUT;
}

void ExtendedTabWidgetPrivate::checkPassword(QString password) {
    if (bSimplelogin) {
        Q_Q(ExtendedTabWidget);
        bool result;
        if (bIgnoreCase)
            result = (password.toLower() == Base64::decode(mPassword).toLower());
        else
            result = (password == Base64::decode(mPassword));
        if (result) {
            emit q->loggedIn();
            bLoggedIn = true;
        } else
            emit q->passwordBad();
    }
}

void ExtendedTabWidgetPrivate::checkPassword(QString username, QString password) {
    Q_Q(ExtendedTabWidget);

    QString pw, un;

    if (bLogin) {
        if (!bIgnoreCase) {
            pw = mPasswords.value(username);
            if (pw.isEmpty()) {
                emit q->usernameBad();
                bLoggedIn = false;
                return;
            }
            bool result = (password == Base64::decode(pw));

            if (result) {
                emit q->loggedIn();
                bLoggedIn = true;
                sUsername = "";
            } else {
                emit q->passwordBad();
                bLoggedIn = false;
            }
        } else {

            pw = password.toLower();
            un = username.toLower();
            bool unFound = false;

            QListIterator<QString> it(mPasswords.keys());
            while (it.hasNext()) {
                QString id = it.next();
                if (id.toLower() == un) {
                    unFound = true;
                    QString storedPw = Base64::decode(mPasswords.value(id));
                    if (storedPw.toLower() == pw) {
                        emit q->loggedIn();
                        sUsername = un;
                        bLoggedIn = true;
                        return;
                    }
                    if (unFound) {
                        emit q->usernameBad();
                        return;
                    }
                }
            }
            emit q->passwordBad();
            sUsername = "";
            bLoggedIn = false;
        }
    }

}

void ExtendedTabWidgetPrivate::setIgnoreCase(bool ignoreCase) {
    bIgnoreCase = ignoreCase;
}

bool ExtendedTabWidgetPrivate::isLoggedIn() {
    return bLoggedIn;
}

void ExtendedTabWidgetPrivate::addPassword(QString password) {
    mPassword = Base64::encode(password.toLatin1());
}

void ExtendedTabWidgetPrivate::addPassword(QString id, QString password) {
    mPasswords.insert(id, Base64::encode(password.toLatin1()));
}

void ExtendedTabWidgetPrivate::nextSecond() {
    QString format = "hh:mm";
    if (bShowSeconds) {
        format += ":ss";
    }

    QDateTime now = QDateTime::currentDateTime();
    sNow = now.toString(format);
    mNow = now.toTime_t();

    q_ptr->update();
}

void ExtendedTabWidgetPrivate::showSeconds(bool show) {
    bShowSeconds = show;
    q_ptr->update();
    pClockRect = NULL;
    q_ptr->update();
}

void ExtendedTabWidgetPrivate::clearMessage() {
    sMessageText.clear();
    q_ptr->update();
}

void ExtendedTabWidgetPrivate::setMessageColor(QColor color) {
    mTextColor = color;
    q_ptr->update();
}

void ExtendedTabWidgetPrivate::setMessageBackground(QColor color) {
    mBackground = QBrush(color);
    q_ptr->update();
}

void ExtendedTabWidgetPrivate::setMessageBackground(QBrush brush) {
    mBackground = brush;
    q_ptr->update();
}

void ExtendedTabWidgetPrivate::timeout() {
    bUseTempColor = false;
    pTimeoutTimer->stop();
    pTimeoutTimer->deleteLater();
    pTimeoutTimer = NULL;
    clearMessage();
}

void ExtendedTabWidgetPrivate::setMessage(QString message, uint timeout) {
    sMessageText = message;
    mMarqueePos = 0;
    bUseTempColor = false;
    setMessage(timeout);
}

void ExtendedTabWidgetPrivate::setMessage(QColor color, QString message, uint timeout) {
    sMessageText = message;
    bUseTempColor = true;
    mTempColor = color;
    setMessage(timeout);
}

void ExtendedTabWidgetPrivate::setMessage(QColor color, QColor back, QString message, uint timeout) {
    sMessageText = message;
    bUseTempBack = true;
    bUseTempColor = true;
    mTempColor = color;
    mTempBackground = QBrush(back);
    setMessage(timeout);
}

void ExtendedTabWidgetPrivate::setMessage(QColor color, QBrush back, QString message, uint timeout) {
    sMessageText = message;
    bUseTempBack = true;
    bUseTempColor = true;
    mTempColor = color;
    mTempBackground = back;
    setMessage(timeout);
}

void ExtendedTabWidgetPrivate::setMessage(uint timeout) {
    if (pTimeoutTimer) { // new message before timeout cancel timeout.
        if (pTimeoutTimer->isActive()) {
            pTimeoutTimer->stop();
            pTimeoutTimer->deleteLater();
            pTimeoutTimer = NULL;
        }
    }

    if (timeout > 0) {
        if (pTimeoutTimer) {
            if (pTimeoutTimer->isActive()) {
                pTimeoutTimer->stop();
            }
        } else {
            pTimeoutTimer = new QTimer(q_ptr);
        }
        pTimeoutTimer->start(timeout);
    }

    // remove all new line characters, only single line message.
    int pos = sMessageText.indexOf('\n');
    while (pos != -1) {
        sMessageText.remove(pos, 1);
        pos = sMessageText.indexOf('\n');
    }
    q_ptr->update();
}

