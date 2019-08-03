ALTER TABLE character_db_version CHANGE COLUMN required_s2389_00_characters_cooldown s2394_00_characters_ilevel bit;

ALTER TABLE `characters` ADD COLUMN `ilevel` TINYINT(3) UNSIGNED DEFAULT 0 NOT NULL AFTER `level`; 