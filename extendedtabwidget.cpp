#include "extendedtabwidget.h"
#include "extendedtabwidget_p.h"

/*!
 * \class ExtendedTabWidget
 * \contentspage index.html
 *
 * \brief An extension of QTabWidget with the ability to various things  in the unused space to the right of the tabs.
 *
 * There are three things that can be added :
 * \list 1
 * \li A login button
 * \li A digital clock
 * \li A single line message box.
 * \endlist
 *
 * More than one of the options can be added at the same time with the order from right to left being
 * the login button, the clock and finally the message box, with the message box taking all available
 * free space.
 *
 * The login button is displayed/hidden using the ExtendedTabWidget::showLogin(bool) method, with
 * true displaying the button.
 *
 * There are two varieties of login, a fairly standard login dialog with both username and a password
 * edit boxes and a 'simple' login which has only a password edit box. Use the ExtendedTabWidget::setLogin(bool) or
 * ExtendedTabWidget::setSimpleLogin(bool) methods, with a value of true, to set the login method.
 *
 * If the login username/password is valid the button will change to display "Log Out" and a
 * ExtendedTabWidget::loggedIn() signal is sent, otherwise either a ExtendedTabWidget::usernameBad()
 * signal  or a ExtendedTabWidget::passwordBad() signal will be sent, depending on the fault. The
 * ExtendedTabWidget::isLoggedIn() method can be used to check whether a user is logged in, and the
 * ExtendedTabWidget::username() method can be used to recover the username of the logged in user. If
 * the simple login dialog is used or the ExtendedTabWidget::usernameBad() signal was sent then
 * ExtendedTabWidget::username() will return an empty QString.
 *
 * Clicking on the button when "Log Out" is displayed will cause a ExtendedTabWidget::loggedOut() signal to
 * be sent. ExtendedTabWidget::loggedIn() returns false and ExtendedTabWidget::username() returns an empty
 * string at this point.
 *
 * The simple login method requires that a password is set, only one allowed, using the ExtendedTabWidget::setPassword(QString)
 * method. The standard login requires that one or more username/password pairs be set up using the
 * ExtendedTabWidget::insertPassword(QString username, QString password) method, once for each user. User names must be
 * unique, adding a second entry with the same username will overwrite the first entry.
 *
 * Setting the value of the ignore case flag to true using the ExtendedTabWidget::setIgnoreCase(bool) method will
 * cause the character case of the password and username to be ignored. If false the case of the username and password
 * are checked, ie UsErNaMe does not equal username.
 *
 * ExtendedTabWidget::clearPasswords() will remove all passwords from the list. Passwords and usernames for the simple
 * and standard login dialogs are kept separately so it is possible to switch between the two methods if required.
 *
 * The clock is displayed when the ExtendedTabWidget::enableClock(bool) is called with the value of true.
 * By default the digital clock is displayed without seconds. The display seconds use the
 * ExtendedTabWidget::showseconds(bool) method.
 *
 * The message box can be displayed using the ExtendedTabWidget::enableMessages(bool) method with a value of true.
 * Messages are sent using the ExtendedTabWidget::setMessage(QString, uint) method with the optional uint value
 * indicating a timeout in seconds before the message is erased. Text colour can be modified by using
 * the alternative ExtendedTabWidget::setMessage(QColor, QString, uint) method, alternatively use the
 * ExtendedTabWidget::setMessageColor(QColor color) to permanently change the text colour.
 *
 * If messages are too long for the available space you can use the marquee option which causes the message to rotate along
 * message box. To enable this use the ExtendedTabWidget::enableMarquee(bool) with a value of true.
 *
 * Each can be drawn with or without a frame, using the ExtendedTabWidget::showFrame(bool) method. By default a frame
 * is shown.
 *
 * \since 5.0
 */

/*!
 * \property ExtendedTabWidget::clock
 *
 * \brief enable the clock
 *
 * This property describes whether the clock is displayed.
 *
 * \sa ExtendedTabWidget::enableClock(bool), ExtendedTabWidget::isClockEnabled()
 */

/*!
 * \property ExtendedTabWidget::messages
 *
 * \brief enable the message box.
 *
 * This property describes whether the message box is displayed.
 *
 * \sa ExtendedTabWidget::enableMessages(bool), ExtendedTabWidget::isMessagesEnabled()
 */

/*!
 * \property ExtendedTabWidget::login
 *
 * \brief enable the standard login dialog.
 *
 * This property describes whether the login button is displayed.
 *
 * \sa ExtendedTabWidget::enableLogin(bool), ExtendedTabWidget::isLoginEnabled()
 */

/*!
 * \property ExtendedTabWidget::simplelogin
 *
 * \brief enable the simple login dialog.
 *
 * This property describes whether the login button is displayed.
 *
 * \sa ExtendedTabWidget::enableSimpleLogin(bool), ExtendedTabWidget::isSimpleLoginEnabled()
 */

