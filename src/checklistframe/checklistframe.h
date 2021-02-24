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
 * @file checklistframe.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef CHECKLISTFRAME_H
#define CHECKLISTFRAME_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QThread>
#include <QMetaProperty>
#include <QReadWriteLock>
#include <QLibrary>

#include "checklistitem.h"
#include "checklistbottomtoolbar.h"
#include "globalvars.h"

extern "C++"
{
    #include "simdatacollector.h"
}

class CheckListFrameThread : public QThread
{
    Q_OBJECT
public:
    static CheckListFrameThread *getInstance();
    static void deleteInstance();

    /* Getters & Setters */
    uint8_t getProcessId() const;
    void setProcessId(const uint8_t &processId);

    int getStageNum() const;
    void setStageNum(int stageNum);

    bool getStatus() const;
    void setStatus(bool status);

private:
    CheckListFrameThread() { m_processId = -1; }
    virtual ~CheckListFrameThread() { }

    uint8_t m_processId;

    /* In process id-2 */
    bool m_status;
    int m_stageNum;

    static CheckListFrameThread *s_checkListFrameThread;

    void run() override;

    void processID_ONE();
    void processID_TWO();

signals:
    void threadFinished_ID_ONE(bool);
    void threadFinished_ID_TWO();
};


class SimConnectThread : public QThread
{
    Q_OBJECT
public:
    SimConnectThread(const int &stageNum);
    virtual ~SimConnectThread();

    bool setIndexArray();

    SimDataCollector *simDataCollector;

    int stageNum() const;
    void setStageNum(int stageNum);

    void setSimDataCollectorConfig(const int stageNum);

private:
    int m_stageNum;
    QVector<int> m_indexes;

    void run() override;

signals:
    void dataCollected_signal(int, int, quint32);
};


class CheckListFrame : public QMainWindow
{
    Q_OBJECT
public:
    explicit CheckListFrame(QWidget *parent = nullptr);
    virtual ~CheckListFrame();

    void addCheckListItemWidgets(bool addBottomToolBar = true);
    void delCheckListItemWidgets();
    void setCheckListStage(int &t_stageNum);
    void startDataRequestFromSim();

protected:
    CheckListBottomToolBar *checkListBottomToolBar;

private:
    SimConnectThread *simConnectThread;
    QVector<CheckListItem*> checkListItems;

    QWidget *center;
    QVBoxLayout *mainLay;
    QVBoxLayout *checkListItemLay;
    QVBoxLayout *bottomToolBarLay;

    int m_stageNum;
    bool itemAllChecked;

    void setupUI();
    void events();

signals:
    void prevPGButton_onClicked_signal();
    void nextPGButton_onClicked_signal();

private slots:
    void prevButton_onClicked();
    void nextButton_onClicked();
    void checkAllButton_onClicked(const bool &status);
    void checkAllItemStatus();

};

#endif // CHECKLISTFRAME_H
