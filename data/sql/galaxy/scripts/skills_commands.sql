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

# Dumping structure for table galaxy.skills_commands
CREATE TABLE IF NOT EXISTS `skills_commands` (
  `skill_id` int(11) NOT NULL,
  `command_id` bigint(20) NOT NULL,
  PRIMARY KEY (`skill_id`,`command_id`),
  KEY `IDX_B26E8E4F5585C142` (`skill_id`),
  KEY `IDX_B26E8E4F33E1689A` (`command_id`),
  CONSTRAINT `FK_B26E8E4F33E1689A` FOREIGN KEY (`command_id`) REFERENCES `command` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK_B26E8E4F5585C142` FOREIGN KEY (`skill_id`) REFERENCES `skill` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.skills_commands: ~0 rows (approximately)
DELETE FROM `skills_commands`;
/*!40000 ALTER TABLE `skills_commands` DISABLE KEYS */;
/*!40000 ALTER TABLE `skills_commands` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
