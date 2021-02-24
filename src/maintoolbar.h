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
 * @file maintoolbar.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPoint>
#include <QMouseEvent>
#include <QString>
#include <QIcon>
#include <utility>

#include "windows.h"
#include "checklistframe.h"
#include "datasettingsframe.h"

#define TOOLBARWIDTH 30
#define TOOLBARHEIGHT 30

class MainToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit MainToolBar(QWidget *parent = nullptr);
    virtual ~MainToolBar();

private:
    bool isPressed;
    bool windowStaysOnTop;
    bool checkListFrameVisible;
    bool dataSettingsFrameVisible;
    int stageNum;

    CheckListFrame *checkListFrame;
    DataSettingsFrame *dataSettingsFrame;

    QWidget *win;

    QLabel *aircraftTypeLabel;
    QLabel *currentStageLabel;
    QPushButton *topWindowButton;
    QPushButton *closeButton;
    QPushButton *minButton;
    QPushButton *dataSettingsButton;
    QPushButton *dropDownButton;

    QHBoxLayout *lay;
    QPoint startPos;

    void setupUI();
    void events();

    void setCurrentStageLabel();  // current status label depends on the stageNum
    void checkStage();
    void showAndHideCheckListFrame();
    void setDropDownArrowIcon(const bool &status);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void topWindowButton_onClicked();
    void dataSettingsButton_onClicked();
    void closeButton_onClicked();
    void minButton_onClicked();
    void dropDownButton_onClicked();

    void prevPGButton_onClicked();
    void nextPGButton_onClicked();

};

int WINAPI makeWindowStaysOnTop();
int WINAPI cancelWindowStaysOnTop();

#endif // MAINTOOLBAR_H
