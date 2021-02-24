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
 * @file datasettingsframe.cpp
 *
 * @brief setup data settings frame UI
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include "datasettingsframe.h"

DataSettingsFrame::DataSettingsFrame(QWidget *parent) : QMainWindow(parent)
{
    this->center = new QWidget();
    this->dataBlockLay = new QVBoxLayout();
    this->bottomToolBarLay = new QVBoxLayout();
    this->mainLay = new QVBoxLayout();
    this->dataSettingsBottomToolBar = new DataSettingsBottomToolBar();

    this->dataSettingsThread = nullptr;

    this->setupUI();
}

DataSettingsFrame::~DataSettingsFrame()
{
    this->dataBlockLay->deleteLater();
    this->dataBlockLay = nullptr;

    this->bottomToolBarLay->deleteLater();
    this->bottomToolBarLay = nullptr;

    this->mainLay->deleteLater();
    this->mainLay = nullptr;

    this->center->deleteLater();
    this->center = nullptr;

    if(dataBlocks.size() != 0) deleteDataBlockWidget();

    if (dataSettingsThread != nullptr)
    {
        this->dataSettingsThread->deleteLater();
        this->dataSettingsThread = nullptr;
    }

    this->dataSettingsBottomToolBar->deleteLater();
    this->dataSettingsBottomToolBar = nullptr;
}

void DataSettingsFrame::setupUI()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:#2C2A38;");

    this->center->setLayout(this->mainLay);
    this->setCentralWidget(this->center);

    this->mainLay->setSpacing(0);
    this->mainLay->setContentsMargins(0, 0, 0, 0);

    this->dataBlockLay->setAlignment(Qt::AlignTop);
    this->dataBlockLay->setSpacing(2);
    this->dataBlockLay->setContentsMargins(2, 2, 2, 2);

    this->bottomToolBarLay->setAlignment(Qt::AlignBottom);
    this->bottomToolBarLay->setSpacing(0);
    this->bottomToolBarLay->setContentsMargins(0, 0, 0, 0);

    addDataBlockWidget();

    this->bottomToolBarLay->addWidget(this->dataSettingsBottomToolBar);

    this->mainLay->addLayout(this->dataBlockLay);
    this->mainLay->addLayout(this->bottomToolBarLay);
}

void DataSettingsFrame::addDataBlockWidget()
{
    int index = 0;
    QMultiMap<int, int>::const_iterator i;
    for (i = global_editableItems.constBegin(); i != global_editableItems.constEnd(); ++i)
    {
        QString itemName = global_checkListItems.value(i.key()).at(i.value()).itemName;
        QString statusTemp = global_checkListItems.value(i.key()).at(i.value()).status;

        int leftBound = statusTemp.indexOf("{") + 1;
        int rightBound = statusTemp.indexOf("}");
        statusTemp = statusTemp.mid(leftBound, (rightBound - leftBound));

        dataBlocks.append(new DataBlock(index, itemName, statusTemp));
        connect(dataBlocks.at(index), SIGNAL(mousePressed_signal(int)), this, SLOT(dataBlockPressed(int)));
        this->dataBlockLay->addWidget(dataBlocks.at(index));
        index++;
    }
}

void DataSettingsFrame::deleteDataBlockWidget()
{
    for (int i = 0; i < dataBlocks.size(); i++)
    {
        dataBlocks.at(i)->close();
        dataBlocks.at(i)->deleteLater();
        dataBlocks[i] = nullptr;
    }
    dataBlocks.clear();
}

void DataSettingsFrame::dataBlockPressed(const int &dataId)
{
    QString userDataString = this->dataSettingsBottomToolBar->getUserDataFromLE();;
    if (userDataString == "")
    {
        // If QLineEdit is empty, then insert a blank string into data base
        userDataString = "_________";
    }

    dataBlocks.at(dataId)->setUserData(userDataString);

    this->dataSettingsThread = new DataSettingsThread(dataId, userDataString);
    dataSettingsThread->start();

    connect(this->dataSettingsThread, &DataSettingsThread::dataSettingsThreadFinished, this, [this](void)
    {
        this->dataSettingsBottomToolBar->cleanLineEdit();
    });
}


/* DataSettingsThread class */
DataSettingsThread::DataSettingsThread(const int &dataId, const QString &userDataString)
    : m_dataId(dataId), m_userDataString(userDataString)
{

}

void DataSettingsThread::run()
{
    int index = 0;
    QMultiMap<int, int>::const_iterator i;
    for (i = global_editableItems.constBegin(); i != global_editableItems.constEnd(); ++i)
    {
        if (index == m_dataId)
        {
            QString statusTemp = global_checkListItems.value(i.key()).at(i.value()).status;
            int leftBound = statusTemp.indexOf("{") + 1;
            int rightBound = statusTemp.indexOf("}");
            global_checkListItems[i.key()][i.value()].status.replace(leftBound, (rightBound-leftBound), m_userDataString);
        }
        index++;
    }
    emit this->dataSettingsThreadFinished();
}
