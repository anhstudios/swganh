# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-15 13:15:15
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy_manager.account_session
CREATE TABLE IF NOT EXISTS `account_session` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `account` int(11) NOT NULL,
  `session_key` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=24 DEFAULT CHARSET=latin1;

# Dumping data for table galaxy_manager.account_session: ~13 rows (approximately)
DELETE FROM `account_session`;
/*!40000 ALTER TABLE `account_session` DISABLE KEYS */;
INSERT INTO `account_session` (`id`, `account`, `session_key`) VALUES
	(1, 7, '2011-Oct-14 17:15:12.144133127.0.0.1'),
	(2, 7, '2011-Oct-15 10:16:38.228464127.0.0.1'),
	(3, 7, '2011-Oct-15 10:20:02.484147127.0.0.1'),
	(4, 11, '2011-Oct-15 10:32:28.428813127.0.0.1'),
	(5, 7, '2011-Oct-15 10:33:41.308981127.0.0.1'),
	(6, 11, '2011-Oct-15 10:34:09.992622127.0.0.1'),
	(7, 11, '2011-Oct-15 11:06:20.338031127.0.0.1'),
	(8, 11, '2011-Oct-15 11:06:20.469039127.0.0.1'),
	(9, 11, '2011-Oct-15 11:06:20.870061127.0.0.1'),
	(10, 11, '2011-Oct-15 11:06:51.018786127.0.0.1'),
	(11, 11, '2011-Oct-15 11:17:54.399729127.0.0.1'),
	(12, 11, '2011-Oct-15 12:04:59.364308127.0.0.1'),
	(13, 11, '2011-Oct-15 12:07:58.585559127.0.0.1'),
	(14, 11, '2011-Oct-15 12:49:11.953027127.0.0.1'),
	(15, 11, '2011-Oct-15 12:50:45.659387127.0.0.1'),
	(16, 11, '2011-Oct-15 12:57:42.180211127.0.0.1'),
	(17, 11, '2011-Oct-15 12:59:53.841741127.0.0.1'),
	(18, 11, '2011-Oct-15 13:00:00.852142127.0.0.1'),
	(19, 11, '2011-Oct-15 13:01:17.339517127.0.0.1'),
	(20, 11, '2011-Oct-15 13:01:34.551501127.0.0.1'),
	(21, 11, '2011-Oct-15 13:04:16.171746127.0.0.1'),
	(22, 11, '2011-Oct-15 13:04:47.120516127.0.0.1'),
	(23, 11, '2011-Oct-15 13:12:33.956217127.0.0.1');
/*!40000 ALTER TABLE `account_session` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
