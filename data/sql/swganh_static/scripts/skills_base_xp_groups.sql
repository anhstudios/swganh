-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:19
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.skills_base_xp_groups
DROP TABLE IF EXISTS `skills_base_xp_groups`;
CREATE TABLE IF NOT EXISTS `skills_base_xp_groups` (
  `skill_id` int(11) unsigned NOT NULL,
  `xp_type_id` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`skill_id`,`xp_type_id`),
  KEY `fk_skills_xpgroups_xp_xp` (`xp_type_id`),
  CONSTRAINT `fk_skills_xpgroups_skill_skill` FOREIGN KEY (`skill_id`) REFERENCES `skills` (`skill_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_skills_xpgroups_xp_xp` FOREIGN KEY (`xp_type_id`) REFERENCES `xp_types` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.skills_base_xp_groups: ~88 rows (approximately)
/*!40000 ALTER TABLE `skills_base_xp_groups` DISABLE KEYS */;
INSERT INTO `skills_base_xp_groups` (`skill_id`, `xp_type_id`) VALUES
	(357, 2),
	(357, 3),
	(528, 4),
	(31, 5),
	(319, 5),
	(110, 6),
	(129, 6),
	(148, 6),
	(167, 6),
	(186, 6),
	(205, 6),
	(224, 6),
	(243, 6),
	(547, 6),
	(688, 7),
	(707, 8),
	(726, 9),
	(764, 9),
	(688, 10),
	(707, 10),
	(726, 10),
	(764, 10),
	(861, 10),
	(899, 10),
	(918, 10),
	(937, 10),
	(91, 11),
	(205, 11),
	(91, 12),
	(243, 12),
	(91, 13),
	(224, 13),
	(91, 14),
	(186, 14),
	(110, 15),
	(167, 15),
	(528, 15),
	(110, 16),
	(148, 16),
	(509, 16),
	(528, 16),
	(110, 17),
	(129, 17),
	(376, 18),
	(433, 19),
	(471, 20),
	(414, 21),
	(71, 22),
	(51, 23),
	(300, 23),
	(566, 23),
	(509, 24),
	(452, 25),
	(395, 26),
	(338, 27),
	(11, 28),
	(262, 28),
	(11, 29),
	(262, 29),
	(281, 29),
	(11, 30),
	(585, 30),
	(51, 32),
	(300, 32),
	(566, 32),
	(490, 33),
	(11, 34),
	(281, 34),
	(71, 35),
	(31, 36),
	(319, 36),
	(509, 37),
	(604, 38),
	(31, 39),
	(319, 39),
	(528, 40),
	(547, 40),
	(623, 41),
	(957, 42),
	(970, 42),
	(784, 43),
	(822, 44),
	(841, 45),
	(803, 46),
	(992, 47),
	(1012, 48),
	(1031, 48),
	(1050, 48);
/*!40000 ALTER TABLE `skills_base_xp_groups` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
