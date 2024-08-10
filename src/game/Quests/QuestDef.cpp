/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
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

#include "Quests/QuestDef.h"
#include "Entities/Player.h"
#include "World/World.h"

Quest::Quest(Field* questRecord)
{
    QuestId = questRecord[0].GetUInt32();
	Patch = questRecord[1].GetUInt8();
    QuestMethod = questRecord[2].GetUInt32();
    ZoneOrSort = questRecord[3].GetInt32();
    MinLevel = questRecord[4].GetUInt32();
    QuestLevel = questRecord[5].GetInt32();
    Type = questRecord[6].GetUInt32();
    RequiredClasses = questRecord[7].GetUInt32();
    RequiredRaces = questRecord[8].GetUInt32();
    RequiredSkill = questRecord[9].GetUInt32();
    RequiredSkillValue = questRecord[10].GetUInt32();
    RepObjectiveFaction = questRecord[11].GetUInt32();
    RepObjectiveValue = questRecord[12].GetInt32();
    RequiredMinRepFaction = questRecord[13].GetUInt32();
    RequiredMinRepValue = questRecord[14].GetInt32();
    RequiredMaxRepFaction = questRecord[15].GetUInt32();
    RequiredMaxRepValue = questRecord[16].GetInt32();
    SuggestedPlayers = questRecord[17].GetUInt32();
    LimitTime = questRecord[18].GetUInt32();
    m_QuestFlags = questRecord[19].GetUInt16();
    m_SpecialFlags = questRecord[20].GetUInt16();
    CharTitleId = questRecord[21].GetUInt32();
    PrevQuestId = questRecord[22].GetInt32();
    NextQuestId = questRecord[23].GetInt32();
    ExclusiveGroup = questRecord[24].GetInt32();
    NextQuestInChain = questRecord[25].GetUInt32();
    SrcItemId = questRecord[26].GetUInt32();
    SrcItemCount = questRecord[27].GetUInt32();
    SrcSpell = questRecord[28].GetUInt32();
    Title = questRecord[29].GetCppString();
    Details = questRecord[30].GetCppString();
    Objectives = questRecord[31].GetCppString();
    OfferRewardText = questRecord[32].GetCppString();
    RequestItemsText = questRecord[33].GetCppString();
    EndText = questRecord[34].GetCppString();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ObjectiveText[i] = questRecord[35 + i].GetCppString();

    for (int i = 0; i < QUEST_ITEM_OBJECTIVES_COUNT; ++i)
        ReqItemId[i] = questRecord[39 + i].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ReqItemCount[i] = questRecord[43 + i].GetUInt32();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        ReqSourceId[i] = questRecord[47 + i].GetUInt32();

    for (int i = 0; i < QUEST_SOURCE_ITEM_IDS_COUNT; ++i)
        ReqSourceCount[i] = questRecord[51 + i].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ReqCreatureOrGOId[i] = questRecord[55 + i].GetInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ReqCreatureOrGOCount[i] = questRecord[59 + i].GetUInt32();

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
        ReqSpell[i] = questRecord[63 + i].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewChoiceItemId[i] = questRecord[67 + i].GetUInt32();

    for (int i = 0; i < QUEST_REWARD_CHOICES_COUNT; ++i)
        RewChoiceItemCount[i] = questRecord[73 + i].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewItemId[i] = questRecord[79 + i].GetUInt32();

    for (int i = 0; i < QUEST_REWARDS_COUNT; ++i)
        RewItemCount[i] = questRecord[83 + i].GetUInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewRepFaction[i] = questRecord[87 + i].GetUInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewRepValue[i] = questRecord[92 + i].GetInt32();

    RewHonorableKills = questRecord[97].GetUInt32();
    RewOrReqMoney = questRecord[98].GetInt32();
    RewMoneyMaxLevel = questRecord[99].GetUInt32();
    RewSpell = questRecord[100].GetUInt32();
    RewSpellCast = questRecord[101].GetUInt32();
    RewMailTemplateId = questRecord[102].GetUInt32();
    RewMailDelaySecs = questRecord[103].GetUInt32();
    PointMapId = questRecord[104].GetUInt32();
    PointX = questRecord[105].GetFloat();
    PointY = questRecord[106].GetFloat();
    PointOpt = questRecord[107].GetUInt32();

    m_detailsemotecount = 0;
    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
    {
        DetailsEmote[i] = questRecord[108 + i].GetUInt32();
        if (DetailsEmote[i] != 0)
            m_detailsemotecount = i + 1;
    }

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        DetailsEmoteDelay[i] = questRecord[112 + i].GetUInt32();

    IncompleteEmote = questRecord[116].GetUInt32();
	IncompleteEmoteDelay = questRecord[117].GetUInt32();
    CompleteEmote = questRecord[118].GetUInt32();
	CompleteEmoteDelay = questRecord[119].GetUInt32();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmote[i] = questRecord[120 + i].GetInt32();

    for (int i = 0; i < QUEST_EMOTE_COUNT; ++i)
        OfferRewardEmoteDelay[i] = questRecord[124 + i].GetInt32();

    QuestStartScript = questRecord[128].GetUInt32();
    QuestCompleteScript = questRecord[129].GetUInt32();

    for (int i = 0; i < QUEST_REPUTATIONS_COUNT; ++i)
        RewMaxRepValue[i] = questRecord[130 + i].GetInt32();

    RequiredCondition = questRecord[135].GetUInt32();

    m_isActive = true;

    m_reqitemscount = 0;
    m_reqCreatureOrGOcount = 0;
    m_rewitemscount = 0;
    m_rewchoiceitemscount = 0;

    for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
    {
        if (ReqItemId[i])
            ++m_reqitemscount;

        if (ReqCreatureOrGOId[i])
            ++m_reqCreatureOrGOcount;
    }

    for (unsigned int i : RewItemId)
    {
        if (i)
            ++m_rewitemscount;
    }

    for (unsigned int i : RewChoiceItemId)
    {
        if (i)
            ++m_rewchoiceitemscount;
    }
}

