/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.draft_schematic_list
DROP TABLE IF EXISTS `draft_schematic_list`;
CREATE TABLE IF NOT EXISTS `draft_schematic_list` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `player_id` bigint(20) DEFAULT NULL,
  `schematic_id` int(11) NOT NULL,
  `schematic_crc` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_4D38EF9B99E6F5DF` (`player_id`),
  CONSTRAINT `FK_4D38EF9B99E6F5DF` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.draft_schematic_list: ~0 rows (approximately)
/*!40000 ALTER TABLE `draft_schematic_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `draft_schematic_list` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
