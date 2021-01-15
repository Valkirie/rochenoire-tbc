/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "world_eastern_kingdoms.h"
#include "AI/ScriptDevAI/include/sc_common.h"
#include "AI/ScriptDevAI/scripts/world/world_map_scripts.h"
#include "AI/ScriptDevAI/base/TimerAI.h"
#include "World/WorldState.h"
#include "World/WorldStateDefines.h"

#include "AI/EventAI/CreatureEventAIMgr.h" //Roche
#include "Globals/ObjectMgr.h"

/* *********************************************************
 *                  EASTERN KINGDOMS
 */
struct world_map_eastern_kingdoms : public ScriptedMap, public TimerManager
{
    world_map_eastern_kingdoms(Map* pMap) : ScriptedMap(pMap), m_shadeData({ AREAID_GOLDSHIRE, AREAID_KHARANOS, AREAID_BRILL })
    {
        AddCustomAction(EVENT_SPAWN, true, [&]
        {
            if (_spawn.size() < 16)
            {
                switch (urand(0, 4))
                {
                    case 0: DoSpawn(NPC_INVADING_FELGUARD); break;
                    case 1: DoSpawn(NPC_INVADING_VOIDWALKER); break;
                    case 2: DoSpawn(NPC_INVADING_INFERNAL); break;
                    case 3: DoSpawn(NPC_INVADING_FEL_STALKER); break;
                    case 4: DoSpawn(NPC_INVADING_ANGUISHER); break;
                }
            }
            ResetTimer(EVENT_SPAWN, urand(1000, 5000));
        });
        AddCustomAction(EVENT_SPAWN_BOSS, true, [&]
        {
            if (_spawnBoss.size() < 1)
            {
                urand(0, 1) ? ResetTimer(EVENT_SPAWN_DREADKNIGHT, urand(5000, 10000)) : ResetTimer(EVENT_SPAWN_FELGUARD_LIEUTENANT, urand(5000, 10000));
            }
            ResetTimer(EVENT_SPAWN_BOSS, urand(20000, 30000));
        });
        AddCustomAction(EVENT_SPAWN_DREADKNIGHT, true, [&]
        {
            DoSpawn(NPC_DREADKNIGHT);
            DoSpawn(NPC_INVADING_INFERNAL);
            DoSpawn(NPC_INVADING_INFERNAL);
        });
        AddCustomAction(EVENT_SPAWN_FELGUARD_LIEUTENANT, true, [&]
        {
            DoSpawn(NPC_FELGUARD_LIEUTENANT);
            DoSpawn(NPC_INVADING_FEL_STALKER);
            DoSpawn(NPC_INVADING_ANGUISHER);
        });
        AddCustomAction(EVENT_REINFORCEMENTS_NEEDED, true, [&]
        {
            if (_spawnSupport.size() < 1)
            {
                if (_spawnProtector.size() < 4 || _spawnGuardian.size() < 4)
                {
                    ResetTimer(EVENT_SPAWN_REINFORCEMENTS, 1000);
                    ResetTimer(EVENT_REINFORCEMENTS_LEADER_SPEAK, urand(45000, 90000));
                }
            }
            ResetTimer(EVENT_REINFORCEMENTS_NEEDED, urand(5000, 10000));
        });
        AddCustomAction(EVENT_SPAWN_REINFORCEMENTS, true, [&]
        {
            switch (urand(0, 2))
            {
                case 0: DoSpawn(NPC_LORD_MARSHAL_RAYNOR); break;
                case 1: DoSpawn(NPC_JUSTINIUS_THE_HARBINGER); break;
                case 2: DoSpawn(NPC_MELGROMM_HIGHMOUNTAIN); break;
            }
        });
        AddCustomAction(EVENT_REINFORCEMENTS_LEADER_SPEAK, true, [&]
        {
            if (_spawnSupport.size() > 0)
            {
                uint32 randomScriptTextId = 0;

                for (std::set<ObjectGuid>::iterator it = _spawnSupport.begin(); it != _spawnSupport.end(); ++it)
                {
                    if (Creature* add = instance->GetCreature(*it))
                    {
                        switch (add->GetEntry())
                        {
                            case NPC_LORD_MARSHAL_RAYNOR:
                                switch (urand(0, 4))
                                {
                                    case 0: randomScriptTextId = LMR_RANDOM_TEXT_1; break;
                                    case 1: randomScriptTextId = LMR_RANDOM_TEXT_2; break;
                                    case 2: randomScriptTextId = LMR_RANDOM_TEXT_3; break;
                                    case 3: randomScriptTextId = LMR_RANDOM_TEXT_4; break;
                                    case 4: randomScriptTextId = LMR_RANDOM_TEXT_5; break;
                                }
                                break;
                            case NPC_JUSTINIUS_THE_HARBINGER:
                                switch (urand(0, 3))
                                {
                                    case 0: randomScriptTextId = JTH_RANDOM_TEXT_1; break;
                                    case 1: randomScriptTextId = JTH_RANDOM_TEXT_2; break;
                                    case 2: randomScriptTextId = JTH_RANDOM_TEXT_3; break;
                                }
                                break;
                            case NPC_MELGROMM_HIGHMOUNTAIN:
                                switch (urand(0, 5))
                                {
                                    case 0: randomScriptTextId = MH_RANDOM_TEXT_1; break;
                                    case 1: randomScriptTextId = MH_RANDOM_TEXT_2; break;
                                    case 2: randomScriptTextId = MH_RANDOM_TEXT_3; break;
                                    case 3: randomScriptTextId = MH_RANDOM_TEXT_4; break;
                                    case 4: randomScriptTextId = MH_RANDOM_TEXT_5; break;
                                    case 5: randomScriptTextId = MH_RANDOM_TEXT_6; break;
                                }
                                break;
                        }
                        DoScriptText(randomScriptTextId, add);
                    }
                }
            }
            ResetTimer(EVENT_REINFORCEMENTS_LEADER_SPEAK, urand(60000, 90000));
        });
        Initialize();
    }

