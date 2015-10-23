#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QColorDialog>

class ExtendedTabWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setLoginText();
    void setLogoutText();
    void setPasswordBadText();
    void setUsernameBadText();

    void enableLogin(bool);
    void setLoginType(bool);

    void showMessages(bool);
    void setMarquee(bool);
    void setMarqueeSpeed(int);
    void setTemporaryColor(int);
    void setPermanentColor();
    void setTemporaryBack(int);
    void setPermanentBack();
    void sendMessage();

    void showClock(bool);
    void setSeconds(bool);

    void showFrame(bool);


private:

    ExtendedTabWidget *tabs;
    QLabel *label;
    bool bSimpleLogin;
    QColor mTempColor, mTempBack;
    QCheckBox *pLogin, *pLoginType, *pMessage, *pMarquee, *pClock, *pSeconds, *pFrame;
    QSpinBox *pMarqueeSpeed;
    QPushButton *pSendMsgBtn, *pChoosePermBackBtn, *pChoosePermColorBtn;
    QLineEdit *pMessageEdit;
    QComboBox *pTempColorBox, *pTempBackBox;

};

#endif // MAINWINDOW_H
