-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:01
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.xp_type
DROP TABLE IF EXISTS `xp_type`;
CREATE TABLE IF NOT EXISTS `xp_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `label` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `cap` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=50 DEFAULT CHARSET=utf8;

-- Dumping data for table galaxy.xp_type: ~49 rows (approximately)
/*!40000 ALTER TABLE `xp_type` DISABLE KEYS */;
INSERT INTO `xp_type` (`id`, `label`, `name`, `cap`) VALUES
	(1, 'apprenticeship', 'Apprenticeship', 1240),
	(2, 'crafting_bio_engineer_creature', 'Bio-Engineer Crafting', 20000),
	(3, 'bio_engineer_dna_harvesting', 'DNA Sampling', 20000),
	(4, 'bountyhunter', 'Bounty Hunter', 10000),
	(5, 'camp', 'Wilderness Survival', 2000),
	(6, 'combat_general', 'Combat', 170000),
	(7, 'combat_meleespecialize_onehandlightsaber', 'Onehanded Lightsaber', 3000),
	(8, 'combat_meleespecialize_twohandlightsaber', 'Twohanded Lightsaber', 200000),
	(9, 'combat_meleespecialize_polearmlightsaber', 'Polearm Lightsaber', 200000),
	(10, 'jedi_general', 'Jedi', 90000),
	(11, 'combat_meleespecialize_onehand', 'Onehanded Weapons', 2000),
	(12, 'combat_meleespecialize_polearm', 'Polearm Weapons', 2000),
	(13, 'combat_meleespecialize_twohand', 'Twohanded Weapons', 2000),
	(14, 'combat_meleespecialize_unarmed', 'Unarmed Combat', 2000),
	(15, 'combat_rangedspecialize_carbine', 'Carbine Weapons', 2000),
	(16, 'combat_rangedspecialize_pistol', 'Pistol Weapons', 2000),
	(17, 'combat_rangedspecialize_rifle', 'Rifle Weapons', 2000),
	(18, 'crafting_clothing_armor', 'Armor Crafting', 56000),
	(19, 'crafting_clothing_general', 'Tailoring', 56000),
	(20, 'crafting_droid_general', 'Droid Crafting', 56000),
	(21, 'crafting_food_general', 'Food Crafting', 56000),
	(22, 'crafting_general', 'General Crafting', 1000),
	(23, 'crafting_medicine_general', 'Medicine Crafting', 1000),
	(24, 'crafting_spice', 'Spice Crafting', 45000),
	(25, 'crafting_structure_general', 'Structure Crafting', 140000),
	(26, 'crafting_weapons_general', 'Weapon Crafting', 56000),
	(27, 'creaturehandler', 'Creature Handling', 20000),
	(28, 'dance', 'Dancing', 2000),
	(29, 'entertainer_healing', 'Entertainer Healing', 2000),
	(30, 'imagedesigner', 'Image Designer', 2000),
	(31, 'jedi', 'Todo: jedi', 2000),
	(32, 'medical', 'Medical', 2000),
	(33, 'merchant', 'Merchant', 10000),
	(34, 'music', 'Musician', 2000),
	(35, 'resource_harvesting_inorganic', 'Surveying', 1000),
	(36, 'scout', 'Scouting', 2000),
	(37, 'slicing', 'Slicing', 30000),
	(38, 'squadleader', 'Squad Leadership', 500000),
	(39, 'trapping', 'Trapping', 2000),
	(40, 'combat_rangedspecialize_heavy', 'Heavy Weapons', 300000),
	(41, 'political', 'Political', 20000),
	(42, 'force_rank_xp', 'Force Rank', 625000),
	(43, 'fs_combat', 'Force-sensitive Combat', 5100000),
	(44, 'fs_crafting', 'Force-sensitive Crafting', 5100000),
	(45, 'fs_senses', 'Force-sensitive Senses', 5100000),
	(46, 'fs_reflex', 'Force-sensitive Reflex', 5100000),
	(47, 'shipwright', 'Shipwright', 60000),
	(48, 'space_combat_general', 'Starship Combat', 10000),
	(49, 'none', 'None', 0);
/*!40000 ALTER TABLE `xp_type` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
