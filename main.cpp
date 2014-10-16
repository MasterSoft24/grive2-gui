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
#include "mainwindow.h"
#include <QApplication>
#include "addsyncwizard.h"
#include <QTextCodec>
#include <QString>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    MainWindow w;
  //  w.show();

    AddSyncWizard* sw=new AddSyncWizard();


    QString p=argv[1];

p="filelist";
p="account";

    if(p==""){
        qDebug()<< "Grive2 GUI v0.1 (grive2 v0.1 required)";
        qDebug()<< "Use <account> or <filelist> command" ;
        qDebug()<< "USAGE: grive2-gui <command>";
        return 0;
    }

    if(p=="account"){
        sw->setTab(1);
        sw->show();
    }
    else if(p=="filelist"){
        sw->setTab(4);
        sw->show();
    }
    else{
        qDebug()<< p+" is unknown command";
        return 0;
    }


    return a.exec();
}