/*!
 * \property ExtendedTabWidget::marquee
 *
 * \brief enable the message box marquee is enabled.
 *
 * This property describes whether the marquee is enabled.
 *
 * \sa ExtendedTabWidget::enableMarquee(bool), ExtendedTabWidget::isMarqueeEnable()
 */

/*!
 * \fn bool ExtendedTabWidget::loggedIn()
 *
 * This signal is emitted when either the simple or the standard login dialog indicate that the user has
 * successfully logged in.
 *
 * \sa ExtendedTabWidget::loggedOut(), ExtendedTabWidget::usernameBad(), ExtendedTabWidget::passwordBad()
 */

/*!
 * \fn bool ExtendedTabWidget::loggedOut()
 *
 * This signal is emitted when either the simple or the standard login dialog indicate that the user has
 * successfully logged out.
 *
 * \sa ExtendedTabWidget::loggedIn(), ExtendedTabWidget::usernameBad(), ExtendedTabWidget::passwordBad()
 */

/*!
 * \fn bool ExtendedTabWidget::passwordBad()
 *
 * This signal is emitted when either the simple or the standard login dialog indicate that the password
 * was bad for the username.
 *
 * \sa ExtendedTabWidget::usernameBad(), ExtendedTabWidget::loggedIn(), ExtendedTabWidget::loggedOut();
 */

/*!
 * \fn bool ExtendedTabWidget::usernameBad()
 *
 * This signal is emitted when either the simple or the standard login dialog indicate that the username
 * was not one of the supplied values
 *
 * \sa ExtendedTabWidget::passwordBad(), ExtendedTabWidget::loggedIn(), ExtendedTabWidget::loggedOut()
 */

/*!
 * \brief Constructs a tabbed widget with parent parent.
 */
ExtendedTabWidget::ExtendedTabWidget(QWidget *parent) :
    QTabWidget(parent),
    d_ptr(new ExtendedTabWidgetPrivate(this)) {

}

/*!
 * \brief Destroys the tabbed widget.
 */
ExtendedTabWidget::~ExtendedTabWidget() {

}

// General stuff =================================================================
/*!
 * \brief Shows a frame around the displayed items if true.
 *
 * The show frame flag operates on all of the extensions, so either they all
 * have frames or none of them have frames.
 */
void ExtendedTabWidget::showFrame(bool show) {
    d_ptr->showFrame(show);
}

/*!
 * \brief Enables/disables the display of the digital clock
 *
 * Shows the clock if true, otherwise hides it.
 */
void ExtendedTabWidget::showClock(bool clock) {
    d_ptr->showClock(clock);
}

/*!
 * \brief Enables/disables the display of the login button
 *
 * Shows the login button if true, otherwise hides it.
 */
void ExtendedTabWidget::showLogin(bool clock) {
    d_ptr->showLogin(clock);
}

/*!
 * \brief Enables/disables the login with a standard username/password dialog if the value is true.
 *
 * Enabling standard login disables simple login if previously set. By default neither simple or standard
 * login is set.
 */
void ExtendedTabWidget::setLogin(bool login) {
    d_ptr->setLogin(login);
}

/*!
 * \brief Enables/disables the login with a simpler password only dialog if the value is true.
 *
 * Enabling simple login disables standard login if previously set. By default neither simple or standard
 * login is set.
 */
void ExtendedTabWidget::setSimpleLogin(bool simpleLogin) {
    d_ptr->setSimpleLogin(simpleLogin);
}

/*!
 * \brief Enables/disables the message box if the value is true.
 */
void ExtendedTabWidget::showMessages(bool messages) {
    d_ptr->showMessages(messages);
}

/*!
 * \brief Returns true if the clock is enabled, otherwise false.
 */
bool ExtendedTabWidget::isClockEnabled() {
    Q_D(ExtendedTabWidget);
    return d->bShowClock;
}

/*!
 * \brief Returns true if the login is enabled, otherwise false.
 */
bool ExtendedTabWidget::isLoginEnabled() {
    Q_D(ExtendedTabWidget);
    return d->bLogin;
}

/*!
 * \brief Returns true if the user is logged in, otherwise false.
 */
bool ExtendedTabWidget::isLoggedIn() {
    Q_D(ExtendedTabWidget);
    return d->bLoggedIn;
}

/*!
 * \brief Returns the username if logged in with the standard login, otherwise an empty string is returned.
 */
QString ExtendedTabWidget::username() {
    Q_D(ExtendedTabWidget);
    return d->sUsername;
}

/*!
 * \brief Returns true if the simple login is enabled, otherwise false.
 */
bool ExtendedTabWidget::isSimpleLoginEnabled() {
    Q_D(ExtendedTabWidget);
    return d->bSimplelogin;
}

