/*
* Airbus A320NEO CheckList For MSFS2020
*
* This file is part of Airbus A320NEO CheckList Project
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Airbus A320NEO CheckList For MSFS2020.
* If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file main.cpp
 *
 * @brief Main Entry Point + verify checklist template file
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include "mainwindow.h"
#include "globalvars.h"

#include <QApplication>
#include <QDebug>

QMap<int, QVector<QCheckList::QCLItem>> global_checkListItems;
QMultiMap<int, int> global_editableItems;

bool initCheck()
{
    const QString fileName = "cl_v4.xml";
    const QString cmd = "certutil -hashfile cl_v4.xml md5";  // get md5 hash value using CMD command
    QProcess process;
    process.start(cmd);
    process.waitForFinished();
    QByteArray qbt = process.readAllStandardOutput();
    QString msg = QString::fromLocal8Bit(qbt);
    if (!msg.contains("288077660676495bd06c095f667b123b"))
    {
        return false;
    }

    MxmlParser mxmlParser;
    if (mxmlParser.readFile(fileName) == 1)
    {
        global_checkListItems = mxmlParser.getCheckListDetails();
        global_editableItems = mxmlParser.getEditableItems();
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!initCheck())
    {
        return 1;
    }

    MainWindow mainWindow;
    mainWindow.setWindowIcon(QIcon("./icons/win_icon.ico"));
    mainWindow.show();

    return a.exec();
}
