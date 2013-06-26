-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:16
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.csr_subcategories
DROP TABLE IF EXISTS `csr_subcategories`;
CREATE TABLE IF NOT EXISTS `csr_subcategories` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'CSR SubCategory ID',
  `name` varchar(45) NOT NULL DEFAULT '""' COMMENT 'CSR SubCategory Name',
  `category_id` int(10) unsigned NOT NULL COMMENT 'CSR Category ID',
  `subcategory_id` int(10) unsigned NOT NULL COMMENT 'CSR SubCategory ID',
  PRIMARY KEY (`id`) USING BTREE,
  KEY `FK_subcategories_category_id` (`category_id`),
  CONSTRAINT `csr_category_id` FOREIGN KEY (`category_id`) REFERENCES `csr_categories` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=66 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.csr_subcategories: ~65 rows (approximately)
/*!40000 ALTER TABLE `csr_subcategories` DISABLE KEYS */;
INSERT INTO `csr_subcategories` (`id`, `name`, `category_id`, `subcategory_id`) VALUES
	(1, 'General', 1, 1),
	(2, 'General', 2, 1),
	(3, 'Items', 3, 1),
	(4, 'Money', 3, 2),
	(5, 'General', 4, 1),
	(6, 'General', 5, 1),
	(7, 'General', 6, 1),
	(8, 'Schematics', 6, 2),
	(9, 'General', 7, 1),
	(10, 'General', 8, 1),
	(11, 'General', 9, 1),
	(12, 'General', 10, 1),
	(13, 'General', 11, 1),
	(14, 'General', 12, 1),
	(15, 'General', 13, 1),
	(16, 'General', 14, 1),
	(17, 'General', 15, 1),
	(18, 'General', 16, 1),
	(19, 'General', 17, 1),
	(20, 'General', 18, 1),
	(21, 'General', 19, 1),
	(22, 'NPC Chat', 19, 2),
	(23, 'Architect', 20, 1),
	(24, 'Armorsmith', 20, 2),
	(25, 'Bio-Engineer', 20, 3),
	(26, 'Bounty Hunter', 20, 4),
	(27, 'Carbineer', 20, 6),
	(28, 'Chef', 20, 7),
	(29, 'Combat Medic', 20, 8),
	(30, 'Commando', 20, 9),
	(31, 'Creature Handler', 20, 10),
	(32, 'Brawler', 20, 5),
	(33, 'Dancer', 20, 11),
	(34, 'Doctor', 20, 12),
	(35, 'Droid Engineer', 20, 13),
	(36, 'Entertainer', 20, 14),
	(37, 'Fencer', 20, 15),
	(38, 'Image Designer', 20, 16),
	(39, 'Marksman', 20, 17),
	(40, 'Medic', 20, 18),
	(41, 'Musician', 20, 19),
	(42, 'Pikeman', 20, 20),
	(43, 'Pistoleer', 20, 21),
	(44, 'Politician', 20, 22),
	(45, 'Ranger', 20, 23),
	(46, 'Rifleman', 20, 24),
	(47, 'Scout', 20, 25),
	(48, 'Shipwright', 20, 26),
	(49, 'Smuggler', 20, 27),
	(50, 'Squad Leader', 20, 28),
	(51, 'Swordsman', 20, 29),
	(52, 'Tailor', 20, 30),
	(53, 'Teras Kasi Artist', 20, 31),
	(54, 'Weaponsmith', 20, 32),
	(55, 'General', 21, 33),
	(56, 'General', 22, 34),
	(57, 'General', 23, 35),
	(58, 'General', 24, 36),
	(59, 'General', 25, 37),
	(60, 'General', 26, 38),
	(61, 'General', 27, 39),
	(62, 'General', 28, 40),
	(63, 'General', 29, 41),
	(64, 'General', 30, 42),
	(65, 'General', 31, 43);
/*!40000 ALTER TABLE `csr_subcategories` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
