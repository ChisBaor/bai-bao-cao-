#include "AuthPage.h"
#include "AuthService.h"
#include <QCheckBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>
#include <QStackedWidget>
#include <QTime>
#include <QVBoxLayout>

namespace {
QLabel *label(const QString &text, const char *objectName = nullptr) {
    auto *l = new QLabel(text);
    l->setWordWrap(true);
    if (objectName) l->setObjectName(objectName);
    return l;
}
QLineEdit *field(const QString &hint, bool password = false) {
    auto *edit = new QLineEdit;
    edit->setPlaceholderText(hint);
    edit->setMinimumHeight(50);
    if (password) edit->setEchoMode(QLineEdit::Password);
    return edit;
}
void addField(QVBoxLayout *layout, const QString &name, QLineEdit *edit) {
    layout->addWidget(label(name, "fieldLabel"));
    layout->addWidget(edit);
}
QWidget *scrollPage(QWidget *content) {
    auto *wrapper = new QWidget;
    auto *layout = new QVBoxLayout(wrapper);
    layout->setContentsMargins(0,0,0,0);
    auto *scroll = new QScrollArea;
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setWidget(content);
    layout->addWidget(scroll);
    return wrapper;
}
} // namespace

AuthPage::AuthPage(QWidget *parent): QWidget(parent) {
    auto *outer = new QVBoxLayout(this);
    outer->setContentsMargins(0,0,0,0);
    outer->setSpacing(0);

    auto *strip = new QWidget;
    strip->setFixedHeight(44);
    auto *stripRow = new QHBoxLayout(strip);
    stripRow->setContentsMargins(28,0,28,0);
    stripRow->addWidget(label(QTime::currentTime().toString("h:mm")));
    stripRow->addStretch();
    stripRow->addWidget(label("●  ▰"));
    outer->addWidget(strip);

    pages_ = new QStackedWidget;
    outer->addWidget(pages_,1);

    // Login — portrait composition based on Figma frame 2006:8413.
    auto *loginContent = new QWidget;
    auto *l = new QVBoxLayout(loginContent);
    l->setContentsMargins(24,20,24,24);
    l->setSpacing(0);
    l->addSpacing(20);
    auto *icon = label("PB", "brandIcon");
    icon->setAlignment(Qt::AlignCenter);
    icon->setFixedSize(62,62);
    l->addWidget(icon);
    l->addSpacing(16);
    l->addWidget(label("Phonebook", "brandName"));
    l->addSpacing(34);
    l->addWidget(label("Hello,\nwelcome back.", "authHeading"));
    l->addSpacing(6);
    l->addWidget(label("Log in to connect with the people who matter.", "subtleText"));
    l->addSpacing(28);
    loginName_ = field("Username or email");
    loginPassword_ = field("Password", true);
    addField(l, "Username", loginName_);
    l->addSpacing(14);
    addField(l, "Password", loginPassword_);

    // Inline error notification, matching the two Figma login error states.
    // Hide the whole container when there is no error so it takes no space.
    loginErrorContainer_ = new QWidget;
    auto *errorContainerLayout = new QVBoxLayout(loginErrorContainer_);
    errorContainerLayout->setContentsMargins(0, 12, 0, 0);
    errorContainerLayout->setSpacing(0);
    auto *errorCard = new QFrame;
    errorCard->setObjectName("loginErrorCard");
    auto *errorRow = new QHBoxLayout(errorCard);
    errorRow->setContentsMargins(12, 11, 12, 11);
    errorRow->setSpacing(9);

    loginErrorIcon_ = label("ⓘ", "loginErrorIcon");
    loginErrorIcon_->setFixedWidth(18);
    loginErrorIcon_->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    errorRow->addWidget(loginErrorIcon_);

    auto *errorTexts = new QVBoxLayout;
    errorTexts->setContentsMargins(0, 0, 0, 0);
    errorTexts->setSpacing(3);
    loginErrorTitle_ = label("", "loginErrorTitle");
    loginErrorDetail_ = label("", "loginErrorDetail");
    errorTexts->addWidget(loginErrorTitle_);
    errorTexts->addWidget(loginErrorDetail_);
    errorRow->addLayout(errorTexts, 1);
    errorContainerLayout->addWidget(errorCard);

    // Scoped style: the warning box won't change other screens.
    errorCard->setStyleSheet(R"(
        QFrame#loginErrorCard {
            background-color: #F9EAE6;
            border: none;
            border-radius: 12px;
        }
        QLabel#loginErrorIcon, QLabel#loginErrorTitle, QLabel#loginErrorDetail {
            background: transparent;
            color: #BD4B42;
        }
        QLabel#loginErrorIcon { font-size: 16px; }
        QLabel#loginErrorTitle { font-size: 12px; font-weight: 600; }
        QLabel#loginErrorDetail { font-size: 11px; }
    )");
    l->addWidget(loginErrorContainer_);
    loginErrorContainer_->hide();
    l->addSpacing(15);
    auto *remember = new QCheckBox("Remember username on this computer");
    remember->setObjectName("rememberUser");
    QSettings prefs;
    remember->setChecked(prefs.value("login/remember", false).toBool());
    if (remember->isChecked()) loginName_->setText(prefs.value("login/username").toString());
    l->addWidget(remember);
    l->addSpacing(24);
    auto *loginButton = new QPushButton("Log in");
    loginButton->setMinimumHeight(50);
    l->addWidget(loginButton);
    l->addSpacing(12);
    switchAccountButton_ = new QPushButton("Log in with another account");
    switchAccountButton_->setObjectName("textButton");
    l->addWidget(switchAccountButton_);
    switchAccountButton_->hide();

    goRegisterButton_ = new QPushButton("No account yet? Sign up");
    goRegisterButton_->setObjectName("textButton");
    l->addWidget(goRegisterButton_);
    l->addSpacing(30);
    auto *privacy = label("Private contacts. Peace of mind.", "subtleText");
    privacy->setAlignment(Qt::AlignCenter);
    l->addWidget(privacy);
    l->addStretch();
    pages_->addWidget(scrollPage(loginContent));
    connect(loginButton, &QPushButton::clicked, this, [this,remember]() {
        QSettings settings;
        settings.setValue("login/remember", remember->isChecked());
        settings.setValue("login/username", remember->isChecked() ? loginName_->text().trimmed() : QString());
        doLogin();
    });
    connect(loginPassword_, &QLineEdit::returnPressed, loginButton, &QPushButton::click);
    connect(goRegisterButton_, &QPushButton::clicked, this, [this]() { pages_->setCurrentIndex(1); });
    connect(switchAccountButton_, &QPushButton::clicked, this, [this]() {
        loginName_->clear();
        loginPassword_->clear();
        loginErrorContainer_->hide();
        switchAccountButton_->hide();
        goRegisterButton_->show();
        loginName_->setFocus();
    });
    // Automatically remove the previous error as soon as the user edits a field.
    auto clearLoginAlert = [this]() {
        loginErrorContainer_->hide();
        switchAccountButton_->hide();
        goRegisterButton_->show();
    };
    connect(loginName_, &QLineEdit::textChanged, this, clearLoginAlert);
    connect(loginPassword_, &QLineEdit::textChanged, this, clearLoginAlert);

    // Sign-up screen (same shared style; business logic unchanged).
    auto *registerContent = new QWidget;
    auto *r = new QVBoxLayout(registerContent);
    r->setContentsMargins(24,20,24,24);
    r->setSpacing(0);
    auto *back = new QPushButton("‹  Log in");
    back->setObjectName("textButton");
    r->addWidget(back,0,Qt::AlignLeft);
    r->addSpacing(25);
    r->addWidget(label("Create account", "authHeading"));
    r->addSpacing(5);
    r->addWidget(label("Keep everyone important in one place.", "subtleText"));
    r->addSpacing(26);
    regName_ = field("Choose a username");
    regEmail_ = field("Email address");
    regPassword_ = field("At least 8 characters", true);
    regConfirm_ = field("Confirm password", true);
    addField(r, "Username", regName_); r->addSpacing(14);
    addField(r, "Email", regEmail_); r->addSpacing(14);
    addField(r, "Password", regPassword_); r->addSpacing(14);
    addField(r, "Confirm password", regConfirm_);
    r->addSpacing(30);
    auto *regButton = new QPushButton("Sign up");
    regButton->setMinimumHeight(50);
    r->addWidget(regButton);
    r->addStretch();
    pages_->addWidget(scrollPage(registerContent));
    connect(regButton, &QPushButton::clicked, this, &AuthPage::doRegister);
    connect(back, &QPushButton::clicked, this, [this]() { pages_->setCurrentIndex(0); });
}

