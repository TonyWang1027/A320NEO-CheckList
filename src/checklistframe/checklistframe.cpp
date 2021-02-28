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
 * @file checklistframe.cpp
 *
 * @brief setup checklist frame UI
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include <QDebug>

#include "checklistframe.h"

// Creating and initializing CheckListFrameThread's static data member
CheckListFrameThread *CheckListFrameThread::s_checkListFrameThread = nullptr;
SimConnectThread *SimConnectThread::s_simConnectThread = nullptr;

CheckListFrame::CheckListFrame(QWidget *parent) : QMainWindow(parent)
{
    this->center = new QWidget();
    this->mainLay = new QVBoxLayout();
    this->checkListItemLay = new QVBoxLayout();
    this->bottomToolBarLay = new QVBoxLayout();
    this->checkListBottomToolBar = new CheckListBottomToolBar();

    m_stageNum = 0;

    this->setupUI();
    this->events();
}

CheckListFrame::~CheckListFrame()
{
    this->checkListItemLay->deleteLater();
    this->checkListItemLay = nullptr;

    this->bottomToolBarLay->deleteLater();
    this->bottomToolBarLay = nullptr;

    this->mainLay->deleteLater();
    this->mainLay = nullptr;

    this->center->deleteLater();
    this->center = nullptr;

    CheckListFrameThread::deleteInstance();

    if(checkListItems.size() != 0) delCheckListItemWidgets();

    this->checkListBottomToolBar->deleteLater();
    this->checkListBottomToolBar = nullptr;
}

void CheckListFrame::setupUI()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:#2C2A38;");

    this->center->setLayout(this->mainLay);
    this->setCentralWidget(this->center);

    this->mainLay->setSpacing(0);
    this->mainLay->setContentsMargins(0, 0, 0, 0);

    this->checkListItemLay->setAlignment(Qt::AlignTop);
    this->checkListItemLay->setSpacing(5);
    this->checkListItemLay->setContentsMargins(5, 5, 5, 5);

    this->bottomToolBarLay->setAlignment(Qt::AlignBottom);
    this->bottomToolBarLay->setSpacing(0);
    this->bottomToolBarLay->setContentsMargins(0, 0, 5, 0);

    this->mainLay->addLayout(this->checkListItemLay);
    this->mainLay->addLayout(this->bottomToolBarLay);
}

void CheckListFrame::events()
{
    connect(this->checkListBottomToolBar, SIGNAL(prevButton_onClicked_signal()), this, SLOT(prevButton_onClicked()));
    connect(this->checkListBottomToolBar, SIGNAL(nextButton_onClicked_signal()), this, SLOT(nextButton_onClicked()));
    connect(this->checkListBottomToolBar, SIGNAL(checkAllButton_onClicked_signal(bool)), this, SLOT(checkAllButton_onClicked(bool)));
}

void CheckListFrame::prevButton_onClicked()
{
    // Refrash check list frame
    if (m_stageNum > 0)
    {
        delCheckListItemWidgets();
        m_stageNum--;
        addCheckListItemWidgets(false);
        checkAllItemStatus();
        SimConnectThread::getInstance()->resetSurveillandStage(m_stageNum);
        emit this->prevPGButton_onClicked_signal();
    }
}

void CheckListFrame::nextButton_onClicked()
{
    // Refrash check list frame
    if (m_stageNum < (global_checkListItems.size() - 1))
    {
        delCheckListItemWidgets();
        m_stageNum++;
        addCheckListItemWidgets(false);
        checkAllItemStatus();
        SimConnectThread::getInstance()->resetSurveillandStage(m_stageNum);
        emit this->nextPGButton_onClicked_signal();
    }
}

void CheckListFrame::checkAllButton_onClicked(const bool &status)
{
    CheckListFrameThread::getInstance()->disconnect();  // Avoid duplicate connections with the same signal
    CheckListFrameThread::getInstance()->setProcessId(1);
    CheckListFrameThread::getInstance()->setStageNum(m_stageNum);
    CheckListFrameThread::getInstance()->setStatus(status);
    CheckListFrameThread::getInstance()->start();

    connect(CheckListFrameThread::getInstance(), &CheckListFrameThread::threadFinished_ID_TWO, this, [this]()
    {
        delCheckListItemWidgets();
        addCheckListItemWidgets(false);
    });
}

