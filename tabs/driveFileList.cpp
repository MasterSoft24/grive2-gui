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


void AddSyncWizard::driveFileListActivate(){

     if(this->tabTreeActivated) return;

     this->tabSyncDataActivate();

     if(ui->syncPath->text()==""){
         showMessage("Error! No one account present.\nCreate account first.");
         return;
     }

    this->setTab(5);

    // QEventLoop loop; QTimer::singleShot(500, &loop, SLOT(quit())); loop.exec();

    disconnect(this,SLOT( treeFileItemChanged(QTreeWidgetItem*, int)));

    proc.start("grive2 -p "+ui->syncPath->text()+" -s");
    connect(&proc,SIGNAL(finished(int)),this,SLOT(driveFileListReady()));



}


void AddSyncWizard::driveFileListReActivate(){
    this->tabTreeActivated=false;
    driveFileListActivate();
}



void AddSyncWizard::driveFileListReady(){

    //qDebug()<<proc.readAllStandardOutput();

    QString buff=proc.readAllStandardOutput();

    QStringList list=buff.split("\n");

    QProcess p;
    p.start("cat "+ui->syncPath->text()+"/.exclude");
    p.waitForFinished(-1);

    QStringList excl=QString(p.readAllStandardOutput()).split("\n");



    QRegExp rx("/Мой диск/1c");

    QRegExpValidator v(rx, 0);

    int pos = 0;


    QTreeWidgetItem *topLevelItem = NULL;

    bool uncheck;

    foreach (const QString &fileName, list){

        if(fileName=="")continue;


        v.setRegExp(QRegExp (fileName));

        uncheck=false;

        for(int x=0;x<excl.size();x++){

                    QString z=excl.at(x);
                    if(z=="")continue;
                    QValidator::State st=v.validate(z, pos);
                    if(st!=QValidator::Invalid){
                        uncheck=true;
                        break;
                    }

        }


                    QStringList splitFileName = fileName.split("/");

                    // add root folder as top level item if treeWidget doesn't already have it
                    if (ui->treeFileList->findItems(splitFileName[0], Qt::MatchFixedString).isEmpty())
                    {
                        topLevelItem = new QTreeWidgetItem;
                        topLevelItem->setText(0, splitFileName[0]);//
//                        topLevelItem->setCheckState(0,Qt::Checked );
                        ui->treeFileList->addTopLevelItem(topLevelItem);
                    }

                    QTreeWidgetItem *parentItem = topLevelItem;

                                // iterate through non-root directories (file name comes after)
                                for (int i = 1; i < splitFileName.size() - 1; ++i)
                                {
                                    // iterate through children of parentItem to see if this directory exists
                                    bool thisDirectoryExists = false;
                                    for (int j = 0; j < parentItem->childCount(); ++j)
                                    {
                                        if (splitFileName[i] == parentItem->child(j)->text(0))
                                        {
                                            thisDirectoryExists = true;
                                            parentItem = parentItem->child(j);
                                            break;
                                        }
                                    }

                                    if (!thisDirectoryExists)
                                    {
                                        parentItem = new QTreeWidgetItem(parentItem);
                                        parentItem->setText(0, splitFileName[i]);
                                        if(!uncheck){
                                        parentItem->setCheckState(0,Qt::Checked );
                                        }
                                        else{
                                        parentItem->setCheckState(0,Qt::Unchecked );
                                        }
                                    }
                                }

                                QTreeWidgetItem *childItem = new QTreeWidgetItem(parentItem);
                                childItem->setText(0, splitFileName.last());
                                if(!uncheck){
                                childItem->setCheckState(0,Qt::Checked );
                                }
                                else{
                                childItem->setCheckState(0,Qt::Unchecked );
                                }
    }

ui->treeFileList->itemAt(0,0)->setText(0,ui->labelAccountName->text());// set root element text to account email

connect(ui->treeFileList,SIGNAL(itemChanged(QTreeWidgetItem*, int)),this,SLOT( treeFileItemChanged(QTreeWidgetItem*, int))  );

    this->tabTreeActivated=true;
    this->setTab(4);



}


void AddSyncWizard::recursiveChecks(QTreeWidgetItem* parent)
{
    Qt::CheckState checkState = parent->checkState(0);
    for(int i = 0; i < parent->childCount(); ++i)
    {
       parent->child(i)->setCheckState(0, checkState);
       recursiveChecks(parent->child(i));
    }

}


void AddSyncWizard::treeFileItemChanged(QTreeWidgetItem* item, int col){
recursiveChecks(item);
}

QString AddSyncWizard::restoreFullPath(QTreeWidgetItem* item){

    QString ret=item->text(0);
    QTreeWidgetItem*  p=0;

    while((p=item->parent())!=0){
        if(p->text(0)==ui->labelAccountName->text()){
            ret="/"+ret;
            break;
        }
        item=p;
        ret=p->text(0)+"/"+ret;
    }

    return ret+"\n";//
}


void AddSyncWizard::recursiveChecksSave(QTreeWidgetItem* item){

        if(item->childCount()>0){

//            if(item->checkState(0)==Qt::Unchecked){
//                //qDebug()<< restoreFullPath(item);
//                this->excludeFileContent+=restoreFullPath(item);
//            }

            for(int i=0;i<item->childCount();i++){

                if(item->checkState(0)==Qt::Unchecked){
                    //qDebug()<< restoreFullPath(item);
                    this->excludeFileContent+=restoreFullPath(item);
                }

                recursiveChecksSave(item->child(i));
            }

        }
        else{
            if(item->checkState(0)==Qt::Unchecked){
               // qDebug()<< restoreFullPath(item);
                this->excludeFileContent+=restoreFullPath(item);
            }
        }
}


void AddSyncWizard::saveTreeButton(){

    QTreeWidgetItem* ri=ui->treeFileList->itemAt(0,0);
    this->excludeFileContent="";
    recursiveChecksSave(ri->child(0));

    QFile f(ui->syncPath->text()+"/.exclude");
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&f);

    QStringList efc=this->excludeFileContent.split("\n");
    efc.removeDuplicates(); //Можно будет убрать если в recursiveChecksSave раскомментировать блок f(item->checkState(0)==Qt::Unchecked){
                                            // и удалить аналогичный блок ниже
    efc.removeAt(0);// убираем запись с именем корневой папки, потому что с ней вообще не синронизирует ничего


    //qDebug()<< efc.join("\n");


  //  out << this->excludeFileContent;
    out << efc.join("\n");
    f.close();

    QFile f1(ui->syncPath->text()+"/.grive_state");//лучше удалить файл состояния чтобы не косячил при изменении .exclude
    f1.remove();
    f1.close();

    showMessage("File list was successfuly saved");
}