uint32 Quest::XPValue(Player* pPlayer) const
{
    if (pPlayer)
    {
		uint32 RewMoneyMaxLevelS = GetRewMoneyMaxLevel(pPlayer);
		if (RewMoneyMaxLevelS > 0)
        {
            uint32 pLevel = pPlayer->GetLevel();
            uint32 qLevel_old = QuestLevel > 0 ? (uint32)QuestLevel : 0;
            uint32 qLevel = pPlayer->GetQuestLevelForPlayer(this);
            float fullxp = 0;
            if (qLevel_old >= 65)
                fullxp = RewMoneyMaxLevelS / 6.0f;
            else if (qLevel_old == 64)
                fullxp = RewMoneyMaxLevelS / 4.8f;
            else if (qLevel_old == 63)
                fullxp = RewMoneyMaxLevelS / 3.6f;
            else if (qLevel_old == 62)
                fullxp = RewMoneyMaxLevelS / 2.4f;
            else if (qLevel_old == 61)
                fullxp = RewMoneyMaxLevelS / 1.2f;
            else if (qLevel_old > 0 && qLevel_old <= 60)
                fullxp = RewMoneyMaxLevelS / 0.6f;

            if (pLevel <= qLevel +  5)
                return uint32(ceilf(fullxp));
            else if (pLevel == qLevel +  6)
                return uint32(ceilf(fullxp * 0.8f));
            else if (pLevel == qLevel +  7)
                return uint32(ceilf(fullxp * 0.6f));
            else if (pLevel == qLevel +  8)
                return uint32(ceilf(fullxp * 0.4f));
            else if (pLevel == qLevel +  9)
                return uint32(ceilf(fullxp * 0.2f));
            else
                return uint32(ceilf(fullxp * 0.1f));
        }
    }

    return 0;
}

int32 Quest::GetRewOrReqMoney(Player *pPlayer) const
{
	int32 sRewOrReqMoney = RewOrReqMoney;
	if (pPlayer && GetQuestRelativeLevel())
	{
		uint32 pQuest_slevel = pPlayer->GetQuestLevelForPlayer(this);

		int32 expected_RewOrReqMoney = int32(0.0472 * pow(QuestLevel, 3) + 1.894 * pow(QuestLevel, 2) + 8.6352 * (QuestLevel));
        float difference_RewOrReqMoney = (float)RewOrReqMoney / (float)expected_RewOrReqMoney;
		int32 leveled_RewOrReqMoney = int32(0.0472 * pow(pQuest_slevel, 3) + 1.894 * pow(pQuest_slevel, 2) + 8.6352 * (pQuest_slevel));

		sRewOrReqMoney = difference_RewOrReqMoney * (float)leveled_RewOrReqMoney;
	}
    else if(RewOrReqMoney <= 0)
        return RewOrReqMoney;

	return sRewOrReqMoney * sWorld.getConfig(CONFIG_FLOAT_RATE_DROP_MONEY);
}

uint32 Quest::GetRewMoneyMaxLevel(Player *pPlayer) const
{
    if (HasQuestFlag(QUEST_FLAGS_NO_MONEY_FROM_XP))
        return 0;
		
    if (pPlayer && GetQuestRelativeLevel())
	{
        uint32 pQuest_slevel = pPlayer->GetQuestLevelForPlayer(this);

		uint32 expected_RewMoneyMaxLevel = uint32(0.0212 * pow(QuestLevel, 3) - 0.3874 * pow(QuestLevel, 2) + 32.809 * (QuestLevel));
		float difference_RewMoneyMaxLevel = (float)RewMoneyMaxLevel / (float)expected_RewMoneyMaxLevel;
		uint32 leveled_RewMoneyMaxLevel = uint32(0.0212 * pow(pQuest_slevel, 3) - 0.3874 * pow(pQuest_slevel, 2) + 32.809 * (pQuest_slevel));

		double scaled_RewMoneyMaxLevel = difference_RewMoneyMaxLevel * (float)leveled_RewMoneyMaxLevel;

		return uint32(scaled_RewMoneyMaxLevel);
	}
	return RewMoneyMaxLevel;
}

bool Quest::IsAllowedInRaid() const
{
    if (Type == QUEST_TYPE_RAID)
        return true;

    return sWorld.getConfig(CONFIG_BOOL_QUEST_IGNORE_RAID);
}

uint32 Quest::GetCharTitleBitIndex() const
{
    if (!CharTitleId)
        return 0;
    CharTitlesEntry const* titleEntry = sCharTitlesStore.LookupEntry(CharTitleId);
    return titleEntry ? titleEntry->bit_index : 0;
}

int32 Quest::GetQuestRelativeLevel() const
{
    if (QuestLevel <= 0 || MinLevel <= 0 || QuestLevel < MinLevel)
        return 0;

    return uint32(QuestLevel - MinLevel);
}