/*!
 * \brief Returns true if the message box is enabled, otherwise false.
 */
bool ExtendedTabWidget::isMessagesEnabled() {
    Q_D(ExtendedTabWidget);
    return d->bShowMessages;
}

void ExtendedTabWidget::resizeEvent(QResizeEvent *evt) {
    d_ptr->resize();
    QTabWidget::resizeEvent(evt);
}


void ExtendedTabWidget::paintEvent(QPaintEvent *evt) {
    QTabWidget::paintEvent(evt);
    QPainter *painter = new QPainter(this);
    d_ptr->paint(painter, evt);
    delete painter;
}

void ExtendedTabWidget::mousePressEvent(QMouseEvent *event) {
    if (!d_ptr->mousePressEvent(event))
        QTabWidget::mousePressEvent(event);
}

void ExtendedTabWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (!d_ptr->mouseReleaseEvent(event))
        QTabWidget::mouseReleaseEvent(event);
}


// Login stuff =====================================================================
/*!
 * \brief Sets the password for the simple login dialog.
 */
void ExtendedTabWidget::setPassword(QString password) {
    d_ptr->addPassword(password);
}

/*!
 * \brief Adds a username/password pair for the standard login dialog.
 *
 * Usernames must be unique, inserting a repeat username will overwrite the earlier version.
 */
void ExtendedTabWidget::addPassword(QString id, QString password) {
    d_ptr->addPassword(id, password);
}

/*!
 * \internal
 * \brief checks the password for a simple login dialog.
 */
void ExtendedTabWidget::checkPassword(QString password) {
    d_ptr->checkPassword(password);
}

/*!
 * \internal
 * \brief checks the username/password for a standard login dialog.
 */
void ExtendedTabWidget::checkPassword(QString id, QString password) {
    d_ptr->checkPassword(id, password);
}

/*!
 * \briefClears all password/usernames.
 */
void ExtendedTabWidget::clearPasswords() {
    Q_D(ExtendedTabWidget);
    d->mPassword.clear();
    d->mPasswords.clear();
}

/*!
 * \brief Sets the marquee speed in characters a second.
 *
 * The default is 10 characters a second. To slow it down enter a lower
 * number of characters a second and to speed it up a higher number.
 */
void ExtendedTabWidget::setMarqueeSpeed(int speed) {
   d_ptr->setMarqueeSpeed(speed);
}


/*!
 * \brief Sets the value of the ignore case flag. If set to true all password/username
 * checks ignore the character case.
 */
void ExtendedTabWidget::setIgnoreCase(bool ignoreCase) {
    d_ptr->setIgnoreCase(ignoreCase);
}

/*!
 * \brief Returns the value of the ignore case flag.
 *
 * True if the character case of usernames and passwords are to be ignored.
 */
bool ExtendedTabWidget::isIgnoreCase() {
    Q_D(ExtendedTabWidget);
    return d->bIgnoreCase;
}

// Clock stuff ======================================================================
/*!
 * \fn void ExtendedTabWidget::showSeconds(bool show)
 *
 * \brief Set the show seconds flag.
 *
 * Show the optional show seconds flag. Displays seconds when true, otherwise the
 * seconds are not displayed.
 */
void ExtendedTabWidget::showSeconds(bool show) {
    d_ptr->showSeconds(show);
}

/*!
 * \internal
 *
 * \brief Triggers the clock update.
 *
 * This is necessary as it is not possible
 * to operate a QTimer in the private class.
 */
void ExtendedTabWidget::nextSecond() {
    d_ptr->nextSecond();
}

// Message stuff ====================================================================
/*!
  * \brief Sets the text colour the message is drawn in.
  *
  * This can temporarily be overridden by using the ExtendedTabWidget::setMessage() method
  * with the text color and background color methods.
  *
  * \sa ExtendedTabWidget::messageBackgroundBrush(),
  *     ExtendedTabWidget::setMessageBackground(QBrush),
  *     ExtendedTabWidget::setMessageBackground(QColor)

 */
void ExtendedTabWidget::setMessageColor(QColor color) {
    d_ptr->setMessageColor(color);
}

/*!
  * \brief Sets the background brush the message is drawn on.
  *
  * This can temporarily be overridden by using the ExtendedTabWidget::setMessage() method
  * with the text color and background color methods.
  *
  * \sa ExtendedTabWidget::messageBackgroundBrush(),
  *     ExtendedTabWidget::setMessageBackground(QColor),
  *     ExtendedTabWidget::setMessageColor(QColor)
 */
void ExtendedTabWidget::setMessageBackground(QBrush brush) {
    d_ptr->setMessageBackground(brush);
}

