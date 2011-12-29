/*
 * This file is part of telepathy-accounts-kcm
 *
 * Copyright (C) 2011 Dominik Schmidt <kde@dominik-schmidt.de>
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

#include "icq-server-settings-widget.h"

#include <KDebug>

IcqServerSettingsWidget::IcqServerSettingsWidget(ParameterEditModel *model,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(model, parent)
{
    kDebug();

    // Set up the UI.
    m_ui = new Ui::IcqServerSettingsWidget;
    m_ui->setupUi(this);

    handleParameter("server", QVariant::String, m_ui->serverLineEdit, m_ui->serverLabel);
    handleParameter("port", QVariant::Int, m_ui->portSpinBox, m_ui->portLabel);
    handleParameter("use-ssl", QVariant::Bool, m_ui->useSslCheckBox, 0);
    handleParameter("allow-multiple-logins", QVariant::Bool, m_ui->allowMultipleLoginsCheckBox, 0);
    handleParameter("always-use-rv-proxy", QVariant::Bool, m_ui->alwaysUseRvProxyCheckBox, 0);
    handleParameter("charset", QVariant::String, m_ui->charsetComboBox, m_ui->charsetLabel);
}

IcqServerSettingsWidget::~IcqServerSettingsWidget()
{
    kDebug();

    delete m_ui;
}

#include "icq-server-settings-widget.moc"
