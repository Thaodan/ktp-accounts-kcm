/*
 * This file is part of telepathy-accounts-kcm-plugins
 *
 * Copyright (C) 2011 Florian Reinhard <florian.reinhard@googlemail.com>
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

#include "sunshine-account-ui-plugin.h"
#include "sunshine-account-ui.h"

#include <KCMTelepathyAccounts/PluginMacros>

#include <KDebug>
#include <KGenericFactory>

#include <QtCore/QVariantList>

SunshineAccountUiPlugin::SunshineAccountUiPlugin(QObject *parent, const QVariantList &)
    : AbstractAccountUiPlugin(parent)
{
    kDebug();

    // Register supported cm/protocol combinations
    registerProvidedProtocol("sunshine", "gadugadu");
}

SunshineAccountUiPlugin::~SunshineAccountUiPlugin()
{
    kDebug();
}

AbstractAccountUi* SunshineAccountUiPlugin::accountUi(const QString &connectionManager,
        const QString &protocol,
        const QString &serviceName)
{
    kDebug();

    // We support sunshine/gadugadu.
    if ((connectionManager == QLatin1String("sunshine")) &&
        (protocol == QLatin1String("gadugadu"))) {
        return new SunshineAccountUi;
    }

    return 0;
}

K_PLUGIN_FACTORY(factory, registerPlugin<SunshineAccountUiPlugin>();)
K_EXPORT_PLUGIN(factory("kcmtelepathyaccounts_plugin_sunshine"))

#include "sunshine-account-ui-plugin.moc"