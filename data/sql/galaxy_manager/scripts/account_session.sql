# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-15 10:35:23
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
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

# Dumping data for table galaxy_manager.account_session: ~1 rows (approximately)
/*!40000 ALTER TABLE `account_session` DISABLE KEYS */;
INSERT IGNORE INTO `account_session` (`id`, `account`, `session_key`) VALUES
	(1, 7, '2011-Oct-14 17:15:12.144133127.0.0.1'),
	(2, 7, '2011-Oct-15 10:16:38.228464127.0.0.1'),
	(3, 7, '2011-Oct-15 10:20:02.484147127.0.0.1'),
	(4, 11, '2011-Oct-15 10:32:28.428813127.0.0.1'),
	(5, 7, '2011-Oct-15 10:33:41.308981127.0.0.1'),
	(6, 11, '2011-Oct-15 10:34:09.992622127.0.0.1');
/*!40000 ALTER TABLE `account_session` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
