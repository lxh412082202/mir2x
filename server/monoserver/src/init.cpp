/*
 * =====================================================================================
 *
 *       Filename: init.cpp
 *        Created: 04/09/2016 15:12:44
 *  Last Modified: 04/27/2016 23:30:06
 *
 *    Description: all the init work for server launching
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#include <vector>

#include "monster.hpp"
#include "monoserver.hpp"

// I have to put it here, since in actorpod.hpp I used MonoServer::AddLog()
// then in monoserver.hpp if I use monster.hpp which includes actorpod.hpp
// it won't compile
//
// and it's good for me to make monoserver.hpp to be compact by moving these
// constant variables out
static std::vector<MONSTERRACEINFO> s_MonsterRaceInfoV;

bool MonoServer::InitMonsterRace()
{
    auto pRecord = m_DBConnection->CreateDBRecord();
    if(!pRecord->Execute("select * from mir2x.tbl_monster order by fld_index")){
        AddLog(LOGTYPE_WARNING, "SQL ERROR: (%d: %s)", pRecord->ErrorID(), pRecord->ErrorInfo());
        return false;
    }

    if(pRecord->RowCount() < 1){
        AddLog(LOGTYPE_INFO, "no monster info found");
        return true;
    }

    AddLog(LOGTYPE_INFO, "starting add monster info:");

    while(pRecord->Fetch()){
        MONSTERRACEINFO stRaceInfo;
        // 1. record the monster race info
        stRaceInfo.Name        = std::string(pRecord->Get("fld_name"));
        stRaceInfo.Index       = std::atoi(pRecord->Get("fld_index"));
        stRaceInfo.Race        = std::atoi(pRecord->Get("fld_race"));
        stRaceInfo.LID         = std::atoi(pRecord->Get("fld_lid"));
        stRaceInfo.Undead      = std::atoi(pRecord->Get("fld_undead"));
        stRaceInfo.Level       = std::atoi(pRecord->Get("fld_level"));
        stRaceInfo.HP          = std::atoi(pRecord->Get("fld_hp"));
        stRaceInfo.MP          = std::atoi(pRecord->Get("fld_mp"));
        stRaceInfo.AC          = std::atoi(pRecord->Get("fld_ac"));
        stRaceInfo.MAC         = std::atoi(pRecord->Get("fld_mac"));
        stRaceInfo.DC          = std::atoi(pRecord->Get("fld_dc"));
        stRaceInfo.AttackSpead = std::atoi(pRecord->Get("fld_attackspeed"));
        stRaceInfo.WalkSpead   = std::atoi(pRecord->Get("fld_walkspeed"));
        stRaceInfo.Spead       = std::atoi(pRecord->Get("fld_speed"));
        stRaceInfo.Hit         = std::atoi(pRecord->Get("fld_hit"));
        stRaceInfo.ViewRange   = std::atoi(pRecord->Get("fld_viewrange"));
        stRaceInfo.RaceIndex   = std::atoi(pRecord->Get("fld_raceindex"));
        stRaceInfo.Exp         = std::atoi(pRecord->Get("fld_exp"));
        stRaceInfo.Escape      = std::atoi(pRecord->Get("fld_escape"));
        stRaceInfo.Water       = std::atoi(pRecord->Get("fld_water"));
        stRaceInfo.Fire        = std::atoi(pRecord->Get("fld_fire"));
        stRaceInfo.Wind        = std::atoi(pRecord->Get("fld_wind"));
        stRaceInfo.Light       = std::atoi(pRecord->Get("fld_light"));
        stRaceInfo.Earth       = std::atoi(pRecord->Get("fld_earth"));

        // 2. make a room in the global table
        s_MonsterRaceInfoV.resize(stRaceInfo.Index + 1, -1);

        // 3. store the result
        s_MonsterRaceInfoV[stRaceInfo.Index] = stRaceInfo;

        // 3. log it
        AddLog(LOGTYPE_INFO,
                "monster added, index = %d, name = %s.", stRaceInfo.Index, stRaceInfo.Name.c_str());
    }

    AddLog(LOGTYPE_INFO, "finished monster info: %d monster added", pRecord->RowCount());
    return true;
}

bool MonoServer::InitMonsterItem()
{
    auto pRecord = m_DBConnection->CreateDBRecord();
    if(!pRecord->Execute("select * from mir2x.tbl_monsteritem")){
        AddLog(LOGTYPE_WARNING, "SQL ERROR: (%d: %s)", pRecord->ErrorID(), pRecord->ErrorInfo());
        return false;
    }

    if(pRecord->RowCount() < 1){
        AddLog(LOGTYPE_INFO, "no monster item found");
        return true;
    }

    AddLog(LOGTYPE_INFO, "starting add monster item:");

    while(pRecord->Fetch()){
        MONSTERITEMINFO stItemInfo;
        // 1. get the item desc
        stItemInfo.MonsterIndex = std::atoi(pRecord->Get("fld_monster"));
        stItemInfo.Type         = std::atoi(pRecord->Get("fld_type"));
        stItemInfo.Chance       = std::atoi(pRecord->Get("fld_chance"));
        stItemInfo.Count        = std::atoi(pRecord->Get("fld_count"));
        // 2. make a room for it
        if(true 
                && stItemInfo.MonsterIndex > 0
                && stItemInfo.MonsterIndex < (int)s_MonsterRaceInfoV.size()){
            s_MonsterRaceInfoV[stItemInfo.MonsterIndex].ItemV.push_back(stItemInfo);
        }
    }
    AddLog(LOGTYPE_INFO, "finished monster item: %d added", pRecord->RowCount());
    return true;
}