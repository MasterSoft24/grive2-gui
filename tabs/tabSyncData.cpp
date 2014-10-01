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


void AddSyncWizard::tabSyncDataActivate(){

    if(this->tabActivated) return;

    // read config file and set form fields
     QSettings* sett=new QSettings(QDir::homePath()+"/.config/grive2gui.conf",QSettings::IniFormat);
     sett->setIniCodec("UTF-8");

     if(sett->value("SyncAccount").toString()!=""){

            ui->labelAccountName->setText(sett->value("SyncAccount").toString());
            AccountEmail=sett->value("SyncAccount").toString();
            ui->authButton->setVisible(false);
     }
     else{
            ui->labelAccountName->setText("No account present");
            ui->saveSettButton->setVisible(false);
            ui->removeSettButton->setVisible(false);
     }


    ui->syncPath->setText(sett->value("SyncDir").toString().toUtf8());


    if(sett->value("RemoteSyncPeriod").toString()==""){
        ui->scanPeriod->setValue(60);

    }
    else{
        ui->scanPeriod->setValue(sett->value("RemoteSyncPeriod").toInt());

    }


    if(sett->value("ManualSync").toString()=="true"){
            ui->manualSync->setCheckState(Qt::Checked);
    }
    else{
            ui->manualSync->setCheckState(Qt::Unchecked);
    }

    this->tabActivated=true;
}


void AddSyncWizard::selDir(){

    ui->syncPath->setText( QFileDialog::getExistingDirectory(this, tr("Select Directory"),QDir::homePath(),QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks));

}

void AddSyncWizard::startAuth(){

    ui->authButton->setEnabled(false);

    // phase #1

    QString scope="https://www.googleapis.com/auth/userinfo.email+https://www.googleapis.com/auth/userinfo.profile+https://docs.google.com/feeds/+https://docs.googleusercontent.com/+https://spreadsheets.google.com/feeds/";
    QString redirectUri="urn:ietf:wg:oauth:2.0:oob";//
    QString clientId="502802353894-fjbma0deq577lug7hrui8ma3ogv03se2.apps.googleusercontent.com";
    QString state="1";
    bool approvalPrompt=true;
    bool accessType=false;

    QUrl url("https://accounts.google.ru/o/oauth2/auth?");

    QUrlQuery query;
    query.addQueryItem(("scope"), scope);
    query.addQueryItem(("redirect_uri"), redirectUri);

    query.addQueryItem(("response_type"), QString("code"));
    query.addQueryItem(("client_id"), clientId);
    query.addQueryItem(("access_type"), accessType ? QString("online") : QString("offline"));
    query.addQueryItem(("approval_prompt"), approvalPrompt ? QString("force") : QString("auto"));
    query.addQueryItem(("state"), state);

    url.setQuery(query);

    QNetworkRequest request(url);


    this->setTab(2);

    // test <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    ui->webView->setPage(new MQWebPage());
    connect( ui->webView->page(), SIGNAL(linkClicked(const QUrl&)),this, SLOT(pageChange(const QUrl&)));



    ui->webView->load(url);

    reply=ui->webView->page()->networkAccessManager()->get(request);

    connect(ui->webView,SIGNAL(loadFinished(bool)),this,SLOT(pageLoaded(bool)));

}

