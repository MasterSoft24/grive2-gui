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
#ifndef ADDSYNCWIZARD_H
#define ADDSYNCWIZARD_H

#include <QWizardPage>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QList>
#include <QSslConfiguration>
#include <QWebSettings>
#include <QNetworkCookieJar>
#include <QList>
#include <QNetworkCookie>
#include <QWebFrame>
#include <QWebElement>
#include <QEventLoop>
#include <QWebView>
#include <QFile>
#include <QSettings>
#include <QTreeWidget>
#include <QProcess>
#include <QTimer>
#include  <QStringList>
#include <qvalidator.h>
#include <QRegExpValidator>
#include <QValidator>
#include <QRegExp>
#include <QTextCodec>

#include "mqwebview.h"


namespace Ui {
class AddSyncWizard;
}

class AddSyncWizard : public QWizardPage
{
    Q_OBJECT

public:
    explicit AddSyncWizard(QWidget *parent = 0, int tabNum=0);
    ~AddSyncWizard();

    void setTab(int tab, bool reAcrivate=false);
    void showMessage(QString msg,int retTab);
    void showMessage(QString msg); //close after ok pressed
    int showQuestion(QString msg);

    QString AccountEmail;
    QProcess proc;
    QString excludeFileContent;

private:
    Ui::AddSyncWizard *ui;

    int msgRetTab;
    int questRetVal;



    // tabSyncData <--------------------
    QNetworkReply* reply;
    bool tabActivated;

      void tabSyncDataActivate();

    //tabWeb <--------------------
    MQWebView* webView;


     bool tabTreeActivated;


    // driveFileList <--------------------
    void driveFileListActivate();
    void driveFileListReActivate();

    void recursiveChecks(QTreeWidgetItem* parent);
    void recursiveChecksSave(QTreeWidgetItem* item);
    QString restoreFullPath(QTreeWidgetItem* item);

private slots:

    void msgOKClick();
    void msgOKClickClose();
    void questCancel();
    void tabController(int n);

    // tabSyncData <--------------------
    void selDir();
    void startAuth();
    void pageLoaded(bool r);
    void pageChange(QUrl url);
    void chManualSync(int state);
    void saveSettButton();
    void removeSettButton();

    // driveFileList <--------------------
    void saveTreeButton();
    void driveFileListReady();
    void treeFileItemChanged(QTreeWidgetItem* item, int col);


};

#endif // ADDSYNCWIZARD_H
