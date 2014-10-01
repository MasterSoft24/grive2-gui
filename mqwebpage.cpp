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
#include "mqwebpage.h"
#include <QDebug>
#include <QNetworkRequest>

MQWebPage::MQWebPage(QWidget *parent) :
    QWebPage(parent)
{
}


bool MQWebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, QWebPage::NavigationType type){

        if (type==QWebPage::NavigationTypeFormSubmitted){
            if(  request.url().toString().indexOf("https://accounts.google.ru/accounts/SetSID?ssdc")==0){//https://accounts.google.ru/accounts/SetSID?ssdc
                this->linkClicked(QUrl("https://accounts.google.com/ServiceLoginAuth"));
            }

        }

}
