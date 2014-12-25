/*
 * Copyright 2014  Martin Klapetek <mklapetek@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "kaccounts-ui-provider.h"
#include <KCMTelepathyAccounts/ParameterEditModel>
#include <KCMTelepathyAccounts/account-edit-widget.h>

#include <TelepathyQt/Profile>
#include <TelepathyQt/ConnectionManager>
#include <TelepathyQt/AccountManager>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/PendingAccount>
#include <TelepathyQt/ProfileManager>

#include <KLocalizedString>

#include <QDBusConnection>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

class KAccountsUiProvider::Private
{
public:
    Tp::ConnectionManagerPtr connectionManager;
    Tp::ProfilePtr profile;

    AccountEditWidget *accountEditWidget;
    Tp::AccountManagerPtr accountManager;
    Tp::ProfileManagerPtr profileManager;
    QDialog *dialog;
    bool thingsReady;
    QString profileName;
};

KAccountsUiProvider::KAccountsUiProvider(QObject *parent)
    : KAccountsUiPlugin(parent),
      d(new Private)
{
    d->accountEditWidget = 0;

    Tp::registerTypes();

    // Start setting up the Telepathy AccountManager.
    Tp::AccountFactoryPtr  accountFactory = Tp::AccountFactory::create(QDBusConnection::sessionBus(),
                                                                       Tp::Features() << Tp::Account::FeatureCore
                                                                       << Tp::Account::FeatureCapabilities
                                                                       << Tp::Account::FeatureProtocolInfo
                                                                       << Tp::Account::FeatureProfile);

    d->accountManager = Tp::AccountManager::create(accountFactory);
    d->accountManager->becomeReady();

    d->profileManager = Tp::ProfileManager::create(QDBusConnection::sessionBus());
    Tp::PendingOperation *op = d->profileManager->becomeReady(Tp::Features() << Tp::ProfileManager::FeatureFakeProfiles);
    connect(op, SIGNAL(finished(Tp::PendingOperation*)), this, SLOT(onProfileManagerReady(Tp::PendingOperation*)));
}

KAccountsUiProvider::~KAccountsUiProvider()
{
    // tp managers are automatically ref-count-deleted

    delete d->accountEditWidget;
    delete d->dialog;
    delete d;
}

void KAccountsUiProvider::onProfileManagerReady(Tp::PendingOperation *op)
{
    if (op && op->isError()) {
        qWarning() << "Profile manager failed to get ready:" << op->errorMessage();
        return;
    }

    // If this was called after profile manager became ready and profile name is not yet known
    // OR if profile name was set and profile manager is not yet ready, return.
    // If profile name is set and this returns, it will get through this again when profile manager
    // becomes ready and vice-versa.
    if (d->profileName.isEmpty() || !d->profileManager->isReady(Tp::Features() << Tp::ProfileManager::FeatureFakeProfiles)) {
        return;
    }

    qDebug() << "Creating service for" << d->profileName;

    d->profile = d->profileManager->profileForService(d->profileName);

    d->connectionManager = Tp::ConnectionManager::create(d->profile->cmName());
    connect(d->connectionManager->becomeReady(), SIGNAL(finished(Tp::PendingOperation*)),
            this, SLOT(onConnectionManagerReady(Tp::PendingOperation*)));
}

void KAccountsUiProvider::setProviderName(const QString &providerName)
{
    d->profileName = providerName;

    onProfileManagerReady(0);
}

void KAccountsUiProvider::onConnectionManagerReady(Tp::PendingOperation*)
{
    Tp::ProtocolInfo protocolInfo = d->connectionManager->protocol(d->profile->protocolName());
    Tp::ProtocolParameterList parameters = protocolInfo.parameters();

    // Add the parameters to the model.
    ParameterEditModel *parameterModel = new ParameterEditModel(this);
    parameterModel->addItems(parameters, d->profile->parameters());

    // Delete account previous widget if it already existed.
    if (d->accountEditWidget) {
        d->accountEditWidget->deleteLater();
        d->accountEditWidget = 0;
    }

    d->dialog = new QDialog();
    QVBoxLayout *mainLayout = new QVBoxLayout(d->dialog);
    d->dialog->setLayout(mainLayout);

    // Set up the account edit widget.
    d->accountEditWidget = new AccountEditWidget(d->profile,
                                                 QString(),
                                                 parameterModel,
                                                 doConnectOnAdd,
                                                 0);

    QDialogButtonBox *dbb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, d->dialog);
    connect(dbb, SIGNAL(accepted()), this, SLOT(onDialogAccepted()));
    connect(dbb, SIGNAL(rejected()), this, SLOT(onDialogRejected()));

    mainLayout->addWidget(d->accountEditWidget);
    mainLayout->addWidget(dbb);

    connect(this,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)),
            d->accountEditWidget,
            SIGNAL(feedbackMessage(QString,QString,KMessageWidget::MessageType)));

    Q_EMIT uiReady();
}

void KAccountsUiProvider::showDialog()
{
    d->dialog->exec();
}

void KAccountsUiProvider::onDialogAccepted()
{
    qDebug();
    // Get the parameter values.
    QVariantMap values  = d->accountEditWidget->parametersSet();

    // Check all pages of parameters pass validation.
    if (!d->accountEditWidget->validateParameterValues()) {
        qDebug() << "A widget failed parameter validation. Not accepting wizard.";
        Q_EMIT feedbackMessage(i18n("Failed to create account"),
                                d->accountEditWidget->errorMessage(),
                                KMessageWidget::Error);
        return;
    }

    // FIXME: In some next version of tp-qt4 there should be a convenience class for this
    // https://bugs.freedesktop.org/show_bug.cgi?id=33153
    QVariantMap properties;

    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Service"))) {
        properties.insert(QLatin1String("org.freedesktop.Telepathy.Account.Service"), d->profile->serviceName());
    }
    if (d->accountManager->supportedAccountProperties().contains(QLatin1String("org.freedesktop.Telepathy.Account.Enabled"))) {
        properties.insert(QLatin1String("org.freedesktop.Telepathy.Account.Enabled"), true);
    }

    //remove password values from being sent. These are stored by Accounts SSO instead

    //FIXME: This is a hack for jabber registration, we don't remove passwords - see Telepathy ML thread "Storing passwords in MC and regsitering new accounts"
    //http://lists.freedesktop.org/archives/telepathy/2011-September/005747.html
    if (!values.contains(QLatin1String("register"))) {
        values.remove(QLatin1String("password"));
    }

    d->accountEditWidget->updateDisplayName();
    Tp::PendingAccount *pa = d->accountManager->createAccount(d->profile->cmName(),
                                                              d->profile->protocolName(),
                                                              d->accountEditWidget->displayName(),
                                                              values,
                                                              properties);

    connect(pa,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountCreated(Tp::PendingOperation*)));
}

void KAccountsUiProvider::onDialogRejected()
{
    d->dialog->reject();
}

void KAccountsUiProvider::onAccountCreated(Tp::PendingOperation *op)
{
    qDebug();
    if (op->isError()) {
        Q_EMIT feedbackMessage(i18n("Failed to create account"),
                                i18n("Possibly not all required fields are valid"),
                                KMessageWidget::Error);
        qWarning() << "Adding Account failed:" << op->errorName() << op->errorMessage();
        Q_EMIT error(op->errorMessage());
        return;
    }

    // Get the PendingAccount.
    Tp::PendingAccount *pendingAccount = qobject_cast<Tp::PendingAccount*>(op);
    if (!pendingAccount) {
        Q_EMIT feedbackMessage(i18n("Something went wrong with Telepathy"),
                                QString(),
                                KMessageWidget::Error);
        qWarning() << "Method called with wrong type.";
        Q_EMIT error(QStringLiteral("Eh"));
        return;
    }

    Tp::AccountPtr account = pendingAccount->account();
    account->setServiceName(d->profile->serviceName());
    if (d->accountEditWidget->connectOnAdd()) {
        account->setRequestedPresence(Tp::Presence::available());
    }

    QVariantMap values  = d->accountEditWidget->parametersSet();

    QVariantMap additionalData;

    additionalData.insert(QStringLiteral("uid"), account->objectPath());

    Q_EMIT success(values[QStringLiteral("account")].toString(), values[QStringLiteral("password")].toString(), additionalData);

    d->dialog->accept();
}