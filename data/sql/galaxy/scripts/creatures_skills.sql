# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-16 19:39:50
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

use galaxy;

# Dumping structure for table galaxy.creatures_skills
CREATE TABLE IF NOT EXISTS `creatures_skills` (
  `creature_id` bigint(20) NOT NULL,
  `skill_id` int(11) NOT NULL,
  PRIMARY KEY (`creature_id`,`skill_id`),
  KEY `IDX_56339608F9AB048` (`creature_id`),
  KEY `IDX_563396085585C142` (`skill_id`),
  CONSTRAINT `FK_563396085585C142` FOREIGN KEY (`skill_id`) REFERENCES `skill` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK_56339608F9AB048` FOREIGN KEY (`creature_id`) REFERENCES `creature` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.creatures_skills: ~0 rows (approximately)
DELETE FROM `creatures_skills`;
/*!40000 ALTER TABLE `creatures_skills` DISABLE KEYS */;
/*!40000 ALTER TABLE `creatures_skills` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