/*!
  * \brief Sets the background brush colour the message is drawn on.
  *
  * This can temporarily be overridden by using the ExtendedTabWidget::setMessage() method
  * with the text color and background color methods.
  *
  * \sa ExtendedTabWidget::messageBackgroundBrush(),
  *     ExtendedTabWidget::setMessageBackground(QBrush),
  *     ExtendedTabWidget::setMessageColor(QColor)
 */
void ExtendedTabWidget::setMessageBackground(QColor color) {
    d_ptr->setMessageBackground(color);
}

/*!
 * \brief Sets the message text, with an optional timeout in seconds.
 *
 * Sets the text message for the tab bar. This is only a single line message so "\n"
 * characters are removed. There really is not enough room for a multi line MessageTabWidget
 * within the tab bar.
 */
void ExtendedTabWidget::setMessage(QString message, uint timeout) {
    d_ptr->setMessage(message, timeout);
}

/*!
 * \brief Sets the message text and text colour, with an optional timeout in seconds.
 *
 * Sets the text message and colour for the tab bar. The optional timeout
 * will cause the message to  disappear after timeout seconds.
 *
 * This is only a single line message so "\n" characters are removed. There
 * really is not enough room for a multi line MessageTabWidget within the
 * tab bar.
 *
 * This is the equivalent of
 * \code
 * tab->setMessageColor(color);
 * tab->setMessage(message, timeout);
 * \endcode
 *
 */
void ExtendedTabWidget::setMessage(QColor color, QString message, uint timeout) {
    d_ptr->setMessage(color, message, timeout);
}

/*!
 * \brief Sets the message text and text colour and background colour, with an optional timeout in seconds.
 *
 * Sets the text message and colour for the tab bar. The optional timeout
 * will cause the message to  disappear after timeout seconds.
 *
 * This is only a single line message so "\n" characters are removed. There
 * really is not enough room for a multi line MessageTabWidget within the
 * tab bar.
 *
 * This is the equivalent of
 * \code
 * tab->setMessageColor(color);
 * tab->setMessage(message, timeout);
 * \endcode
 *
 */
void ExtendedTabWidget::setMessage(QColor color, QColor back, QString message, uint timeout) {
    d_ptr->setMessage(color, back, message, timeout);
}

/*!
 * \brief Sets the message text and text colour and background brush, with an optional timeout in seconds.
 *
 * Sets the text message and colour for the tab bar. The optional timeout
 * will cause the message to  disappear after timeout seconds.
 *
 * This is only a single line message so "\n" characters are removed. There
 * really is not enough room for a multi line MessageTabWidget within the
 * tab bar.
 *
 * This is the equivalent of
 * \code
 * tab->setMessageColor(color);
 * tab->setMessage(message, timeout);
 * \endcode
 *
 */
void ExtendedTabWidget::setMessage(QColor color, QBrush back, QString message, uint timeout) {
    d_ptr->setMessage(color, back, message, timeout);
}

/*!
 * \brief The permanent background brush.
 *
 * This QBrush object defines the standard background for messages.
 *
 * \sa ExtendedTabWidget::setMessageBackground(QBrush)
 */
QBrush ExtendedTabWidget::messageBackgroundBrush() {
    return d_ptr->messageBackgroundBrush();
}

/*!
 * \brief The permanent background brush.
 *
 * This QColor defines the standard text color for messages.
 *
 * \sa ExtendedTabWidget::setMessageColor(QColor)
 */
QColor ExtendedTabWidget::messageTextColor() {
    return d_ptr->messageTextColor();
}

/*!
 * \internal.
 * \brief timeout for message timeout timer.
 */
void ExtendedTabWidget::timeout() {
    d_ptr->timeout();
}

/*!
 * \brief Sets the message text scrolling.
 *
 * Sets the text message to scrolling if the text is longer than the available space.
 * If the text will fit inside the available space then this is ignored.
 */
void ExtendedTabWidget::setMarquee(bool marquee) {
    d_ptr->setMarquee(marquee);
}

/*!
 * \brief Sets the message text scrolling.
 *
 * Sets the text message to scrolling if the text is longer than the available space.
 * If the text will fit inside the available space then this is ignored.
 */
bool ExtendedTabWidget::isMarqueeEnabled() {
    Q_D(ExtendedTabWidget);
    return d->bMarquee;
}

void ExtendedTabWidget::updateMarquee() {
    d_ptr->updateMarquee();
}

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    d_ptr(new LoginDialogPrivate(this)) {
}

LoginDialog::~LoginDialog() {
}

void LoginDialog::acceptLogin() {
    d_ptr->acceptLogin();
}


SimpleLoginDialog::SimpleLoginDialog(QWidget *parent) :
    QDialog(parent),
    d_ptr(new SimpleLoginDialogPrivate(this)) {
}

SimpleLoginDialog::~SimpleLoginDialog() {
}

void SimpleLoginDialog::acceptLogin() {
    d_ptr->acceptLogin();
}
