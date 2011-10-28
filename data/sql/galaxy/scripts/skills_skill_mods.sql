# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-16 19:39:53
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

use galaxy;

# Dumping structure for table galaxy.skills_skill_mods
CREATE TABLE IF NOT EXISTS `skills_skill_mods` (
  `skill_id` int(11) NOT NULL,
  `skillmod_id` bigint(20) NOT NULL,
  PRIMARY KEY (`skill_id`,`skillmod_id`),
  KEY `IDX_E43191855585C142` (`skill_id`),
  KEY `IDX_E43191859C9E5E6D` (`skillmod_id`),
  CONSTRAINT `FK_E43191855585C142` FOREIGN KEY (`skill_id`) REFERENCES `skill` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK_E43191859C9E5E6D` FOREIGN KEY (`skillmod_id`) REFERENCES `skill_mod` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.skills_skill_mods: ~0 rows (approximately)
DELETE FROM `skills_skill_mods`;
/*!40000 ALTER TABLE `skills_skill_mods` DISABLE KEYS */;
/*!40000 ALTER TABLE `skills_skill_mods` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
