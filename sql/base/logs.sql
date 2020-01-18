/*
SQLyog Ultimate v12.4.3 (64 bit)
MySQL - 8.0.17 : Database - tbclogs
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `logs_quests` */

DROP TABLE IF EXISTS `logs_quests`;

CREATE TABLE `logs_quests` (
  `account` int(11) NOT NULL COMMENT 'account guid',
  `character` int(11) NOT NULL COMMENT 'character guid',
  `plevel` tinyint(3) NOT NULL COMMENT 'player level',
  `quest_id` mediumint(8) NOT NULL COMMENT 'quest id',
  `e_timer` mediumint(8) NOT NULL COMMENT 'elapsed timer'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
