# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-15 13:18:33
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.player_session
CREATE TABLE IF NOT EXISTS `player_session` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `player` int(11) NOT NULL,
  `session_key` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.player_session: ~12 rows (approximately)
DELETE FROM `player_session`;
/*!40000 ALTER TABLE `player_session` DISABLE KEYS */;
INSERT INTO `player_session` (`id`, `player`, `session_key`) VALUES
	(1, 2, '2011-Oct-15 10:34:12.860786583892433'),
	(2, 2, '2011-Oct-15 11:00:04.2395191911554948'),
	(3, 2, '2011-Oct-15 11:01:58.578059108479490'),
	(4, 2, '2011-Oct-15 11:17:55.731805650786457'),
	(5, 2, '2011-Oct-15 12:05:00.5653771487939274'),
	(6, 2, '2011-Oct-15 12:08:01.1237042075744114'),
	(7, 2, '2011-Oct-15 12:49:19.1854411764510336'),
	(8, 2, '2011-Oct-15 12:50:48.2655361754873045'),
	(9, 2, '2011-Oct-15 12:57:43.378279915998978'),
	(10, 2, '2011-Oct-15 13:04:25.911303334053504'),
	(11, 2, '2011-Oct-15 13:04:47.329528600393932'),
	(12, 2, '2011-Oct-15 13:12:56.993535264187668');
/*!40000 ALTER TABLE `player_session` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