void CheckListFrame::addCheckListItemWidgets(bool addBottomToolBar)
{
    for (int i = 0; i < global_checkListItems.value(m_stageNum).size(); i++)
    {
        uint8_t editable = global_checkListItems.value(m_stageNum).at(i).editable;
        checkListItems.append(new CheckListItem(editable, &(global_checkListItems[m_stageNum][i].isChecked)));

        checkListItems.at(i)->setContentLabel(global_checkListItems.value(m_stageNum).at(i).itemName);
        QString statusTemp = global_checkListItems.value(m_stageNum).at(i).status;
        checkListItems.at(i)->setStatusLabel(statusTemp.remove("{").remove("}"));
        connect(checkListItems.at(i), SIGNAL(itemStatusChanged_signal()), this, SLOT(checkAllItemStatus()));
        this->checkListItemLay->addWidget(checkListItems.at(i));
    }

    if (addBottomToolBar)
    {
        checkAllItemStatus();
        this->bottomToolBarLay->addWidget(this->checkListBottomToolBar);
    }
}

void CheckListFrame::delCheckListItemWidgets()
{
    for (int i = 0; i < global_checkListItems.value(m_stageNum).size(); i++)
    {
        checkListItems.at(i)->close();
        checkListItems.at(i)->deleteLater();
        checkListItems[i] = nullptr;
    }
    checkListItems.clear();
}

void CheckListFrame::setCheckListStage(int &t_stageNum)
{
    m_stageNum = t_stageNum;
}

void CheckListFrame::checkAllItemStatus()
{
    CheckListFrameThread::getInstance()->disconnect();  // Avoid duplicate connections with the same signal
    CheckListFrameThread::getInstance()->setProcessId(0);
    CheckListFrameThread::getInstance()->setStageNum(m_stageNum);
    CheckListFrameThread::getInstance()->start();

    connect(CheckListFrameThread::getInstance(), &CheckListFrameThread::threadFinished_ID_ONE, this, [this](bool allChecked)
    {
        this->checkListBottomToolBar->setCheckAllButtonStatus(allChecked);
    });
}

void CheckListFrame::startDataRequestFromSim()
{
    // simConnectThread = new SimConnectThread(m_stageNum);
    SimConnectThread::getInstance()->setStageNum(m_stageNum);
    lockForWrite();
    SimDataCollector::getInstance()->setTerminateDataRequests(false);
    unlock();

    qRegisterMetaType<uint32_t>("uint32_t");
    connect(SimConnectThread::getInstance(), &SimConnectThread::dataCollected_signal,
            this, [this](int stage, int index, uint32_t value)
    {
//        qDebug() << "stage:" << stage << " - index:" << index << " - value:" << value;

        if (global_checkListItems.value(stage).at(index).autocheck_default == value)
        {
            global_checkListItems[stage][index].isChecked = true;
            this->checkListItems.at(index)->setItemStyle(true);
        }
        else
        {
            global_checkListItems[stage][index].isChecked = false;
            this->checkListItems.at(index)->setItemStyle(false);
        }

        this->checkAllItemStatus();
    });

    SimConnectThread::getInstance()->start();
}

void CheckListFrame::terminateDataRequestFromSim()
{
    lockForWrite();
    SimDataCollector::getInstance()->setTerminateDataRequests(true);
    unlock();
}


/* CheckListFrameThread Class */
CheckListFrameThread *CheckListFrameThread::getInstance()
{
    if (s_checkListFrameThread == nullptr) s_checkListFrameThread = new CheckListFrameThread();
    return s_checkListFrameThread;
}

void CheckListFrameThread::deleteInstance()
{
    if (s_checkListFrameThread != nullptr)
    {
        s_checkListFrameThread->deleteLater();
        s_checkListFrameThread = nullptr;
    }
}

void CheckListFrameThread::run()
{
    if (m_processId == 0)
    {
        processID_ONE();
    }
    else if (m_processId == 1)
    {
        processID_TWO();
    }
}

