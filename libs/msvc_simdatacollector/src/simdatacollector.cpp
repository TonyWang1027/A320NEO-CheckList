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
 * @file simdatacollector.cpp
 *
 * @brief SimDataCollector DLL - gets MSFS2020 simulator variables
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#include "simdatacollector.h"

HRESULT hr;
HANDLE hSimConnect = NULL;
QReadWriteLock rwlock;

SimDataCollector *SimDataCollector::s_simDataCollector = nullptr;

void lockForRead()
{
    rwlock.lockForRead();
}

void lockForWrite()
{
    rwlock.lockForWrite();
}

void unlock()
{
    rwlock.unlock();
}

void CALLBACK Dispatch(SIMCONNECT_RECV *pData, DWORD cbData, void *pContext)
{
    switch (pData->dwID)
    {
        case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
        {
            SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
            switch (pObjData->dwRequestID)
            {
                case REQUEST:
                {
                    const SimResponse* pS = (SimResponse*)&pObjData->dwData;
                    // std::cout << "APU PCT STARTER: " << pS->apu_generator_s << std::endl;    --- sample
                    SimDataCollector::getInstance()->emitSignals(pS);
                    break;
                }
            }
            break;
        }
        case SIMCONNECT_RECV_ID_QUIT:
            SimDataCollector::getInstance()->setTerminateDataRequests(true);
            break;
        default:
            break;
    }
}

void SimDataCollector::emitSignals(const SimResponse*& pS)
{
    rwlock.lockForRead();
    if (stageNum == 0)
    {
        emit this->dataCollected_signal(stageNum, indexes.at(0), pS->brake_parking_indicator);
        emit this->dataCollected_signal(stageNum, indexes.at(1), pS->flaps_handle_index);
        emit this->dataCollected_signal(stageNum, indexes.at(2), pS->spoilers_armed);
        if (pS->general_eng_starter1 && pS->general_eng_starter2)
            emit this->dataCollected_signal(stageNum, indexes.at(3), 1);
        else
            emit this->dataCollected_signal(stageNum, indexes.at(3), 0);

        if (pS->gear_center_position && pS->gear_left_position && pS->gear_right_position)
            emit this->dataCollected_signal(stageNum, indexes.at(4), 1);
        else
            emit this->dataCollected_signal(stageNum, indexes.at(4), 0);

        emit this->dataCollected_signal(stageNum, indexes.at(5), pS->antiskid_brakes_active);
    }
    else if (stageNum == 1)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->light_nav_on);
    }
    // no autocheck in stage number 2
    else if (stageNum == 3)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->apu_generator_active);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(1), pS->light_beacon_on);
    }
    else if (stageNum == 4)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->brake_parking_indicator);
    }
    else if (stageNum == 5)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->brake_parking_indicator);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(1), pS->apu_generator_active);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(2), pS->spoilers_armed);
    }
    else if (stageNum == 6)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->light_taxi_on);
    }
    else if (stageNum == 7)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->light_landing);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(1), pS->light_landing_on);
    }
    // no autocheck in stage number 8
    else if (stageNum == 9)
    {
        if (pS->gear_center_position && pS->gear_left_position && pS->gear_right_position)
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), 1);
        else
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), 0);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(1), pS->flaps_handle_index);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(2), pS->spoilers_armed);
        if (pS->light_landing || pS->light_taxi_on)
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(3), 1);
        else
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(3), 0);
    }
    // no autocheck in stage number 10
    else if (stageNum == 11)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->light_landing_on);
    }
    // no autocheck in stage number 12 & 13 & 14
    else if (stageNum == 15)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->light_landing_on);
    }
    else if (stageNum == 16)
    {
        if (pS->gear_center_position && pS->gear_left_position && pS->gear_right_position)
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), 1);
        else
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), 0);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(1), pS->light_landing);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(2), pS->spoilers_armed);
    }
    else if (stageNum == 17)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->light_landing_on);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(1), pS->light_taxi_on);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(2), pS->flaps_handle_index);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(3), pS->spoilers_armed);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(4), pS->apu_generator_active);
    }
    else if (stageNum == 18)
    {
        if (pS->light_landing || pS->light_taxi_on)
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), 1);
        else
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), 0);
    }
    else if (stageNum == 19)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->brake_parking_indicator);
        if (pS->general_eng_starter1 && pS->general_eng_starter2)
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(1), 1);
        else
            emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(1), 0);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(2), pS->light_beacon_on);
    }
    else if (stageNum == 20)
    {
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(0), pS->light_nav_on);
        emit SimDataCollector::dataCollected_signal(stageNum, indexes.at(1), pS->apu_generator_active);
    }
    rwlock.unlock();
}

SimDataCollector::SimDataCollector()
{
    terminateDataRequests = false;
}

SimDataCollector *SimDataCollector::getInstance()
{
    if (s_simDataCollector == nullptr) s_simDataCollector = new SimDataCollector();
    return  s_simDataCollector;
}

void SimDataCollector::deleteInstance()
{
    if (s_simDataCollector != nullptr)
    {
        s_simDataCollector->deleteLater();
        s_simDataCollector = nullptr;
    }
}

bool SimDataCollector::connectToSim()
{
    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "A320CheckList", NULL, 0, NULL, 0)))
        return true;
    else
        return false;
}

void SimDataCollector::setRequests()
{
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "BRAKE PARKING INDICATOR", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "GENERAL ENG STARTER:1", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "GENERAL ENG STARTER:2", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "GEAR CENTER POSITION", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "GEAR RIGHT POSITION", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "GEAR LEFT POSITION", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "APU GENERATOR ACTIVE", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "FLAPS HANDLE INDEX", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "SPOILERS ARMED", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "LIGHT STROBE ON", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "AUTOPILOT FLIGHT DIRECTOR ACTIVE", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "ANTISKID BRAKES ACTIVE", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "LIGHT BEACON ON", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "LIGHT NAV ON", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "LIGHT TAXI ON", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "LIGHT LANDING ON", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "LIGHT LANDING", NULL, SIMCONNECT_DATATYPE_INT32);
    hr = SimConnect_AddToDataDefinition(hSimConnect, DEFINITION, "AUTOPILOT MASTER", NULL, SIMCONNECT_DATATYPE_INT32);

    hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQUEST, DEFINITION, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);
}

void SimDataCollector::startRequestData()
{
    // block current thread
    while (!terminateDataRequests) {
        // Continuously call SimConnect_CallDispatch until quit - MyDispatchProc1 will handle simulation events
        SimConnect_CallDispatch(hSimConnect, Dispatch, NULL);
        Sleep(1);
    }
}

void SimDataCollector::closeSimConnection()
{
    hr = SimConnect_Close(hSimConnect);
}

void SimDataCollector::clearDataDefinition()
{
    hr = SimConnect_ClearDataDefinition(hSimConnect, DEFINITION);
}

int SimDataCollector::getStageNum() const
{
    return stageNum;
}

void SimDataCollector::setStageNum(int value)
{
    stageNum = value;
}

QVector<int> SimDataCollector::getIndexes() const
{
    return indexes;
}

void SimDataCollector::setIndexes(const QVector<int> &value)
{
    indexes = value;
}

bool SimDataCollector::getTerminateDataRequests() const
{
    return terminateDataRequests;
}

void SimDataCollector::setTerminateDataRequests(bool value)
{
    terminateDataRequests = value;
}