    std::set<ObjectGuid> _spawn;
    std::set<ObjectGuid> _spawnBoss;
    std::set<ObjectGuid> _spawnSupport;
    std::set<ObjectGuid> _spawnProtector;
    std::set<ObjectGuid> _spawnGuardian;

    Position const spawnPortalPos = { -11900.3f, -3207.62f, -14.7534f, 0.146405f };
    Position const spawnPortalPos1 = { -11901.25f, -3202.272f, -14.7534f, 0.146405f };
    Position const spawnBossPortalPos = { -11891.500f, -3207.010f, -14.798f, 0.146405f };
    Position const spawnReinforcementPos = { -11815.1f, -3190.39f, -30.7338f, 3.32447f };

    // Shade of the Horseman village attack event
    ShadeOfTheHorsemanData m_shadeData;


    // A récupérer depuis une base de données :
    static const uint16 NBRareChallenge = 6;
    std::array<uint32, NBRareChallenge> RC_ENTRYList =
    {
       99, 471, 79, 61, 472, 100 //Elwin forest
    };

    
    // A récupérer depuis une base de donnée : 
    static const uint16 NBRC_Script_HUMANOID_Warrior = 1;
    uint32 RC_ScriptID_HUMANOID_Warrior[NBRC_Script_HUMANOID_Warrior] =
    {
       99,
    };

    static const uint16 NBRC_Script_HUMANOID_Mage = 1;
    uint32 RC_ScriptID_HUMANOID_Mage[NBRC_Script_HUMANOID_Mage] =
    {
       99,
    };


    static const uint16 NBRC_Script_BEAST = 1;
    uint32 RC_ScriptID_BEAST[NBRC_Script_BEAST] =
    {
       99,
    };

    std::list <uint32> RC_ChooseList;
    uint32 RC_Count = 0;
    uint32 RC_Despawn = 0;
    uint32 m_uiRCTimer;
    const uint32 m_uiRCTimerCheck = 10 * IN_MILLISECONDS;  // Timer to check Rare Challenge

    uint32 m_uiRCTimerReset;
    const uint32 m_uiRCTimerResetCheck = 3 * HOUR * IN_MILLISECONDS;  //Should be between 1 hour and 12 hours. // Timer to reset Rare challenge and shuffle a new set of rare


