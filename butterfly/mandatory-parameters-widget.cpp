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

#include "mandatory-parameters-widget.h"

#include "ui_mandatory-parameters-widget.h"

#include <KDebug>
#include <KMessageBox>

class MandatoryParametersWidget::Private
{
public:
    Private()
            : accountParameter(0),
              passwordParameter(0),
              ui(0)
    {
        kDebug();
    }

    Tp::ProtocolParameterList parameters;
    Tp::ProtocolParameter *accountParameter;
    Tp::ProtocolParameter *passwordParameter;

    Ui::MandatoryParametersWidget *ui;
};

MandatoryParametersWidget::MandatoryParametersWidget(Tp::ProtocolParameterList parameters,
                                                     const QVariantMap &values,
                                                     QWidget *parent)
 : AbstractAccountParametersWidget(parameters, values, parent),
   d(new Private)
{
    kDebug();

    // Save the parameters.
    d->parameters = parameters;

    // Store the parameters this widget supports
    foreach (Tp::ProtocolParameter *parameter, d->parameters) {
      
        qDebug() << parameter->name();
      
      
        if ((parameter->name() == "account") && (parameter->type() == QVariant::String)) {
            if (!d->accountParameter) {
                d->accountParameter = parameter;
            }
        } else if ((parameter->name() == "password") && (parameter->type() == QVariant::String)) {
            if (!d->passwordParameter) {
                d->passwordParameter = parameter;
            }
        }
    }

    // Set up the UI.
    d->ui = new Ui::MandatoryParametersWidget;
    d->ui->setupUi(this);

   // Prefill UI elements if appropriate.
   if (d->accountParameter) {
       if (values.contains(d->accountParameter->name())) {
           d->ui->accountLineEdit->setText(values.value(d->accountParameter->name()).toString());
       } else {
           d->ui->accountLineEdit->setText(d->accountParameter->defaultValue().toString());
       }
   }

   if (d->passwordParameter) {
       if (values.contains(d->passwordParameter->name())) {
           d->ui->passwordLineEdit->setText(values.value(d->passwordParameter->name()).toString());
       } else {
           d->ui->passwordLineEdit->setText(d->passwordParameter->defaultValue().toString());
       }
   }

//    Hide any elements we don't have the parameters passed to show.
   if (!d->accountParameter) {
       d->ui->accountLabel->hide();
       d->ui->accountLineEdit->hide();
   }

   if (!d->passwordParameter) {
       d->ui->passwordLabel->hide();
       d->ui->passwordLineEdit->hide();
   }
}

MandatoryParametersWidget::~MandatoryParametersWidget()
{
    kDebug();

    delete d;
}

QMap<Tp::ProtocolParameter*, QVariant> MandatoryParametersWidget::parameterValues() const
{
    kDebug();

    QMap<Tp::ProtocolParameter*, QVariant> parameters;

   // Populate the map of parameters and their values with all the parameters this widget contains.
   if (d->accountParameter) {
       parameters.insert(d->accountParameter, d->ui->accountLineEdit->text());
   }

   if (d->passwordParameter) {
       parameters.insert(d->passwordParameter, d->ui->passwordLineEdit->text());
   }

   return parameters;
}

bool MandatoryParametersWidget::validateParameterValues()
{
    kDebug();


    return true;
}


#include "mandatory-parameters-widget.moc"

