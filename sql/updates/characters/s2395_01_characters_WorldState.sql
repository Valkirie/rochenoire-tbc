ALTER TABLE character_db_version CHANGE COLUMN s2394_00_characters_ilevel required_s2395_01_characters_WorldState bit;

DROP TABLE IF EXISTS world_state;
CREATE TABLE world_state(
   Id INT(11) UNSIGNED NOT NULL COMMENT 'Internal save ID',
   Data longtext,
   PRIMARY KEY(`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='WorldState save system';