    void Initialize() override
    {
        m_shadeData.Reset();

        RC_Count = 0; //RareChallenge
        RC_Despawn = 0;
        m_uiRCTimer = 6 * IN_MILLISECONDS;  //Timer after loading ? need to be shorter ?
        m_uiRCTimerReset = 2 * IN_MILLISECONDS; //Timer after loading ? need to be shorter ?
    }

    bool CheckConditionCriteriaMeet(Player const* player, uint32 instanceConditionId, WorldObject const* conditionSource, uint32 conditionSourceType) const override
    {
        if (instanceConditionId >= INSTANCE_CONDITION_ID_FIRE_BRIGADE_PRACTICE_GOLDSHIRE && instanceConditionId <= INSTANCE_CONDITION_ID_LET_THE_FIRES_COME_HORDE)
            return m_shadeData.IsConditionFulfilled(instanceConditionId, player->GetAreaId());

        script_error_log("world_map_eastern_kingdoms::CheckConditionCriteriaMeet called with unsupported Id %u. Called with param plr %s, src %s, condition source type %u",
            instanceConditionId, player ? player->GetGuidStr().c_str() : "nullptr", conditionSource ? conditionSource->GetGuidStr().c_str() : "nullptr", conditionSourceType);
        return false;
    }

    void FillInitialWorldStates(ByteBuffer& data, uint32& count, uint32 /*zoneId*/, uint32 areaId) override
    {
        switch (areaId)
        {
            case AREAID_GOLDSHIRE:
            case AREAID_KHARANOS:
            case AREAID_BRILL:
            {
                FillInitialWorldStateData(data, count, WORLD_STATE_SHADE_OF_THE_HORSEMAN_TIMER, m_shadeData.CalculateWorldstateTimerValue());
                break;
            }
            default: break;
        }
    }

    void OnCreatureCreate(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_NEZRAZ:
            case NPC_HINDENBURG:
            case NPC_ZAPETTA:
            case NPC_SQUIBBY_OVERSPECK:
            case NPC_JONATHAN:
            case NPC_WRYNN:
            case NPC_BOLVAR:
            case NPC_PRESTOR:
            case NPC_WINDSOR:
            case NPC_HIGHLORD_KRUUL:
            case NPC_AGENT_PROUDWELL:
            case NPC_FALSTAD_WILDHAMMER:
                m_npcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
                break;
            case NPC_DREADKNIGHT:
            case NPC_FELGUARD_LIEUTENANT:
                _spawnBoss.insert(pCreature->GetObjectGuid());
                break;
            case NPC_LORD_MARSHAL_RAYNOR:
            case NPC_JUSTINIUS_THE_HARBINGER:
            case NPC_MELGROMM_HIGHMOUNTAIN:
                _spawnSupport.insert(pCreature->GetObjectGuid());
                break;
            case NPC_INVADING_INFERNAL:
            case NPC_INVADING_FELGUARD:
            case NPC_INVADING_FEL_STALKER:
            case NPC_INVADING_VOIDWALKER:
            case NPC_INVADING_ANGUISHER:
                _spawn.insert(pCreature->GetObjectGuid());
                break;
            case NPC_ARGENT_GUARDIAN:
                _spawnGuardian.insert(pCreature->GetObjectGuid());
                break;
            case NPC_ARGENT_PROTECTOR:
                _spawnProtector.insert(pCreature->GetObjectGuid());
                break;
            case NPC_MASKED_ORPHAN_MATRON:
                m_npcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
                break;
            case NPC_COSTUMED_ORPHAN_MATRON:
                m_npcEntryGuidCollection[pCreature->GetEntry()].push_back(pCreature->GetObjectGuid());
                break;
        }

