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
 * @file datasettingsbottomtoolbar.cpp
 *
 * @brief setup bottom toolbar UI in data settings frame
 *
 * @author Tony Wang <2894735011@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include "datasettingsbottomtoolbar.h"

DataSettingsBottomToolBar::DataSettingsBottomToolBar(QWidget *parent) : QWidget(parent)
{
    this->userDataLineEdit = new QLineEdit();
    this->lay = new QHBoxLayout();

    setupUI();
}

DataSettingsBottomToolBar::~DataSettingsBottomToolBar()
{
    this->lay->deleteLater();
    this->lay = nullptr;

    this->userDataLineEdit->deleteLater();
    this->userDataLineEdit = nullptr;
}

void DataSettingsBottomToolBar::setupUI()
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->setStyleSheet("background:none;");
    this->userDataLineEdit->setStyleSheet("background:#373446;color:white;border:none;");

    this->setFixedHeight(30);

    this->userDataLineEdit->setFixedHeight(25);

    this->userDataLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    this->setLayout(this->lay);
    this->lay->setSpacing(0);
    this->lay->setContentsMargins(5, 0, 5, 0);

    this->lay->addWidget(this->userDataLineEdit);
}

const QString DataSettingsBottomToolBar::getUserDataFromLE()
{
    return (this->userDataLineEdit->text());
}

void DataSettingsBottomToolBar::cleanLineEdit()
{
    this->userDataLineEdit->clear();
}
