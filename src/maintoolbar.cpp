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
 * @file maintoolbar.cpp
 *
 * @brief setup tool bar UI, controls data settings frame and checklist frame
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include <QDebug>

#include "maintoolbar.h"

MainToolBar::MainToolBar(QWidget *parent) : QWidget(parent)
{
    /* Declaration */
    this->aircraftTypeLabel = new QLabel();
    this->currentStageLabel = new QLabel();
    this->topWindowButton = new QPushButton();
    this->dataSettingsButton = new QPushButton();
    this->minButton = new QPushButton();
    this->closeButton = new QPushButton();
    this->dropDownButton = new QPushButton();
    this->lay = new QHBoxLayout();

    /* Init Valuables */
    this->win = parent;
    this->isPressed = false;
    this->windowStaysOnTop = false;
    this->checkListFrameVisible = false;
    this->dataSettingsFrameVisible = false;
    this->stageNum = 0;
    this->checkListFrame = nullptr;
    this->dataSettingsFrame = nullptr;

    /* Methods & Functions */
    this->setupUI();  // build layout
    this->events();

    SimConnectThread::getInstance()->simConnect();
}

MainToolBar::~MainToolBar()  // virtual destructor
{
    SimConnectThread::getInstance()->simDisconnect();
    SimConnectThread::deleteInstance();

    delete this->lay;
    this->lay = nullptr;

    delete this->aircraftTypeLabel;
    this->aircraftTypeLabel = nullptr;

    delete this->currentStageLabel;
    this->currentStageLabel = nullptr;

    delete this->topWindowButton;
    this->topWindowButton = nullptr;

    delete this->dataSettingsButton;
    this->dataSettingsButton = nullptr;

    delete this->minButton;
    this->minButton = nullptr;

    delete this->closeButton;
    this->closeButton = nullptr;

    delete this->dropDownButton;
    this->dropDownButton = nullptr;

    if (this->checkListFrame != nullptr)
    {
        this->checkListFrame->close();
        this->checkListFrame->deleteLater();
        this->checkListFrame = nullptr;
    }
    if (this->dataSettingsFrame != nullptr)
    {
        this->dataSettingsFrame->close();
        this->dataSettingsFrame->deleteLater();
        this->dataSettingsFrame = nullptr;
    }
}

void MainToolBar::setupUI()
{
    this->setFixedHeight(TOOLBARHEIGHT);

    this->aircraftTypeLabel->setFixedSize(80, TOOLBARHEIGHT);
    this->currentStageLabel->setFixedSize(130, TOOLBARHEIGHT);
    this->dataSettingsButton->setFixedSize(TOOLBARWIDTH, TOOLBARHEIGHT);
    this->minButton->setFixedSize(TOOLBARWIDTH, TOOLBARHEIGHT);
    this->closeButton->setFixedSize(TOOLBARWIDTH, TOOLBARHEIGHT);
    this->topWindowButton->setFixedSize(TOOLBARWIDTH, TOOLBARHEIGHT);
    this->dropDownButton->setFixedSize(TOOLBARWIDTH, TOOLBARHEIGHT);

    this->aircraftTypeLabel->setObjectName("aircraftTypeLabel");
    this->dataSettingsButton->setObjectName("dataSettingButton");
    this->minButton->setObjectName("minButton");
    this->closeButton->setObjectName("closeButton");
    this->dropDownButton->setObjectName("dropDownButton");
    this->topWindowButton->setObjectName("topWindowButton");

    this->aircraftTypeLabel->setStyleSheet("QLabel#aircraftTypeLabel{ color:white; }");
    this->dataSettingsButton->setStyleSheet("QPushButton#dataSettingButton{ border:none;background:none; }"
                                   "QPushButton#dataSettingButton:hover { border:none;background:#464659; }");
    this->minButton->setStyleSheet("QPushButton#minButton{ border:none;background:none; }"
                                   "QPushButton#minButton:hover { border:none;background:#464659; }");
    this->closeButton->setStyleSheet("QPushButton#closeButton{ border:none;background:none; }"
                                     "QPushButton#closeButton:hover { border:none;background:#464659; }");
    this->dropDownButton->setStyleSheet("QPushButton#dropDownButton{ border:none;background:none; }"
                                     "QPushButton#dropDownButton:hover { border:none;background:#464659; }");
    this->topWindowButton->setStyleSheet("QPushButton#topWindowButton{ border:none;background:none; }"
                                     "QPushButton#topWindowButton:hover { border:none;background:#464659; }");

    this->aircraftTypeLabel->setToolTip(QString::fromUtf8("v0.10.1"));
    this->dataSettingsButton->setToolTip(QString::fromUtf8("设置检查单数据"));
    this->topWindowButton->setToolTip(QString::fromUtf8("置顶窗口"));
    this->dropDownButton->setToolTip(QString::fromUtf8("显示/隐藏检查单"));
    this->minButton->setToolTip(QString::fromUtf8("最小化窗口"));
    this->closeButton->setToolTip(QString::fromUtf8("关闭窗口"));

    this->currentStageLabel->setAlignment(Qt::AlignCenter);
    this->aircraftTypeLabel->setAlignment(Qt::AlignCenter);

    this->aircraftTypeLabel->setText(QString::fromUtf8("A320NEO-CL"));

    // Set layout
    this->setLayout(this->lay);
    this->lay->setSpacing(0);
    this->lay->setContentsMargins(0, 0, 0, 0);

    this->dataSettingsButton->setIcon(QIcon(QPixmap("./icons/Settings Icon.png")));
    this->minButton->setIcon(QIcon(QPixmap("./icons/Min Icon.png")));
    this->closeButton->setIcon(QIcon(QPixmap("./icons/Close Icon.png")));
    this->topWindowButton->setIcon(QIcon(QPixmap("./icons/Needle Icon (disable).png")));
    setDropDownArrowIcon(true);
    setCurrentStageLabel();

    // Add widgets to layout
    this->lay->addWidget(this->aircraftTypeLabel);
    this->lay->addWidget(this->currentStageLabel, 1, Qt::AlignCenter);
    this->lay->addWidget(this->topWindowButton);
    this->lay->addWidget(this->dataSettingsButton);
    this->lay->addWidget(this->dropDownButton);
    this->lay->addWidget(this->minButton);
    this->lay->addWidget(this->closeButton);
}

