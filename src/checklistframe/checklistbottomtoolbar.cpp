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
 * @file checklistbottomtoolbar.cpp
 *
 * @brief setup bottom toolbar UI in checklist frame
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include <QDebug>

#include "checklistbottomtoolbar.h"

CheckListBottomToolBar::CheckListBottomToolBar(QWidget *parent) : QWidget(parent)
{
    this->prevButton = new QPushButton();
    this->nextButton = new QPushButton();
    this->checkAllButton = new QPushButton();
    this->lay = new QHBoxLayout();

    this->allChecked = false;

    setupUI();
    events();
}

CheckListBottomToolBar::~CheckListBottomToolBar()
{
    this->lay->deleteLater();
    this->lay = nullptr;

    this->prevButton->deleteLater();
    this->lay = nullptr;

    this->nextButton->deleteLater();
    this->nextButton = nullptr;

    this->checkAllButton->deleteLater();
    this->checkAllButton = nullptr;
}

void CheckListBottomToolBar::setupUI()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->setStyleSheet("background:none;");

    this->setFixedHeight(45);

    this->checkAllButton->setObjectName("checkAllButton");
    this->prevButton->setObjectName("prevButton");
    this->nextButton->setObjectName("nextButton");

    this->checkAllButton->setFixedSize(BUTTONWIDTH, BUTTONHEIGHT);
    this->prevButton->setFixedSize(BUTTONWIDTH, BUTTONHEIGHT);
    this->nextButton->setFixedSize(BUTTONWIDTH, BUTTONHEIGHT);

    this->checkAllButton->setToolTip(QString::fromUtf8("全部完成"));
    this->prevButton->setToolTip(QString::fromUtf8("检查单上一阶段"));
    this->nextButton->setToolTip(QString::fromUtf8("检查单下一阶段"));

    this->checkAllButton->setStyleSheet("QPushButton#checkAllButton{ border-radius:15px;background:#464659; }");
    this->prevButton->setStyleSheet("QPushButton#prevButton{ border-radius:15px;background:#464659; }");
    this->nextButton->setStyleSheet("QPushButton#nextButton{ border-radius:15px;background:#464659; }");

    this->setLayout(this->lay);
    this->lay->setSpacing(3);
    this->lay->setContentsMargins(0, 0, 0, 0);

    this->prevButton->setIcon(QIcon(QPixmap("./icons/Left Pointing Icon.png")));
    this->nextButton->setIcon(QIcon(QPixmap("./icons/Right Pointing Icon.png")));

    this->lay->addWidget(this->checkAllButton, 1, Qt::AlignRight);
    this->lay->addWidget(this->prevButton);
    this->lay->addWidget(this->nextButton);
}

void CheckListBottomToolBar::events()
{
    connect(this->prevButton, SIGNAL(clicked(bool)), this, SLOT(prevButton_onClicked()));
    connect(this->nextButton, SIGNAL(clicked(bool)), this, SLOT(nextButton_onClicked()));
    connect(this->checkAllButton, SIGNAL(clicked(bool)), this, SLOT(checkAllButton_onClicked()));
}

void CheckListBottomToolBar::setCheckAllButtonStatus(const bool &status)
{
    allChecked = status;
    if (status)
    {
        this->checkAllButton->setIcon(QIcon(QPixmap("./icons/Select All Icon (enable).png")));
    }
    else
    {
        this->checkAllButton->setIcon(QIcon(QPixmap("./icons/Select All Icon (disable).png")));
    }
}

void CheckListBottomToolBar::prevButton_onClicked()
{
    emit this->prevButton_onClicked_signal();
}

void CheckListBottomToolBar::nextButton_onClicked()
{
    emit this->nextButton_onClicked_signal();
}

void CheckListBottomToolBar::checkAllButton_onClicked()
{
    if (!allChecked)
    {
        emit this->checkAllButton_onClicked_signal(true);
        this->checkAllButton->setIcon(QIcon(QPixmap("./icons/Select All Icon (enable).png")));
        allChecked = true;
    }
    else
    {
        emit this->checkAllButton_onClicked_signal(false);
        this->checkAllButton->setIcon(QIcon(QPixmap("./icons/Select All Icon (disable).png")));
        allChecked = false;
    }
}
