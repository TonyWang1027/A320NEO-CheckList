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
 * @file datablock.cpp
 *
 * @brief setup UI for each individual item in data settings frame
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include "datablock.h"

DataBlock::DataBlock(const int dataId, const QString& dataName, const QString& userData)
{
    this->dataNameLabel = new QLabel();
    this->userDataLabel = new QLabel();
    this->lay = new QHBoxLayout();

    this->isHovered = false;
    this->isPressed = false;
    this->dataId = dataId;
    setDataName(dataName);
    setUserData(userData);

    this->setupUI();
    this->events();
}

DataBlock::~DataBlock()
{
    this->lay->deleteLater();
    this->lay = nullptr;

    this->dataNameLabel->deleteLater();
    this->dataNameLabel = nullptr;

    this->userDataLabel->deleteLater();
    this->userDataLabel = nullptr;
}

void DataBlock::setupUI()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setAttribute(Qt::WA_Hover, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->setStyleSheet("background:none;");
    this->dataNameLabel->setStyleSheet("color:white;");
    this->userDataLabel->setStyleSheet("color:white;");

    this->setFixedHeight(BLOCKHEIGHT);

    this->dataNameLabel->setFixedSize(160, BLOCKHEIGHT);
    this->userDataLabel->setFixedHeight(BLOCKHEIGHT);

    this->dataNameLabel->setText(this->dataName + ":");

    this->dataNameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->userDataLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    this->dataNameLabel->setIndent(5);

    this->setLayout(this->lay);
    this->lay->setSpacing(0);
    this->lay->setContentsMargins(5, 0, 0, 5);

    this->lay->addWidget(this->dataNameLabel);
    this->lay->addWidget(this->userDataLabel, 1, Qt::AlignLeft);
}

void DataBlock::events()
{

}

void DataBlock::enterEvent(QEvent *event)
{
    if (!isHovered) isHovered = true;
    QWidget::enterEvent(event);
}

void DataBlock::leaveEvent(QEvent *event)
{
    if (isHovered) isHovered = false;
    QWidget::enterEvent(event);
}

void DataBlock::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // Antialiasing
    QPen pen;
    pen.setWidth(3);
    if (!isHovered)
    {
        const QColor color(50, 50, 63);
        pen.setColor(color);
    }
    else
    {
        const QColor color(121, 178, 173);
        pen.setColor(color);
    }
    const QPen cPen = pen;
    painter.setPen(cPen);
    painter.setBrush(QBrush(Qt::transparent));
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 8, 8);
    QWidget::paintEvent(event);
}

void DataBlock::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->isPressed = true;
        emit this->mousePressed_signal(getDataId());
        QWidget::mousePressEvent(event);
    }
}

void DataBlock::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->isPressed = false;
        QWidget::mouseReleaseEvent(event);
    }
}

QString DataBlock::getDataName() const
{
    return dataName;
}

void DataBlock::setDataName(const QString &value)
{
    dataName = value;
}

QString DataBlock::getUserData() const
{
    return userData;
}

void DataBlock::setUserData(const QString &value)
{
    userData = value;
    this->userDataLabel->setText(userData);
}

int DataBlock::getDataId() const
{
    return dataId;
}

void DataBlock::setDataId(int value)
{
    dataId = value;
}