void AddSyncWizard::pageLoaded(bool r){

    QString currentUrl=ui->webView->url().toString();

    if(currentUrl.indexOf("https://accounts.google.com/o/oauth2/approval")==0){

        // phase #2

        this->setTab(1);

        // получили требуемую строку для получения токена
        QWebElement e = ui->webView->page()->mainFrame()->findFirstElement("input#code");
        //qDebug() << e.attribute("value");


        QString postData = "client_id=";
        postData += QUrl::toPercentEncoding("502802353894-fjbma0deq577lug7hrui8ma3ogv03se2.apps.googleusercontent.com");
        postData += "&client_secret=";
        postData += QUrl::toPercentEncoding("HMQXlR2HDhrw58KR5lDQYKea");
        postData += "&code=";
        postData += QUrl::toPercentEncoding(e.attribute("value").trimmed());
        postData += "&grant_type=authorization_code";
        postData += "&redirect_uri=urn:ietf:wg:oauth:2.0:oob";//


        QUrl urlt("https://accounts.google.com/o/oauth2/token");

        QNetworkAccessManager* networkManager= new QNetworkAccessManager(0);

        QNetworkRequest request;
        request.setUrl(urlt);
        QByteArray ba;
        ba+=postData;



        reply = networkManager->post(request, ba);

        QEventLoop loop;
        connect(reply, SIGNAL(finished()),&loop, SLOT(quit()));
        loop.exec();

        QString content= reply->readAll();


        QJsonDocument json = QJsonDocument::fromJson(content.toUtf8());
        QJsonObject job = json.object();
        QString v=job["refresh_token"].toString();

        if((v!="")&&(ui->syncPath->text()!="")){
            QString conf="SyncDir=";
            conf+=ui->syncPath->text()+"\n";
            conf+="RemoteSyncPeriod="+ui->scanPeriod->text()+"\n";
            conf+="SyncAccount="+AccountEmail+"\n";
            conf+="ManualSync=";

            if(ui->manualSync->checkState()==Qt::Checked){
                conf+="true\n";
            }
            else{
                conf+="false\n";
            }

            // store config to file
            QFile cfg(QDir::homePath()+"/.config/grive2gui.conf");
            cfg.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out(&cfg);
            out << conf;
            cfg.close();

            // store grive key to file
            QFile key(ui->syncPath->text()+"/.grive");
            key.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream outk(&key);
            outk << "{\"refresh_token\" : \""+v+"\"}";
            key.close();

           int r= showQuestion("Are you want to set the list of files for sync?");

               switch(r){

               case 0:// no

                    showMessage("Google drive account was successfuly added");

                                break;
               case 1:// yes

                                break;

               }

                this->close();
        }
        else{

            qDebug()<<"Google auth failure";
            ui->authButton->setEnabled(true);

            showMessage("Create account failure",1);

        }




    }



}

void AddSyncWizard::pageChange(QUrl url){


    QString currentUrl=url.toString();

    if(currentUrl.indexOf("https://accounts.google.com/ServiceLoginAuth")==0){

        QWebElement e = ui->webView->page()->mainFrame()->findFirstElement("#Email");
        AccountEmail= e.evaluateJavaScript("this.value").toString();
        //qDebug() << e.evaluateJavaScript("this.value").toString();


    }

}

void AddSyncWizard::chManualSync(int state){

    if(state==2){
        ui->scanPeriod->setEnabled(false);
    }
    else{
        ui->scanPeriod->setEnabled(true);
    }
}


void AddSyncWizard::saveSettButton(){

    QString conf="SyncDir=";
    conf+=ui->syncPath->text()+"\n";
    conf+="RemoteSyncPeriod="+ui->scanPeriod->text()+"\n";
    conf+="SyncAccount="+AccountEmail+"\n";
    conf+="ManualSync=";

    if(ui->manualSync->checkState()==Qt::Checked){
        conf+="true\n";
    }
    else{
        conf+="false\n";
    }

    // store config to file
    QFile cfg(QDir::homePath()+"/.config/grive2gui.conf");
    cfg.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&cfg);
    out << conf;
    cfg.close();

}


void AddSyncWizard::removeSettButton(){

    int r=showQuestion("Are you shure what you want remove this account?");
    if(r==0){// not sure
        setTab(1);
    }
    else{//sure

        // remove account
        QFile f(QDir::homePath()+"/.config/grive2gui.conf");
        f.remove();
        QFile f1(ui->syncPath->text()+"/.grive");
        f1.remove();

        ui->labelAccountName->setText("No account present");
        ui->syncPath->setText("");
        ui->manualSync->setCheckState(Qt::Unchecked);
        ui->scanPeriod->setValue(60);
        ui->saveSettButton->setVisible(false);
        ui->removeSettButton->setVisible(false);
         ui->authButton->setVisible(true);

        showMessage("Account was successfully removed");
    }
}













