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
 * @file simdatacollector.h
 *
 * @brief SimDataCollector dynamic-link library header file
 *
 * @author Tony Wang <634599706@qq.com>
 *
 * @date 2021/02/24
 *
 */



#ifndef SIMDATACOLLECTOR_H
#define SIMDATACOLLECTOR_H

#include <QtCore/qglobal.h>

#define SIMDATACOLLECTOR_EXPORT Q_DECL_EXPORT

#include <QtCore>
#include <windows.h>
#include <QObject>
#include <QVector>
#include <QReadWriteLock>
#include <QDebug>

#include "SimConnect.h"

extern HRESULT hr;
extern HANDLE hSimConnect;
extern QReadWriteLock rwlock;

enum DATA_DEFINE_ID {
    DEFINITION,
};

enum DATA_REQUEST_ID {
    REQUEST,
};

struct SimResponse {
    uint32_t brake_parking_indicator;  // parking brake
    uint32_t general_eng_starter1;  // engine_1 master starter switch
    uint32_t general_eng_starter2;  // engine_2 master starter switch
    uint32_t gear_center_position;  // gear center
    uint32_t gear_right_position;  // gear right
    uint32_t gear_left_position;  // gear left
    uint32_t apu_generator_active;  // apu available
    uint32_t flaps_handle_index;  // flaps position
    uint32_t spoilers_armed;  // spoilers
    uint32_t light_strobe_on;  // strobe light
    uint32_t autopilot_flight_director_active;  // flight director
    uint32_t antiskid_brakes_active;  // brakes anti-skid
    uint32_t light_beacon_on;  // beacon lights
    uint32_t light_nav_on;   // nav&log lights
    uint32_t light_taxi_on;  // taxi lights (taxi mode)
    uint32_t light_landing_on;  // landing lights
    uint32_t light_landing;  // taxi lights (T.O mode)
    uint32_t autopilot_master;  // autopilot_1 switch (AP1)
};

class SIMDATACOLLECTOR_EXPORT SimDataCollector : public QObject
{
    Q_OBJECT
public:
    static SimDataCollector *getInstance();
    static void deleteInstance();

    bool connectToSim();
    void setRequests();
    void startRequestData();
    void closeSimConnection();
    void clearDataDefinition();
    void emitSignals(const SimResponse*& pS);

    int getStageNum() const;
    void setStageNum(int value);

    QVector<int> getIndexes() const;
    void setIndexes(const QVector<int> &value);

    bool getTerminateDataRequests() const;
    void setTerminateDataRequests(bool value);

private:
    SimDataCollector();
    virtual ~SimDataCollector() { }

    bool terminateDataRequests;
    int stageNum;
    QVector<int> indexes;

    static SimDataCollector *s_simDataCollector;

signals:
    void dataCollected_signal(int, int, uint32_t);

};

extern "C++" SIMDATACOLLECTOR_EXPORT void lockForRead();
extern "C++" SIMDATACOLLECTOR_EXPORT void lockForWrite();
extern "C++" SIMDATACOLLECTOR_EXPORT void unlock();

void CALLBACK Dispatch(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);

#endif // SIMDATACOLLECTOR_H
