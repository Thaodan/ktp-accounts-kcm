/*
 * This file is part of telepathy-accounts-kcm
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

#include "skype-advanced-settings-widget.h"

#include <KDebug>

SkypeAdvancedSettingsWidget::SkypeAdvancedSettingsWidget(ParameterEditModel *model, QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    kDebug();

    // Set up the UI.
    m_ui = new Ui::SkypeAdvancedSettingsWidget;
    m_ui->setupUi(this);

    //server
    handleParameter("skypeout-online",  QVariant::Bool, m_ui->skypeOutCheckBox, 0);
    handleParameter("skype-sync",       QVariant::Bool, m_ui->syncCheckBox, 0);
    handleParameter("check-for-updates",QVariant::Bool, m_ui->updatesCheckBox, 0);
    handleParameter("reject-all-auths", QVariant::Bool, m_ui->authRequestsCheckBox, 0);
    handleParameter("skype-autostart",  QVariant::Bool, m_ui->autostartCheckBox, 0);
}

SkypeAdvancedSettingsWidget::~SkypeAdvancedSettingsWidget()
{
    kDebug();

    delete m_ui;
}