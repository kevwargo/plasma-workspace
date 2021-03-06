/*
 *   Copyright (C) 2017 David Edmundson <davidedmundson@kde.org>
 *
 * This program is free software you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/

#ifndef NOTIFICATIONSANITIZER_H
#define NOTIFICATIONSANITIZER_H

#include <QString>

namespace NotificationSanitizer
{
    /*
     * This turns generic random text of either plain text of any degree of faux-HTML into HTML allowed
     * in the notification spec namely:
     * a, img, b, i, u, and br
     * In addition to the notification spec the following tags are allowed:
     * table, tr, and td
     * All other tags and attributes are stripped
     * Whitespace is stripped and converted to <br/>
     * Double newlines are compressed
     *
     * Image src is only copied when referring to a local file
     */
    QString parse(const QString &in);
}

#endif
