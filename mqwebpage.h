/*
    grive2gui: an GPL program to sync a local directory with Google Drive with
    a grive2. This is GUI frontend for him.

    Copyright (C) 2014  Vladimir Kamensky

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation version 2
    of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef MQWEBPAGE_H
#define MQWEBPAGE_H

#include <QWebPage>

class MQWebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit MQWebPage(QWidget *parent = 0);

    bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type);

signals:

public slots:

};

#endif // MQWEBPAGE_H
