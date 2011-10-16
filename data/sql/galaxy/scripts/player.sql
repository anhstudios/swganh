# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-15 23:25:17
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.player
CREATE TABLE IF NOT EXISTS `player` (
  `id` bigint(20) NOT NULL,
  `profession_tag` varchar(255) NOT NULL,
  `born_date` date NOT NULL,
  `total_playtime` bigint(20) NOT NULL,
  `csr_tag` smallint(6) NOT NULL,
  `current_force` int(11) NOT NULL,
  `max_force` int(11) NOT NULL,
  `experimentation_enabled` tinyint(1) NOT NULL,
  `crafting_stage` int(11) NOT NULL,
  `nearest_crafting_station` bigint(20) NOT NULL,
  `experimentation_points` int(11) NOT NULL,
  `accomplishment_counter` int(11) NOT NULL,
  `current_language` int(11) NOT NULL,
  `current_stomach` int(11) NOT NULL,
  `max_stomach` int(11) NOT NULL,
  `current_drink` int(11) NOT NULL,
  `max_drink` int(11) NOT NULL,
  `jedi_state` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_98197A65BF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.player: ~5 rows (approximately)
/*!40000 ALTER TABLE `player` DISABLE KEYS */;
INSERT INTO `player` (`id`, `profession_tag`, `born_date`, `total_playtime`, `csr_tag`, `current_force`, `max_force`, `experimentation_enabled`, `crafting_stage`, `nearest_crafting_station`, `experimentation_points`, `accomplishment_counter`, `current_language`, `current_stomach`, `max_stomach`, `current_drink`, `max_drink`, `jedi_state`) VALUES
	(8589934598, 'crafting_artisan', '2011-10-15', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(8589934603, 'outdoors_scout', '2011-10-15', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(8589934644, 'combat_marksman', '2011-10-15', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(8589934660, 'crafting_artisan', '2011-10-15', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(8589934676, 'crafting_artisan', '2011-10-15', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(8589934692, 'combat_brawler', '2011-10-15', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
/*!40000 ALTER TABLE `player` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
