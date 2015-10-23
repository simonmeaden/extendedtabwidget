#ifndef EXTENDEDTABWIDGET_H
#define EXTENDEDTABWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QPainter>
#include <QDialog>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QGuiApplication>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QTimer>

#include "extendedtabwidget_global.h"
#include "extendedtabwidget_p.h"

/*!
 * \internal
 *
 * \brief A login dialog for use with standard login.
 */
class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent);
    ~LoginDialog();

signals:
    void loginData(QString, QString);

public slots:
    void acceptLogin();

protected:
    LoginDialogPrivate *d_ptr;
    QLineEdit *userEdit, *passEdit;

    Q_DECLARE_PRIVATE(LoginDialog)

};

/*!
 * \internal
 *
 * \brief A login dialog for use with simple login.
 */
class SimpleLoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit SimpleLoginDialog(QWidget *parent);
    ~SimpleLoginDialog();

signals:
    void simpleLoginData(QString);

public slots:
    void acceptLogin();

protected:
    SimpleLoginDialogPrivate *d_ptr;
    QLineEdit *passEdit;

    Q_DECLARE_PRIVATE(SimpleLoginDialog)

};

class EXTENDEDTABWIDGETSHARED_EXPORT ExtendedTabWidget : public QTabWidget {

    Q_OBJECT
    Q_DECLARE_PRIVATE(ExtendedTabWidget)
    Q_PROPERTY(bool clock
               READ isClockEnabled
               WRITE showClock
               DESIGNABLE true)
    Q_PROPERTY(bool messages
               READ isMessagesEnabled
               WRITE showMessages
               DESIGNABLE true)
    Q_PROPERTY(bool login
               READ isLoginEnabled
               WRITE setLogin
               DESIGNABLE true)
    Q_PROPERTY(bool simplelogin
               READ isSimpleLoginEnabled
               WRITE setSimpleLogin
               DESIGNABLE true)
    Q_PROPERTY(bool ignoreCase
               READ isIgnoreCase
               WRITE setIgnoreCase
               DESIGNABLE true)
    Q_PROPERTY(bool marquee
               READ isMarqueeEnabled
               WRITE setMarquee
               DESIGNABLE true)

public:

    ExtendedTabWidget(QWidget *parent=0);
    ~ExtendedTabWidget();

    bool isLoggedIn();
    QString username();
    bool isClockEnabled();
    bool isLoginEnabled();
    bool isSimpleLoginEnabled();
    bool isMessagesEnabled();
    bool isIgnoreCase();
    bool isMarqueeEnabled();
    QBrush messageBackgroundBrush();
    QColor messageTextColor();

public slots:
    void showClock(bool);
    void showLogin(bool);
    void setLogin(bool);
    void setSimpleLogin(bool);
    void showMessages(bool);

    void showFrame(bool);

    void setPassword(QString password);
    void addPassword(QString, QString password);
    void clearPasswords();
    void setIgnoreCase(bool);

    void showSeconds(bool);

    void setMessageColor(QColor color);
    void setMessageBackground(QColor);
    void setMessageBackground(QBrush);
    void setMessage(QString message, uint timeout=0);
    void setMessage(QColor color, QString message, uint timeout=0);
    void setMessage(QColor color, QColor back, QString message, uint timeout=0);
    void setMessage(QColor color, QBrush back, QString message, uint timeout=0);
    void setMarquee(bool marquee);
    void setMarqueeSpeed(int);
    void clearMessage() { d_ptr->clearMessage(); }

protected slots:
    void checkPassword(QString password);
    void checkPassword(QString, QString);
    void nextSecond();
    void updateMarquee();
    void timeout();

signals:
    void loggedOut();
    void loggedIn();
    void passwordBad();
    void usernameBad();

protected:

    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void resizeEvent(QResizeEvent *evt);

private:
    ExtendedTabWidgetPrivate *d_ptr;

};


#endif // EXTENDEDTABWIDGET_H
