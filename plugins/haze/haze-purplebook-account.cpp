/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2016 Björn Bidar <theodorstormgrade@gmail.com>
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

#include "haze-purplebook-account.h"

#include "purplebook-options-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

HazePurplebookAccountUi::HazePurplebookAccountUi(QObject *parent)
 : AbstractAccountUi(parent)
{
    // Register supported parameters
    registerSupportedParameter(QLatin1String("account"), QVariant::String);
    registerSupportedParameter(QLatin1String("password"), QVariant::String);

}

HazePurplebookAccountUi::~HazePurplebookAccountUi()
{
}

AbstractAccountParametersWidget *HazePurplebookAccountUi::mainOptionsWidget(
        ParameterEditModel *model,
        QWidget *parent) const
{
    return new PurplebookOptionsWidget(model, parent);
}

bool HazePurplebookAccountUi::hasAdvancedOptionsWidget() const
{
    return false;
}
  
#include "haze-purplebook-account.moc"
