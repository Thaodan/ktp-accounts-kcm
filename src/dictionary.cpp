/*
 * This file is part of Kopete
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

#include "dictionary.h"

#include <KDebug>
#include <KLocale>

Dictionary* Dictionary::s_self = 0;

Dictionary::Dictionary()
{
    kDebug();

    // Set up the singleton instance
    s_self = this;

    // TODO: Populate the dictionary
}

Dictionary::~Dictionary()
{
    kDebug();

    // Delete the singleton instance of this class
    s_self = 0;
}

Dictionary *Dictionary::instance()
{
    kDebug();

    // Construct the singleton if hasn't been already
    if (!s_self) {
        s_self = new Dictionary;
    }

    // Return the singleton instance of this class
    return s_self;
}

QString Dictionary::string(const QString &key) const
{
    return m_strings.value(key);
}