void CheckListFrameThread::processID_ONE()
{
    bool allChecked = true;
    for (int i = 0; i < global_checkListItems.value(m_stageNum).size(); i++)
    {
        if (!global_checkListItems.value(m_stageNum).at(i).isChecked)
        {
            allChecked = false;
            break;
        }
    }
    emit this->threadFinished_ID_ONE(allChecked);
}

void CheckListFrameThread::processID_TWO()
{
    if (m_status)
    {
        for (int i = 0; i < global_checkListItems.value(m_stageNum).size(); i++)
        {
            global_checkListItems[m_stageNum][i].isChecked = true;
        }
    }
    else
    {
        for (int i = 0; i < global_checkListItems.value(m_stageNum).size(); i++)
        {
            global_checkListItems[m_stageNum][i].isChecked = false;
        }
    }
    emit this->threadFinished_ID_TWO();
}

uint8_t CheckListFrameThread::getProcessId() const
{
    return m_processId;
}

void CheckListFrameThread::setProcessId(const uint8_t &processId)
{
    m_processId = processId;
}

int CheckListFrameThread::getStageNum() const
{
    return m_stageNum;
}

void CheckListFrameThread::setStageNum(int stageNum)
{
    m_stageNum = stageNum;
}

bool CheckListFrameThread::getStatus() const
{
    return m_status;
}

void CheckListFrameThread::setStatus(bool status)
{
    m_status = status;
}


/* SimConnectThread class */
SimConnectThread *SimConnectThread::getInstance()
{
    if (s_simConnectThread == nullptr)
    {
        s_simConnectThread = new SimConnectThread();
    }
    return s_simConnectThread;
}

void SimConnectThread::deleteInstance()
{
    if (s_simConnectThread != nullptr)
    {
        s_simConnectThread->deleteLater();
        s_simConnectThread = nullptr;
    }
}

SimConnectThread::~SimConnectThread()
{
    SimDataCollector::deleteInstance();
}

void SimConnectThread::run()
{
    if (isConnected)
    {
        SimDataCollector::getInstance()->setStageNum(m_stageNum);
        setIndexArray();
        SimDataCollector::getInstance()->startRequestData();
    }
}

bool SimConnectThread::setIndexArray()
{
    lockForWrite();
    m_indexes.clear();
    for (int i = 0; i < global_checkListItems.value(m_stageNum).size(); i++)
    {
        if (global_checkListItems.value(m_stageNum).at(i).autocheck_enable == 1)
        {
            m_indexes.append(i);
        }
    }
    SimDataCollector::getInstance()->setIndexes(m_indexes);
    unlock();
    return true;
}

void SimConnectThread::resetSurveillandStage(const int stageNum)
{
    lockForWrite();
    SimDataCollector::getInstance()->setStageNum(stageNum);
    setStageNum(stageNum);
    m_indexes.clear();

    for (int i = 0; i < global_checkListItems.value(m_stageNum).size(); i++)
    {
        if (global_checkListItems.value(m_stageNum).at(i).autocheck_enable == 1)
        {
            m_indexes.append(i);
        }
    }

    SimDataCollector::getInstance()->setIndexes(m_indexes);
    unlock();
}

void SimConnectThread::simConnect()
{
    if (!SimDataCollector::getInstance()->connectToSim())
    {
        isConnected = false;
//        qDebug() << "Connection Failed";
        return;
    }

    connect(SimDataCollector::getInstance(), &SimDataCollector::dataCollected_signal,
            this, [this](int stage, int index, uint32_t value)
    {
        emit this->dataCollected_signal(stage, index, value);
    });

    SimDataCollector::getInstance()->setRequests();
    isConnected = true;
//    qDebug() << "Connected";
}

void SimConnectThread::simDisconnect()
{
    if (!isConnected) return;

    SimDataCollector::getInstance()->clearDataDefinition();
    lockForWrite();
    SimDataCollector::getInstance()->setTerminateDataRequests(true);
    unlock();
    SimDataCollector::getInstance()->closeSimConnection();

    disconnect(SimDataCollector::getInstance(), nullptr, nullptr, nullptr);
    isConnected = false;
}


int SimConnectThread::stageNum() const
{
    return m_stageNum;
}

void SimConnectThread::setStageNum(const int stageNum)
{
    m_stageNum = stageNum;
}