void MainToolBar::events()
{
    connect(this->topWindowButton, SIGNAL(clicked(bool)), this, SLOT(topWindowButton_onClicked()));
    connect(this->minButton, SIGNAL(clicked(bool)), this, SLOT(minButton_onClicked()));
    connect(this->closeButton, SIGNAL(clicked(bool)), this, SLOT(closeButton_onClicked()));
    connect(this->dropDownButton, SIGNAL(clicked(bool)), this, SLOT(dropDownButton_onClicked()));
    connect(this->dataSettingsButton, SIGNAL(clicked(bool)), this, SLOT(dataSettingsButton_onClicked()));
}

//---override mouse events (mousePressEvent, mouseMoveEvent, mouseReleaseEvent)---//
void MainToolBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->isPressed = true;
        this->startPos = event->globalPos();
        this->setMouseTracking(true);
        event->accept();
    }
}

void MainToolBar::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) && (this->isPressed == true))
    {
        QPoint movePos = event->globalPos() - this->startPos;
        this->startPos = event->globalPos();
        this->win->move(this->win->pos() + movePos);   // move main tool bar
        if (this->checkListFrame != nullptr)
            this->checkListFrame->move(this->checkListFrame->pos() + movePos);   // move check list frame
        if (this->dataSettingsFrame != nullptr)
            this->dataSettingsFrame->move(this->dataSettingsFrame->pos() + movePos);   // move data settings frame
        event->accept();
    }
}

void MainToolBar::mouseReleaseEvent(QMouseEvent *event)
{
    this->isPressed = false;
    QWidget::mouseReleaseEvent(event);
}

/* Events */
void MainToolBar::topWindowButton_onClicked()
{
    if(this->windowStaysOnTop == false)
    {
        this->topWindowButton->setIcon(QIcon(QPixmap("./icons/Needle Icon (enable).png")));
        makeWindowStaysOnTop();
        this->windowStaysOnTop = true;
    }
    else
    {
        this->topWindowButton->setIcon(QIcon(QPixmap("./icons/Needle Icon (disable).png")));
        cancelWindowStaysOnTop();
        this->windowStaysOnTop = false;
    }
}

void MainToolBar::dataSettingsButton_onClicked()
{
    if(checkListFrame == nullptr)
    {
        if(!this->dataSettingsFrameVisible)
        {
            this->dataSettingsFrame = new DataSettingsFrame(this);
            this->dataSettingsFrame->setGeometry(this->win->x(), (this->win->y() + 30), this->win->width(), 480);
            this->dataSettingsFrame->show();

            this->dataSettingsFrameVisible = true;
        }
        else
        {
            this->dataSettingsFrame->close();
            this->dataSettingsFrame->deleteLater();
            this->dataSettingsFrame = nullptr;

            this->dataSettingsFrameVisible = false;
        }
    }
}

