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
 * @file mxmlparser.cpp
 *
 * @brief parse checklist template from a xml file
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include <QDebug>

#include "mxmlparser.h"

MxmlParser::MxmlParser()
    : stageNum(0)
{

}

MxmlParser::~MxmlParser() { }

uint8_t MxmlParser::readFile(const QString &filename)
{
    // return value:
    // 1: OK
    // 0: Unknown error occured
    // -1: Cannot read xml file
    // -2: Not a standard A320 check list file

    QFile xmlFile(filename);
    if(!xmlFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "ERROR: Cannot read your file!" << " - " << "File name: " << filename << "\n";
        return -1;
    }
    xmlReader.setDevice(&xmlFile);

    xmlReader.readNext();
    while (!xmlReader.atEnd())
    {
        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "CheckListDetail")
            {
                readCheckListMembers();
            }
            else
            {
                xmlReader.raiseError(QObject::tr("[[Not a standard A320 check list file!]]"));
            }
        }
        else
        {
            xmlReader.readNext();
        }
    }

    xmlFile.close();

    if (xmlReader.hasError())
    {
        qDebug() << "Not a standard A320 check list file!\n";
        return -2;
    }
    else if (xmlFile.error() != QFile::NoError)
    {
        qDebug() << "ERROR: Cannot read your file!" << " - " << "File name: " << filename << "\n";
        return 0;
    }
    return 1;
}

QMap<int, QVector<QCheckList::QCLItem>>& MxmlParser::getCheckListDetails()
{
    return checkListDetails;
}

QMultiMap<int, int> MxmlParser::getEditableItems()
{
    QMultiMap<int, int> editableItems_reversed;

    QMultiMap<int, int>::const_iterator ci = editableItems.constBegin();

    while (ci != editableItems.constEnd())
    {
        editableItems_reversed.insertMulti(ci.key(),ci.value());
        ++ci;
    }
    return editableItems_reversed;
}

void MxmlParser::readCheckListMembers()
{
    xmlReader.readNext();
    while (!xmlReader.atEnd())
    {
        if (xmlReader.isEndElement())
        {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement())
        {
            selectCheckListStage();
        }
        else
        {
            xmlReader.readNext();
        }
    }
}

void MxmlParser::selectCheckListStage()
{
    QStringRef stageName = xmlReader.name();
    QStringRef stageNameCN = xmlReader.attributes().value("stageNameCN");
    stageNum = xmlReader.attributes().value("stageNum").toInt();
    if (stageName >= 0 && stageNum <= 20)
    {
        currentStageCheckListItems.stageName = stageNameCN.toString();
        readMembers();
        checkListDetails.insert(stageNum, checkListItems);
        checkListItems.clear();
    }
    else
    {
        skipUnknownElement();
    }
}

void MxmlParser::readCheckListStage()
{
    xmlReader.readNext();
    while (!xmlReader.atEnd())
    {
        if (xmlReader.isEndElement())
        {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "block")
            {
                readMembers();
            }
            else
            {
                skipUnknownElement();
            }
        }
        else
        {
            xmlReader.readNext();
        }
    }
}

void MxmlParser::readMembers()
{
    xmlReader.readNext();
    while (!xmlReader.atEnd())
    {
        if (xmlReader.isEndElement())
        {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement())
        {
            if(xmlReader.name() == "item")
            {
                readItem(xmlReader.attributes().value("id"));
            }
            else
            {
                skipUnknownElement();
            }
        }
        else
        {
            xmlReader.readNext();
        }
    }
}

void MxmlParser::readItem(const QStringRef itemId)
{
    xmlReader.readNext();

    QString itemName;
    QString status;
    uint8_t editable;
    uint8_t autocheck_enable;
    uint8_t autocheck_default;

    while (!xmlReader.atEnd())
    {
        if (xmlReader.isEndElement())
        {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "itemName")
            {
                itemName = xmlReader.readElementText();
            }
            else if (xmlReader.name() == "status")
            {
                status = xmlReader.readElementText();
            }
            else if (xmlReader.name() == "editable")
            {
                editable = xmlReader.readElementText().toInt();
            }
            else if (xmlReader.name() == "autocheck")
            {
                QStringRef strf1 = xmlReader.attributes().value("enable");
                autocheck_enable = strf1.toInt();

                QStringRef strf2 = xmlReader.attributes().value("default");
                autocheck_default = strf2.toInt();

                xmlReader.readElementText();  // move curser
            }
            else
            {
                skipUnknownElement();
            }
        }
        xmlReader.readNext();
    }

    if (editable == 1)
    {
        editableItems.insertMulti(stageNum, itemId.toInt());
    }

    currentStageCheckListItems.id = itemId.toInt();
    currentStageCheckListItems.itemName = itemName;
    currentStageCheckListItems.status = status;
    currentStageCheckListItems.editable = editable;
    currentStageCheckListItems.autocheck_enable = autocheck_enable;
    currentStageCheckListItems.autocheck_default = autocheck_default;
    currentStageCheckListItems.isChecked = false;

    checkListItems.append(std::move(currentStageCheckListItems));
}

void MxmlParser::skipUnknownElement()
{
    xmlReader.readNext();
    while (!xmlReader.atEnd())
    {
        if (xmlReader.isEndElement())
        {
            xmlReader.readNext();
            break;
        }

        if (xmlReader.isStartElement())
        {
            skipUnknownElement();
        }
        else
        {
            xmlReader.readNext();
        }
    }
}

MxmlParser global_mxmlParser;

void readFile()
{
    global_mxmlParser.readFile("cl_v4.xml");
}

QMap<int, QVector<QCheckList::QCLItem>> &getCheckListDetails()
{
    return global_mxmlParser.getCheckListDetails();
}

QMultiMap<int, int> getEditableItems()
{
    return global_mxmlParser.getEditableItems();
}
