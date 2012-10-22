-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:17
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.item_families
DROP TABLE IF EXISTS `item_families`;
CREATE TABLE IF NOT EXISTS `item_families` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Item Family ID',
  `family_name` varchar(255) DEFAULT NULL COMMENT 'Item Family Name',
  `crate_size` int(10) unsigned NOT NULL DEFAULT '25' COMMENT 'Item Family Crate Size',
  `crate_type` int(10) unsigned NOT NULL DEFAULT '2772' COMMENT 'Item Family Crate Type',
  PRIMARY KEY (`id`),
  UNIQUE KEY `pk_item_family_name` (`family_name`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8 ROW_FORMAT=REDUNDANT COMMENT='Item Families';

-- Dumping data for table swganh_static.item_families: ~30 rows (approximately)
/*!40000 ALTER TABLE `item_families` DISABLE KEYS */;
INSERT INTO `item_families` (`id`, `family_name`, `crate_size`, `crate_type`) VALUES
	(1, 'survey_tool', 25, 2775),
	(2, 'travel_ticket', 25, 2775),
	(3, 'crafting_tool', 25, 2775),
	(4, 'crafting_station', 25, 2775),
	(5, 'furniture', 25, 2781),
	(6, 'food', 25, 2776),
	(7, 'wearable', 25, 2779),
	(8, 'manufacture schematic', 25, 2772),
	(9, 'generic', 25, 2775),
	(10, 'weapon', 25, 2782),
	(11, 'instrument', 25, 2775),
	(12, 'bug_jar', 25, 2772),
	(13, 'component', 25, 2775),
	(14, 'crafting', 25, 2772),
	(15, 'deed', 0, 0),
	(16, 'dice', 25, 2775),
	(17, 'droid', 25, 2777),
	(18, 'droid_battery', 25, 2777),
	(19, 'firework', 25, 2775),
	(20, 'fishing', 25, 2775),
	(21, 'item', 25, 2772),
	(22, 'medicine', 25, 2780),
	(23, 'mission', 25, 2772),
	(24, 'powerup', 25, 2782),
	(25, 'scout', 25, 2772),
	(26, 'ship', 25, 2772),
	(27, 'slicing', 25, 2775),
	(28, 'parrot_cage', 25, 2775),
	(29, 'hopper', 25, 2772),
	(30, 'crate', 0, 0);
/*!40000 ALTER TABLE `item_families` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
