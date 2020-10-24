ALTER TABLE `saved_variables` ADD COLUMN `lastMaintenanceDay` BIGINT(40) UNSIGNED DEFAULT 0 NOT NULL FIRST
, ADD COLUMN `nextMaintenanceDay` BIGINT(40) UNSIGNED DEFAULT 0 NOT NULL AFTER `lastMaintenanceDay`
, ADD COLUMN `MaintenanceMarker` BOOL DEFAULT 0 NOT NULL AFTER `nextMaintenanceDay`; 