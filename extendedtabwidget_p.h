#ifndef EXTENDEDTABWIDGETPRIVATE_H
#define EXTENDEDTABWIDGETPRIVATE_H

#include <QObject>
#include <QPalette>
#include <QPainter>
#include <QPaintEvent>
#include <QGuiApplication>
#include <QFont>
#include <QTabBar>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDateTime>

class ExtendedTabWidget;
class LoginDialog;
class SimpleLoginDialog;

class LoginDialogPrivate {
public:
    LoginDialogPrivate(LoginDialog *parent);

    void acceptLogin();

    void initBuild(bool largeText);

    LoginDialog *q_ptr;
    Q_DECLARE_PUBLIC(LoginDialog)
};

class SimpleLoginDialogPrivate {
public:
    SimpleLoginDialogPrivate(SimpleLoginDialog *parent);

    void acceptLogin();

    void initBuild(bool largeText);

    SimpleLoginDialog *q_ptr;
    Q_DECLARE_PUBLIC(SimpleLoginDialog)
};

class ExtendedTabWidgetPrivate {
    Q_DECLARE_PUBLIC(ExtendedTabWidget)
public:
    ExtendedTabWidgetPrivate(ExtendedTabWidget *parent);
    ~ExtendedTabWidgetPrivate();

    bool isInLoginFrame(int x, int y);

    ExtendedTabWidget *q_ptr;

    QMap<QString, QString> mPasswords;
    QString mPassword;
    bool bLoggedIn, bShowLogin, bLogin, bSimplelogin, bShowClock, bShowMessages;
    QPalette mPalette;
    QRect *pLoginRect, *pClockRect, *pMessageRect, *pMessageClip;
    int mSw, mFmh, mMessageWidth;

    // Clock stuff.
    QString sNow;
    uint mNow;
    bool bShowSeconds, bShowFrame;
    bool bIsInframe, bButtonPressed, bIgnoreCase;

    void showFrame(bool frame);

    QTimer *pClockTimer;
    QTimer *pMarqueeTimer;
    QTimer *pTimeoutTimer;

    void showLogin(bool);
    void setLogin(bool);
    void setSimpleLogin(bool);
    void showClock(bool);
    void showMessages(bool);

    void nextSecond();
    void showSeconds(bool);

    // Login stuff
    QString sLoginText;

    static const QString LOGIN;
    static const QString LOGOUT;
    static const QString WITHSECONDS;
    static const QString NOSECONDS;
    const static int CLOCK_TIME = 1000;
    const static int MARQUEE_TIME = 100;

    bool mousePressEvent(QMouseEvent *event);
    bool mouseReleaseEvent(QMouseEvent *event);
    void paint(QPainter *painter, QPaintEvent *event);
    void resize();

    void displayLogin();
    void displayLogout();
    void checkPassword(QString value);
    void checkPassword(QString username, QString value);
    void setIgnoreCase(bool);
    void setLargeTextForLoginDialog(bool);
    bool isLoggedIn();
    void addPassword(QString password);
    void addPassword(QString, QString);
    void setMarqueeSpeed(int);
    void timeout();
    void clearFrames();


    // message stuff
    QString sMessageText;
    QString sUsername;
    QColor mTextColor, mTempColor;
    QBrush mBackground, mTempBackground;
    bool bMarquee, bUseTempColor, bUseTempBack;
    int mMarqueePos, mMarqueeSpeed;

    uint mTimeout;
    void clearMessage();
    void setMessage(uint);
    void setMessageColor(QColor color);
    void setMessageBackground(QColor);
    void setMessageBackground(QBrush);
    void setMessage(QString message, uint timeout=0);
    void setMessage(QColor, QString message, uint timeout=0);
    void setMessage(QColor, QColor, QString message, uint timeout=0);
    void setMessage(QColor, QBrush, QString message, uint timeout=0);
    void setMarquee(bool);
    void startMarqueeTimerIfRequired();
    void stopMarqueeTimer();
    void updateMarquee();
    QBrush messageBackgroundBrush();
    QColor messageTextColor();

};

#endif // EXTENDEDTABWIDGETPRIVATE_H
