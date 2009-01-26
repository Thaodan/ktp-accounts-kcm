/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2009 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef TELEPATHY_ACCOUNTS_KCM_KCM_TELEPATHY_ACCOUNTS_H
#define TELEPATHY_ACCOUNTS_KCM_KCM_TELEPATHY_ACCOUNTS_H

#include "ui_main-widget.h"

#include <kcmodule.h>

class KCategorizedSortFilterProxyModel;

namespace Telepathy {
    namespace Client {
        class AccountManager;
        class PendingOperation;
    }
}

class KCMTelepathyAccounts : public KCModule, Ui::MainWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(KCMTelepathyAccounts);

public:
    explicit KCMTelepathyAccounts(QWidget *parent = 0,
                                  const QVariantList& args = QVariantList());
    ~KCMTelepathyAccounts();

public Q_SLOTS:
    void load();

private Q_SLOTS:
    void startAccountManager();
    void startAccountManagerFinished(Telepathy::Client::PendingOperation *op);
    void onAccountReady(Telepathy::Client::PendingOperation *op);

private:
    KCategorizedSortFilterProxyModel *m_accountsListProxyModel;
    Telepathy::Client::AccountManager *m_accountManager;

};

#endif // header guard

