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
#include "addsyncwizard.h"
#include "ui_AddSyncWizard.h"


#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>

#include <QUrl>
#include <QUrlQuery>
#include <QList>
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
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QTabBar>
#include <QMovie>
#include <QDesktopWidget>

#include "tabs/driveFileList.cpp"
#include "tabs/tabSyncData.cpp"


AddSyncWizard::AddSyncWizard(QWidget *parent,int tabNum) :
    QWizardPage(parent),
    ui(new Ui::AddSyncWizard)
{
    ui->setupUi(this);

    this->setWindowFlags( Qt::Window
                          | Qt::WindowCloseButtonHint);

    connect(ui->folderSelButton,SIGNAL(clicked()),this,SLOT(selDir()));
    connect(ui->authButton,SIGNAL(clicked()),this,SLOT(startAuth()));
    connect(ui->manualSync,SIGNAL(stateChanged(int)),this,SLOT(chManualSync(int)));
    connect(ui->removeSettButton,SIGNAL(clicked()),this,SLOT(removeSettButton()));
    connect(ui->saveSettButton,SIGNAL(clicked()),this,SLOT(saveSettButton()));
    connect(ui->saveTreeButton,SIGNAL(clicked()),this,SLOT(saveTreeButton()));


    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabController(int)));


    QTabBar* tabBar;
    ui->tabWidget->tabBar()->hide();

    ui->tabWidget->setCurrentIndex(5);

    QMovie* movProgress=new QMovie(":/files/Progress.gif");
    ui->progressLabel->setMovie(movProgress);
    movProgress->start();

    this->tabActivated=false;
    this->tabTreeActivated=false;

    this->moveWindowToCenter();
}

AddSyncWizard::~AddSyncWizard()
{
    delete ui;
}


void AddSyncWizard::moveWindowToCenter()
{
    QRect frect = frameGeometry();
    setFixedSize(frect.width(),QDesktopWidget().height()*0.85);
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());
}

void AddSyncWizard::setTab(int tab,bool reAcrivate){

    if(reAcrivate){
            switch(tab){

                case 0:

                    break;
                case 1:
                    //tabSyncDataActivate();
                    break;
                case 2:

                    break;
                case 3:

                    break;
                case 4:
                    driveFileListReActivate();
                    break;
                case 5:

                    break;
            }
    }
    ui->tabWidget->setCurrentIndex(tab);

}

void AddSyncWizard::showMessage(QString msg,int retTab){

    connect(ui->msgButtonOK,SIGNAL(clicked()),this,SLOT(msgOKClick()));
    this->msgRetTab=retTab;
    ui->messageText->setText(msg);
    this->setTab(0);
}

void AddSyncWizard::showMessage(QString msg){

    connect(ui->msgButtonOK,SIGNAL(clicked()),this,SLOT(msgOKClickClose()));
    ui->messageText->setText(msg);
    this->setTab(0);
}


int AddSyncWizard::showQuestion(QString msg){

    //this->msgRetTab=retTab;
    ui->questText->setText(msg);
    this->questRetVal=0;
    this->setTab(3);

    connect(ui->questButtonCancel,SIGNAL(clicked()),this,SLOT(questCancel()));

    QEventLoop loop;
    connect(ui->questButtonOK,SIGNAL(clicked()),&loop,SLOT(quit()));
    loop.exec();

    if(this->questRetVal!=-1)this->questRetVal=1;
    else this->questRetVal=0
            ;
    return this->questRetVal;
}


void AddSyncWizard::msgOKClick(){

    setTab(this->msgRetTab);
    disconnect(this,SLOT(msgOKClick()));
}

void AddSyncWizard::msgOKClickClose(){

    disconnect(this,SLOT(msgOKClickClose()));
    this->close();

}

void AddSyncWizard::questCancel(){

    this->questRetVal=-1;
    ui->questButtonOK->click();
}


void AddSyncWizard::tabController(int n){// n= target tab number

    switch(n){

        case 0:

            break;
        case 1:
            tabSyncDataActivate();
            break;
        case 2:

            break;
        case 3:

            break;
        case 4:
            driveFileListActivate();
            break;
        case 5:

            break;
    }
}

