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
 * @file checklistbottomtoolbar.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef CHECKLISTBOTTOMTOOLBAR_H
#define CHECKLISTBOTTOMTOOLBAR_H

#include <QHBoxLayout>
#include <QPushButton>

#define BUTTONHEIGHT 30
#define BUTTONWIDTH 30

class CheckListBottomToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit CheckListBottomToolBar(QWidget *parent = nullptr);
    virtual ~CheckListBottomToolBar();

    void setCheckAllButtonStatus(const bool &status);

private:
    QPushButton *prevButton;  // Control page(stage) of the check list (prev page)
    QPushButton *nextButton;  // Control page(stage) of the check list (next page)
    QPushButton *checkAllButton;
    QHBoxLayout *lay;

    bool allChecked;

    void setupUI();
    void events();

signals:
    void prevButton_onClicked_signal();
    void nextButton_onClicked_signal();
    void checkAllButton_onClicked_signal(bool);

private slots:
    void prevButton_onClicked();
    void nextButton_onClicked();
    void checkAllButton_onClicked();

};

#endif // CHECKLISTBOTTOMTOOLBAR_H
