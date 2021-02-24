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
 * @file checklistitem.cpp
 *
 * @brief setup UI for each individual item in checklist frame
 *
 * @author Tony Wang <2894735011@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include <QDebug>

#include "checklistitem.h"

CheckListItem::CheckListItem(uint8_t t_editable, bool *t_isChecked)
{
    this->contentLabel = new QLabel();
    this->displayIconLabel = new QLabel();
    this->lay = new QHBoxLayout();
    this->statusLabel = new QLabel();

    this->isHovered = false;
    this->isPressed = false;
    this->isChecked = t_isChecked;

    // Change uint8_t to bool variable for save memory space
    if (t_editable == 1) editable = true;
    else editable = false;

    this->setupUI();
    this->events();
}

CheckListItem::~CheckListItem()
{
    this->lay->deleteLater();
    this->lay = nullptr;

    this->contentLabel->deleteLater();
    this->contentLabel = nullptr;

    this->displayIconLabel->deleteLater();
    this->displayIconLabel = nullptr;

    this->statusLabel->deleteLater();
    this->statusLabel = nullptr;
}

void CheckListItem::setupUI()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setAttribute(Qt::WA_Hover, true);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    this->setStyleSheet("background:none;");

    this->setFixedHeight(ITEMHEIGHT);

    this->displayIconLabel->setFixedSize(40, ITEMHEIGHT);
    this->contentLabel->setFixedHeight(ITEMHEIGHT);
    this->statusLabel->setFixedHeight(ITEMHEIGHT);

    if (*(this->isChecked))
    {
        this->contentLabel->setStyleSheet("color:#00C308;");
        this->statusLabel->setStyleSheet("color:#00C308;padding-right:5px;");
        this->displayIconLabel->setPixmap(QPixmap("./icons/Checked Icon.png").scaled(this->displayIconLabel->size() - QSize(20, 20)));
    }
    else
    {
        this->contentLabel->setStyleSheet("color:white;");
        this->statusLabel->setStyleSheet("color:white;padding-right:5px;");
        this->displayIconLabel->setPixmap(QPixmap("./icons/UnCheck Icon.png").scaled(this->displayIconLabel->size() - QSize(20, 20)));
    }

    this->statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    this->contentLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->displayIconLabel->setAlignment(Qt::AlignCenter);

    this->setLayout(this->lay);
    this->lay->setSpacing(5);
    this->lay->setContentsMargins(0, 0, 0, 0);

    this->lay->addWidget(this->displayIconLabel);
    this->lay->addWidget(this->contentLabel);
    this->lay->addWidget(this->statusLabel);
}

void CheckListItem::events()
{

}

void CheckListItem::adjustItemStyle()
{
    if (*(this->isChecked))
    {
        setItemStyle(false);
        *(this->isChecked) = false;
    }
    else
    {
        setItemStyle(true);
        *(this->isChecked) = true;
    }
}

void CheckListItem::setItemStyle(const bool &status)
{
    if (status)
    {
        this->contentLabel->setStyleSheet(QString("color:#00C308;"));
        this->statusLabel->setStyleSheet(QString("color:#00C308;padding-right:5px;"));
        this->displayIconLabel->setPixmap(QPixmap("./icons/Checked Icon.png").scaled(this->displayIconLabel->size() - QSize(20, 20)));
    }
    else
    {
        this->contentLabel->setStyleSheet(QString("color:white;"));
        this->statusLabel->setStyleSheet(QString("color:white;padding-right:5px;"));
        this->displayIconLabel->setPixmap(QPixmap("./icons/UnCheck Icon.png").scaled(this->displayIconLabel->size() - QSize(20, 20)));
    }
}

void CheckListItem::enterEvent(QEvent *event)
{
    if (!isHovered) isHovered = true;
    QWidget::enterEvent(event);
}

void CheckListItem::leaveEvent(QEvent *event)
{
    if (isHovered) isHovered = false;
    QWidget::enterEvent(event);
}

void CheckListItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->isPressed = true;
        adjustItemStyle();
        emit this->itemStatusChanged_signal();
        QWidget::mousePressEvent(event);
    }
}

void CheckListItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->isPressed = false;
        QWidget::mouseReleaseEvent(event);
    }
}

void CheckListItem::paintEvent(QPaintEvent *event)
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

void CheckListItem::setContentLabel(const QString& content)
{
    this->contentLabel->setText(content);
    this->contentLabel->adjustSize();
}

void CheckListItem::setStatusLabel(const QString& status)
{
    this->statusLabel->setText(status);
    this->statusLabel->adjustSize();
}