        switch (IsInRC_ENTRY(pCreature->GetEntry())) //RareChallenge
        {
        case CODE_RARE:
            m_npcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
        case CODE_RARECHALLENGE:
            m_npcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            pCreature->ForcedDespawn();
            pCreature->setRareChallenge(true);
            break;
        }


    }

    uint8 IsInRC_ENTRY(uint32 Entry)
    {
        //for (std::vector<uint32>::iterator RcEntry = RC_ENTRYList.begin(); RcEntry != RC_ENTRYList.end(); RcEntry++)
        for (auto& RcEntry : RC_ENTRYList)
        {
            if (Entry == RcEntry)
                return CODE_RARE;

            if (Entry == RcEntry + START_ID_RC)
                return CODE_RARECHALLENGE;

        }
        return CODE_NULL;

    }


    void OnCreatureDeath(Creature* pCreature) override
    {
        switch (pCreature->GetEntry())
        {
            case NPC_DREADKNIGHT:
            case NPC_FELGUARD_LIEUTENANT:
                _spawnBoss.erase(pCreature->GetObjectGuid());
                break;
            case NPC_LORD_MARSHAL_RAYNOR:
            case NPC_JUSTINIUS_THE_HARBINGER:
            case NPC_MELGROMM_HIGHMOUNTAIN:
                _spawnSupport.erase(pCreature->GetObjectGuid());
                break;
            case NPC_ARGENT_GUARDIAN:
                _spawnGuardian.erase(pCreature->GetObjectGuid());
                break;
            case NPC_ARGENT_PROTECTOR:
                _spawnProtector.erase(pCreature->GetObjectGuid());
                break;
            default:
                _spawn.erase(pCreature->GetObjectGuid());
                break;
        }

        switch (IsInRC_ENTRY(pCreature->GetEntry())) //RareChallenge
        {
        case CODE_RARECHALLENGE:
            RC_ChooseList.remove(pCreature->GetEntry());
            pCreature->GetMap()->m_RareChallengeStore.erase(pCreature->GetEntry());
            //m_RareChallengeStore[pCreature->GetEntry()].PlayersWithinRadius.clear();
            break;
        }



    }

    void OnCreatureRespawn(Creature* pCreature) override
    {

        if ((std::find(RC_ChooseList.begin(), RC_ChooseList.end(), pCreature->GetEntry()) != RC_ChooseList.end()))
        {
            SetRareChalengeCreature(pCreature->GetEntry());
        }
    }

    void DespawnAdds()
    {
        for (std::set<ObjectGuid>::iterator it = _spawn.begin(); it != _spawn.end(); ++it)
            if (Creature* add = instance->GetCreature(*it))
                add->AddObjectToRemoveList();
        _spawn.clear();

        for (std::set<ObjectGuid>::iterator it = _spawnSupport.begin(); it != _spawnSupport.end(); ++it)
            if (Creature* add = instance->GetCreature(*it))
                add->AddObjectToRemoveList();
        _spawnSupport.clear();

        for (std::set<ObjectGuid>::iterator it = _spawnBoss.begin(); it != _spawnBoss.end(); ++it)
            if (Creature* add = instance->GetCreature(*it))
                add->AddObjectToRemoveList();
        _spawnBoss.clear();
    }

    void DoSpawn(uint32 spawnId)
    {
        Position spawnPos;
        uint32 spawnScriptTextId = 0;
        uint32 formationCreatureEntry = 0;
        uint32 despawnTimer = 0;
        TempSpawnType spawnType = TEMPSPAWN_DEAD_DESPAWN;

        switch (spawnId)
        {
            case NPC_DREADKNIGHT:
            case NPC_FELGUARD_LIEUTENANT:
                spawnPos = spawnBossPortalPos;
                break;
            case NPC_LORD_MARSHAL_RAYNOR:
                spawnPos = spawnReinforcementPos;
                despawnTimer = 3 * MINUTE * IN_MILLISECONDS;
                spawnType = TEMPSPAWN_TIMED_OR_DEAD_DESPAWN;
                formationCreatureEntry = NPC_STORMWIND_MARSHAL;
                spawnScriptTextId = LMR_SPAWN;
                break;
            case NPC_JUSTINIUS_THE_HARBINGER:
                spawnPos = spawnReinforcementPos;
                despawnTimer = 3 * MINUTE * IN_MILLISECONDS;
                spawnType = TEMPSPAWN_TIMED_OR_DEAD_DESPAWN;
                formationCreatureEntry = NPC_AZUREMYST_VINDICATOR;
                spawnScriptTextId = JTH_SPAWN;
                break;
            case NPC_MELGROMM_HIGHMOUNTAIN:
                spawnPos = spawnReinforcementPos;
                despawnTimer = 3 * MINUTE * IN_MILLISECONDS;
                spawnType = TEMPSPAWN_TIMED_OR_DEAD_DESPAWN;
                formationCreatureEntry = NPC_THUNDER_BLUFF_HUNTSMAN;
                spawnScriptTextId = MH_SPAWN;
                break;
            default:
                spawnPos = urand(0, 1) ? spawnPortalPos : spawnPortalPos1;
                break;
        }

        if (Creature* pProudwell = GetSingleCreatureFromStorage(NPC_AGENT_PROUDWELL, true))
        {
            if (Creature* summon = pProudwell->SummonCreature(spawnId, spawnPos.x, spawnPos.y, spawnPos.z, spawnPos.o, spawnType, despawnTimer))
            {
                if (spawnScriptTextId)
                    DoScriptText(spawnScriptTextId, summon);

                if (formationCreatureEntry)
                {
                    if (Creature* add = summon->SummonCreature(formationCreatureEntry, summon->GetPositionX(), summon->GetPositionY(), summon->GetPositionZ(), 0.0f, spawnType, despawnTimer))
                        add->GetMotionMaster()->MoveFollow(summon, 5.f, 220.f * float(M_PI) / 180.0f, true);

                    if (Creature* add = summon->SummonCreature(formationCreatureEntry, summon->GetPositionX(), summon->GetPositionY(), summon->GetPositionZ(), 0.0f, spawnType, despawnTimer))
                        add->GetMotionMaster()->MoveFollow(summon, 5.f, 140.f * float(M_PI) / 180.0f, true);
                }
            }
        }
    }

    void OnEventHappened(uint16 event_id, bool activate, bool resume) override
    {
        if (event_id != GAME_EVENT_BEFORE_THE_STORM)
            return;

        DespawnAdds();

        if (activate && !resume)
        {
            ResetTimer(EVENT_SPAWN, 1000);
            ResetTimer(EVENT_SPAWN_BOSS, 30000);
            ResetTimer(EVENT_REINFORCEMENTS_NEEDED, 30000);
        }
        else
        {
            ResetAllTimers();
        }
    }

    void Update(uint32 diff) override
    {
        UpdateTimers(diff);

        if (m_shadeData.Update(diff))
        {
            // Brill
            if (Creature* matron = instance->GetCreature(m_npcEntryGuidStore[NPC_MASKED_ORPHAN_MATRON]))
                matron->AI()->SendAIEvent(AI_EVENT_CUSTOM_A, matron, matron);
            // Goldshire & Kharanos
            for (ObjectGuid guid : m_npcEntryGuidCollection[NPC_COSTUMED_ORPHAN_MATRON])
                if (Creature* matron = instance->GetCreature(guid))
                    matron->AI()->SendAIEvent(AI_EVENT_CUSTOM_A, matron, matron);
        }


        UpdateRareChallenge(diff);

    }

    void UpdateRareChallenge(uint32 diff)
    {
        if (m_uiRCTimer > m_uiRCTimerCheck)
            m_uiRCTimer = m_uiRCTimerCheck;

        if (m_uiRCTimerReset > m_uiRCTimerResetCheck)
            m_uiRCTimerReset = m_uiRCTimerResetCheck;


        if (m_uiRCTimer < diff)
        {
            m_uiRCTimer = m_uiRCTimerCheck;
            while (RC_Count < NB_MAX_RC)
            {
                uint32 RandEntry = rand() % NBRareChallenge;
                uint32 EntryChoosen = RC_ENTRYList[RandEntry];
                if (!(std::find(RC_ChooseList.begin(), RC_ChooseList.end(), EntryChoosen) != RC_ChooseList.end()))
                {
                    RC_ChooseList.push_back(EntryChoosen);
                    SetRareChalengeCreature(EntryChoosen);
                    RC_Count++;
                }
            }
        }
        else
        {
            m_uiRCTimer -= diff;
        }




        if (m_uiRCTimerReset < diff)
        {
            std::list <uint32> RC_ChooseListTemp = RC_ChooseList;

            for (uint32 RCChoosenEntry : RC_ChooseListTemp)
            {
                if (Creature* pCreature = GetSingleCreatureFromStorage(GetNewRCEntry(RCChoosenEntry)))
                {
                    if (!(pCreature->IsInCombat() || pCreature->IsNoLoot()))
                    {
                        if (pCreature->IsAlive())
                        {
                            if (Creature* pCreatureOri = GetSingleCreatureFromStorage(RCChoosenEntry))
                            {
                                pCreatureOri->Respawn();
                            }
                        }

                        pCreature->ForcedDespawn();
                        RC_Despawn++;
                        RC_ChooseList.remove(RCChoosenEntry);

                    }
                }
                else
                {
                    RC_Despawn++;
                    RC_ChooseList.remove(RCChoosenEntry);
                }
            }

            if (RC_Despawn >= NB_MAX_RC)  //Work done
            {
                m_uiRCTimerReset = m_uiRCTimerResetCheck;
                RC_ChooseList.clear();
                RC_Despawn = 0;
                RC_Count = 0;
            }
            else
            {
                m_uiRCTimerReset = m_uiRCTimerCheck;  //If work not done, we add a short timer.
            }
        }
        else
        {
            m_uiRCTimerReset -= diff;
        }

    }

    uint32 GetData(uint32 type) const override
    {
        if (type >= TYPE_SHADE_OF_THE_HORSEMAN_ATTACK_PHASE && type <= TYPE_SHADE_OF_THE_HORSEMAN_MAX)
            return m_shadeData.HandleGetData(type);
        return 0;
    }

    void SetData(uint32 type, uint32 data) override
    {
        if (type >= TYPE_SHADE_OF_THE_HORSEMAN_ATTACK_PHASE && type <= TYPE_SHADE_OF_THE_HORSEMAN_MAX)
            return m_shadeData.HandleSetData(type, data);
    }



    uint32 GetNewRCEntry(uint32 Entry)
    {
        return Entry + START_ID_RC;
    }

    uint32 GetScriptID(Creature* Crea)
    {
        uint32 Class = Crea->GetCreatureInfo()->UnitClass;
        uint32 Type = Crea->GetCreatureInfo()->CreatureType;

        uint32 RandEntry = 0; // rand() % NBRC_Script;
        uint32 EntryChoosen = 0; // RC_ScriptID[RandEntry];

        if (Type == 7) //Humanoïd
        {
            if (Class == 1) //Warrior
            {
                RandEntry = rand() % NBRC_Script_HUMANOID_Warrior;
                return RC_ScriptID_HUMANOID_Warrior[RandEntry];
            }
            else
            {
                RandEntry = rand() % NBRC_Script_HUMANOID_Mage;
                return RC_ScriptID_HUMANOID_Mage[RandEntry];

            }

        }
        else
        {
            RandEntry = rand() % NBRC_Script_BEAST;
            return RC_ScriptID_BEAST[RandEntry];
        }
        
        return 0;
    }

    Creature* SetRareChalengeCreature(uint32 uiCreatureEntry)
    {
        uint32 uiRC_Entry = GetNewRCEntry(uiCreatureEntry);
        if (Creature* pRCCreature = GetSingleCreatureFromStorage(uiRC_Entry))
        {
            if (pRCCreature->IsDespawned())
            {
                if (Creature* pCreature = GetSingleCreatureFromStorage(uiCreatureEntry))
                {
                    if (!(pCreature->IsDespawned() || pCreature->IsInCombat() || pCreature->IsNoLoot()))
                    {

                        pCreature->ForcedDespawn();
                        pRCCreature->ForcedDespawn();
                        uint32 ui_ScriptID = GetScriptID(pRCCreature);
                        sEventAIMgr.LoadCreatureEventAI_Scripts(uiRC_Entry, ui_ScriptID);
                        pRCCreature->AIM_Initialize();

                        pRCCreature->setRareChallenge(true);

                        RareProximity ThisRare;
                        ThisRare.x = pRCCreature->GetPositionX();
                        ThisRare.y = pRCCreature->GetPositionY();
                        ThisRare.Name = pRCCreature->GetName();

                        if (rand() % 3)
                        {
                            // Free for all (pvp global) with +1 level rare
                            pRCCreature->setRareChallengeLevel(1);  
                            ThisRare.RareFlag = RARE_FLAG_FFA;

                        }
                        else
                        {
                            // standard pvp zone with +3 level
                            pRCCreature->setRareChallengeLevel(3);
                            ThisRare.RareFlag = RARE_FLAG_PVP;

                        }

                        //CreatureEventAI::CreatureEventAI(pRCCreature, uiCreatureEntry);
                       
                        pRCCreature->GetMap()->m_RareChallengeStore[uiRC_Entry] = ThisRare;
                        pRCCreature->Respawn();

                        return pRCCreature;


                    }
                }
            }
        }

        return nullptr;

    }



};

