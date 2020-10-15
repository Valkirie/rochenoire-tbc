-- DO THE WORK
SET sql_mode = 'NO_UNSIGNED_SUBTRACTION';
UPDATE creature_addon ca SET ca.lvar = 0;

INSERT IGNORE INTO creature_addon (guid) (SELECT guid FROM creature);

UPDATE creature c, creature_addon ca, creature_template ct, tbcaowow.dbc_areatable area
SET ca.lvar = GREATEST(LEAST((ct.MinLevel - area.areaLevel), 3), -3)
WHERE c.guid = ca.guid
AND c.id = ct.Entry
AND c.areaId = area.areaId
AND ct.CreatureType IN (1,2,3,4,5,6,7,9)
AND ct.NpcFlags = 0
AND ct.ExtraFlags = 0;

DELETE FROM creature_addon WHERE lvar = 0 AND mount = 0 AND bytes1 = 0 AND b2_0_sheath = 0 AND b2_1_flags = 0 AND emote = 0 AND moveflags = 0 AND ISNULL(auras);