void MainToolBar::closeButton_onClicked()
{
    if (this->checkListFrame != nullptr)
    {
        this->checkListFrame->close();
        this->checkListFrame->deleteLater();
        this->checkListFrame = nullptr;
    }
    if (this->dataSettingsFrame != nullptr)
    {
        this->dataSettingsFrame->close();
        this->dataSettingsFrame->deleteLater();
        this->dataSettingsFrame = nullptr;
    }
    this->win->close();
}

void MainToolBar::minButton_onClicked()
{
    this->win->showMinimized();
}

void MainToolBar::dropDownButton_onClicked()
{
    showAndHideCheckListFrame();
}

void MainToolBar::showAndHideCheckListFrame()
{
    if(this->dataSettingsFrame == nullptr)
    {
        if(checkListFrameVisible == false)
        {
            this->checkListFrame = new CheckListFrame(this);
            this->checkListFrame->setGeometry(this->win->x(), (this->win->y() + 30), this->win->width(), 690);

            setDropDownArrowIcon(false);

            this->checkListFrame->setCheckListStage(stageNum);
            this->checkListFrame->startDataRequestFromSim();
            this->checkListFrame->addCheckListItemWidgets();

            connect(this->checkListFrame, SIGNAL(prevPGButton_onClicked_signal()), this, SLOT(prevPGButton_onClicked()));
            connect(this->checkListFrame, SIGNAL(nextPGButton_onClicked_signal()), this, SLOT(nextPGButton_onClicked()));

            this->checkListFrame->show();
            this->checkListFrameVisible = true;
        }
        else
        {
            // If check list item bar is all checked in current stage,
            // then close the drop down window will automatic turn to next check list stage
            setDropDownArrowIcon(true);

            this->checkListFrame->terminateDataRequestFromSim();

            this->checkListFrame->close();
            this->checkListFrame->deleteLater();
            this->checkListFrame = nullptr;

            checkStage();

            this->checkListFrameVisible = false;
        }
    }
}

void MainToolBar::setDropDownArrowIcon(const bool &status)
{
    if (status)
    {
        this->dropDownButton->setIcon(QIcon(QPixmap("./icons/Drop-Down Arrow Icon (down).png")));
    }
    else
    {
        this->dropDownButton->setIcon(QIcon(QPixmap("./icons/Drop-Down Arrow Icon (up).png")));
    }
}

void MainToolBar::prevPGButton_onClicked()
{
    stageNum--;
    setCurrentStageLabel();
}

void MainToolBar::nextPGButton_onClicked()
{
    stageNum++;
    setCurrentStageLabel();
}

void MainToolBar::setCurrentStageLabel()
{
    if (global_checkListItems.size() == 0) return;

    this->currentStageLabel->setText(QString::fromUtf8(global_checkListItems.value(stageNum).first().stageName.toUtf8()));
    this->currentStageLabel->setStyleSheet("background-color:#4A413C;color:white;margin-left:5px;margin-right:5px;font: 75 10pt '微软雅黑';");
    this->currentStageLabel->setAlignment(Qt::AlignCenter);
    this->currentStageLabel->adjustSize();
}

void MainToolBar::checkStage()
{
    // If check list running to the last stage,
    // then will not check whether check list item bar is all checked or not

    //// TODO: Move this method to a new thread
    if (global_checkListItems.size() == (stageNum + 1)) return;

    bool allChecked = true;
    for (int i = 0; i < global_checkListItems.value(stageNum).size(); i++)
    {
        if (!global_checkListItems.value(stageNum).at(i).isChecked)
        {
            allChecked = false;
            break;
        }
    }
    if (allChecked)
    {
        stageNum++;
        setCurrentStageLabel();
    }
}

int WINAPI makeWindowStaysOnTop()
{
    HWND hWnd = NULL;
    HWND htmp = NULL;
    POINT pt;
    GetCursorPos(&pt);

    hWnd = WindowFromPoint(pt);

    if(NULL == hWnd)
        return -1;

    while((htmp = GetParent(hWnd)))
    {
        hWnd = htmp;
    }

    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);   // set topmost

    return 0;
}

int WINAPI cancelWindowStaysOnTop()
{
    HWND hWnd = NULL;
    HWND htmp = NULL;
    POINT pt;

    GetCursorPos(&pt);

    hWnd = WindowFromPoint(pt);

    if(NULL == hWnd)
        return -1;

    while((htmp = GetParent(hWnd)))
    {
        hWnd = htmp;
    }

    SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);   // cancel topmost

    return 0;
}
