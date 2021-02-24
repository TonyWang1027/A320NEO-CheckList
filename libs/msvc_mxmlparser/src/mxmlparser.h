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
 * @file mxmlparser.h
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef MXMLPARSER_H
#define MXMLPARSER_H

#include <QtCore/qglobal.h>

#if defined(MXMLPARSER_LIBRARY)
#  define MXMLPARSER_EXPORT Q_DECL_EXPORT
#else
#  define MXMLPARSER_EXPORT Q_DECL_IMPORT
#endif

#include <QString>
#include <QVector>
#include <QStringRef>
#include <QXmlStreamReader>
#include <QFile>
#include <QObject>
#include <QMap>
#include <QMultiMap>
#include <QVector>

#include "checklistdefine.h"

class MXMLPARSER_EXPORT MxmlParser
{
public:
    MxmlParser();
    ~MxmlParser();

    uint8_t readFile(const QString& filename);

    QMap<int, QVector<QCheckList::QCLItem>>& getCheckListDetails();
    QMultiMap<int, int> getEditableItems();

private:
    void readCheckListMembers();
    void selectCheckListStage();
    void readCheckListStage();

    void readMembers();
    void readItem(const QStringRef itemId);

    void skipUnknownElement();

    int stageNum;

    QXmlStreamReader xmlReader;

    QCheckList::QCLItem currentStageCheckListItems;
    QVector<QCheckList::QCLItem> checkListItems;
    QMap<int, QVector<QCheckList::QCLItem>> checkListDetails;
    QMultiMap<int, int> editableItems;  // Key -> stageNum, value -> itemId in each stage
};

extern MxmlParser global_mxmlParser;
extern "C++" MXMLPARSER_EXPORT void readFile();
extern "C++" MXMLPARSER_EXPORT QMap<int, QVector<QCheckList::QCLItem>>& getCheckListDetails();
extern "C++" MXMLPARSER_EXPORT QMultiMap<int, int> getEditableItems();

#endif // MXMLPARSER_H