void AuthPage::resetForm() {
    loginPassword_->clear();
    regPassword_->clear();
    regConfirm_->clear();
    loginErrorContainer_->hide();
    switchAccountButton_->hide();
    goRegisterButton_->show();
    pages_->setCurrentIndex(0);
}
void AuthPage::doLogin() {
    QString error;
    const Session session = AuthService::login(loginName_->text(), loginPassword_->text(), &error);
    if (!session.valid()) {
        // AuthService currently returns this exact error when a valid-password
        // login is attempted for an account with Status != ACTIVE.
        const bool locked = (error == QStringLiteral("Tai khoan da bi khoa. Lien he quan tri vien."));
        loginErrorIcon_->setText(locked ? QStringLiteral("🔒") : QStringLiteral("ⓘ"));
        loginErrorTitle_->setText(locked ? QStringLiteral("Account locked") : QString());
        loginErrorTitle_->setVisible(locked);
        loginErrorDetail_->setText(locked
            ? QStringLiteral("You cannot access this account right now. Please contact an administrator for help.")
            : QStringLiteral("The username or password is incorrect. Please check and try again."));
        loginErrorContainer_->show();
        switchAccountButton_->setVisible(locked);
        goRegisterButton_->setVisible(!locked);
        return;
    }
    loginErrorContainer_->hide();
    switchAccountButton_->hide();
    goRegisterButton_->show();
    loginPassword_->clear();
    emit authenticated(session);
}
void AuthPage::doRegister() {
    if (regPassword_->text() != regConfirm_->text()) {
        QMessageBox::warning(this, "Sign up", "Passwords do not match. Please try again.");
        return;
    }
    QString error;
    if (!AuthService::registerUser(regName_->text(), regEmail_->text(), regPassword_->text(), &error)) {
        // AuthService provides internal error strings; always display English to the user.
        const bool validationError = error.startsWith(QStringLiteral("Ten >= 3 ky tu"));
        const QString message = validationError
            ? QStringLiteral("Use a username with 3–50 characters, a valid email address, and a password with at least 8 characters.")
            : QStringLiteral("Unable to create an account. The username or email may already be in use. Please check your details and try again.");
        QMessageBox::warning(this, "Sign up failed", message);
        return;
    }
    QMessageBox::information(this, "Success", "Your account has been created successfully. Please log in.");
    loginName_->setText(regName_->text().trimmed());
    resetForm();
}
