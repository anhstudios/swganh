
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.quest_journal_list
DROP TABLE IF EXISTS `quest_journal_list`;
CREATE TABLE IF NOT EXISTS `quest_journal_list` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `player_id` bigint(20) DEFAULT NULL,
  `quest_owner_id` bigint(20) DEFAULT NULL,
  `quest_crc` int(11) NOT NULL,
  `active_step_bitmask` int(11) NOT NULL,
  `completed_step_bitmask` int(11) NOT NULL,
  `completed` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_DF1EF7BA99E6F5DF` (`player_id`),
  KEY `IDX_DF1EF7BA9F66DE5C` (`quest_owner_id`),
  CONSTRAINT `FK_DF1EF7BA99E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`),
  CONSTRAINT `FK_DF1EF7BA9F66DE5C` FOREIGN KEY (`quest_owner_id`) REFERENCES `player` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.quest_journal_list: ~0 rows (approximately)
/*!40000 ALTER TABLE `quest_journal_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_journal_list` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