struct go_infernaling_summoner_portal_hound : public GameObjectAI
{
    go_infernaling_summoner_portal_hound(GameObject* go) : GameObjectAI(go)
    {
        m_uiSummonTimer = 15000;
    }

    uint32 m_uiSummonTimer;

    void UpdateAI(const uint32 uiDiff) override
    {
        if (!m_go->IsSpawned())
            return;

        if (m_uiSummonTimer <= uiDiff)
        {
            if (Creature* pProudwell = ((ScriptedInstance*)m_go->GetMap()->GetInstanceData())->GetSingleCreatureFromStorage(NPC_AGENT_PROUDWELL))
            {
                pProudwell->CastSpell(pProudwell, SPELL_SUMMON_INFERNALING_PORTAL_HOUND, TRIGGERED_OLD_TRIGGERED, nullptr, nullptr, m_go->GetObjectGuid());
            }
            m_uiSummonTimer = urand(60000, 120000);
        }
        else
            m_uiSummonTimer -= uiDiff;
    }
};

GameObjectAI* GetAI_go_infernaling_summoner_portal_hound(GameObject* go)
{
    return new go_infernaling_summoner_portal_hound(go);
}

// does not work currently - need to handle spell 33710 somehow when it hits/activates either object 183358 or 183357?
bool GOUse_go_infernaling_summoner_portal_hound(Player* /*pPlayer*/, GameObject* pGo)
{
    ScriptedInstance* pInstance = (ScriptedInstance*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    if (Creature* pProudwell = pInstance->GetSingleCreatureFromStorage(NPC_AGENT_PROUDWELL))
    {
        pProudwell->CastSpell(pProudwell, SPELL_SUMMON_INFERNALING_PORTAL_HOUND, TRIGGERED_OLD_TRIGGERED, nullptr, nullptr, pGo->GetObjectGuid());
    }

    return false;
}

struct npc_agent_proudwell : public ScriptedAI
{
    npc_agent_proudwell(Creature* pCreature) : ScriptedAI(pCreature) { }

    void Reset() override {}

    void SummonedCreatureDespawn(Creature* pSummoned) override
    {
        uint32 despawnScriptTextId = 0;
        switch (pSummoned->GetEntry())
        {
            case NPC_LORD_MARSHAL_RAYNOR: despawnScriptTextId = LMR_DESPAWN; break;
            case NPC_JUSTINIUS_THE_HARBINGER: despawnScriptTextId = JTH_DESPAWN; break;
            case NPC_MELGROMM_HIGHMOUNTAIN: despawnScriptTextId = MH_DESPAWN; break;
        }

        world_map_eastern_kingdoms* pInstance = (world_map_eastern_kingdoms*)pSummoned->GetInstanceData();

        if (pInstance)
            pInstance->_spawnSupport.erase(pSummoned->GetObjectGuid());

        if (despawnScriptTextId && pSummoned->IsAlive())
            DoScriptText(despawnScriptTextId, pSummoned);
    }
};

UnitAI* GetAI_npc_agent_proudwell(Creature* pCreature)
{
    return new npc_agent_proudwell(pCreature);
}

InstanceData* GetInstanceData_world_map_eastern_kingdoms(Map* pMap)
{
    return new world_map_eastern_kingdoms(pMap);
}

void AddSC_world_eastern_kingdoms()
{
    Script* pNewScript = new Script;
    pNewScript->Name = "world_map_eastern_kingdoms";
    pNewScript->GetInstanceData = &GetInstanceData_world_map_eastern_kingdoms;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_infernaling_summoner_portal_hound";
    pNewScript->GetGameObjectAI = &GetAI_go_infernaling_summoner_portal_hound;
    pNewScript->pGOUse = &GOUse_go_infernaling_summoner_portal_hound;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_agent_proudwell";
    pNewScript->GetAI = &GetAI_npc_agent_proudwell;
    pNewScript->RegisterSelf();
}
