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
 * @file datasettingsframe.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef DATASETTINGSFRAME_H
#define DATASETTINGSFRAME_H

#include <iostream>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QVector>
#include <QThread>

#include "datablock.h"
#include "datasettingsbottomtoolbar.h"
#include "globalvars.h"

class DataSettingsThread : public QThread
{
    Q_OBJECT
public:
    DataSettingsThread(const int &dataId, const QString &userDataString);
    virtual ~DataSettingsThread() { }

private:
    int m_dataId;
    QString m_userDataString;

    void run() override;

signals:
    void dataSettingsThreadFinished();

};

class DataSettingsFrame : public QMainWindow
{
    Q_OBJECT
public:
    explicit DataSettingsFrame(QWidget *parent = nullptr);
    virtual ~DataSettingsFrame();

private:
    QWidget *center;
    QVBoxLayout *dataBlockLay;
    QVBoxLayout *bottomToolBarLay;
    QVBoxLayout *mainLay;

    QVector<DataBlock*> dataBlocks;

    DataSettingsBottomToolBar *dataSettingsBottomToolBar;
    DataSettingsThread *dataSettingsThread;

    void addDataBlockWidget();
    void deleteDataBlockWidget();
    void setupUI();

private slots:
    void dataBlockPressed(const int &dataId);

};

#endif // DATASETTINGSFRAME_H
