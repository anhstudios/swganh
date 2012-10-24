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

-- Dumping structure for table swganh_static.galaxy_account_types
DROP TABLE IF EXISTS `galaxy_account_types`;
CREATE TABLE IF NOT EXISTS `galaxy_account_types` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `type` char(255) NOT NULL,
  `description` char(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.galaxy_account_types: ~29 rows (approximately)
/*!40000 ALTER TABLE `galaxy_account_types` DISABLE KEYS */;
INSERT INTO `galaxy_account_types` (`id`, `type`, `description`) VALUES
	(1, 'charactercreation', 'Character Creation'),
	(2, 'newbietutorial', 'Newbie Tutorial'),
	(3, 'customerservice', 'Customer Service'),
	(4, 'missionsystemdynamic', 'Dynamic Mission System'),
	(5, 'missionsystemplayer', 'Player Mission System'),
	(6, 'bountysystem', 'Bounty Mission System'),
	(7, 'cloningsystem', 'Cloning System'),
	(8, 'insurancesystem', 'Insurance System'),
	(9, 'travelsystem', 'Galactic Travel Commission'),
	(10, 'shippingsystem', 'Galactic Shipping Commission'),
	(11, 'bazaarsystem', 'Galactic Trade Commission'),
	(12, 'dispensersystem', 'Dispenser System'),
	(13, 'skilltrainingsystem', 'Skill Training Union'),
	(14, 'rebelfaction', 'Rebellion'),
	(15, 'imperialfaction', 'the Empire'),
	(16, 'jabbaspalace', 'Jabba the Hutt'),
	(17, 'poisystem', 'POI System'),
	(18, 'corpseexpiraton', 'Corpse Expiration Tracking'),
	(19, 'testing', 'Testing'),
	(20, 'structuremaintenance', 'Structure Maintenance'),
	(21, 'tipsurcharge', 'Tip Surcharge'),
	(23, 'npcloot', 'NPC Loot'),
	(24, 'junkdealer', 'Junk Dealer'),
	(25, 'cantinadrink', 'Cantina Drink'),
	(26, 'betatest', 'Beta Test Fund'),
	(27, 'badgegroupcoinsplit', 'Group Split Error Account'),
	(28, 'gamblingslotstandard', 'Standard Slot Machine Account'),
	(29, 'gamblingroulette', 'Roulette Account'),
	(30, 'gamblingcardsabacc', 'Sabacc Account'),
	(31, 'vehiclerepairs', 'Vehicle Repair System'),
	(32, 'relicdealer', 'Relic Dealer'),
	(33, 'newplayerquests', 'New Player Quests'),
	(34, 'fines', 'Contraband Scanning Fines'),
	(35, 'bank', 'Bank');
/*!40000 ALTER TABLE `galaxy_account_types` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
