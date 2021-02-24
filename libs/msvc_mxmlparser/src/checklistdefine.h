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
 * @file checklistdefine.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef CHECKLISTDEFINE_H
#define CHECKLISTDEFINE_H

#include <QString>
#include <iostream>

namespace QCheckList
{
    struct QCLItem
    {
        int id;
        QString stageName;
        QString itemName;
        QString status;
        uint8_t editable;  // 1 for editable item, 0 for not editable item
        uint8_t autocheck_enable;
        uint8_t autocheck_default;
        bool isChecked;
    };
}

#endif // CHECKLISTDEFINE_H
