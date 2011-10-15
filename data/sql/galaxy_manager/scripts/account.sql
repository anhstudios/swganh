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

# Dumping structure for table galaxy_manager.account
CREATE TABLE IF NOT EXISTS `account` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) NOT NULL,
  `username_canonical` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `email_canonical` varchar(255) NOT NULL,
  `enabled` tinyint(1) NOT NULL,
  `algorithm` varchar(255) NOT NULL,
  `salt` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `last_login` datetime DEFAULT NULL,
  `locked` tinyint(1) NOT NULL,
  `expired` tinyint(1) NOT NULL,
  `expires_at` datetime DEFAULT NULL,
  `confirmation_token` varchar(255) DEFAULT NULL,
  `password_requested_at` datetime DEFAULT NULL,
  `roles` longtext NOT NULL COMMENT '(DC2Type:array)',
  `credentials_expired` tinyint(1) NOT NULL,
  `credentials_expire_at` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UNIQ_7D3656A492FC23A8` (`username_canonical`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;

# Dumping data for table galaxy_manager.account: ~2 rows (approximately)
/*!40000 ALTER TABLE `account` DISABLE KEYS */;
INSERT IGNORE INTO `account` (`id`, `username`, `username_canonical`, `email`, `email_canonical`, `enabled`, `algorithm`, `salt`, `password`, `last_login`, `locked`, `expired`, `expires_at`, `confirmation_token`, `password_requested_at`, `roles`, `credentials_expired`, `credentials_expire_at`) VALUES
	(6, 'kronos3', 'kronos3', '', '', 1, '', '20111014165409', 'c42d960061f74617f54d5342e22d96c12ff5cdae', '2011-10-14 16:54:09', 0, 0, NULL, NULL, NULL, '0', 0, NULL),
	(7, 'kronos', 'kronos', '', '', 1, '', '20111014165741', 'a4048cab95fd8f044b04476dc8a623bbed65fb96', '2011-10-14 16:57:41', 0, 0, NULL, NULL, NULL, '0', 0, NULL),
	(8, 'kronos5', 'kronos5', '', '', 1, '', '20111015102756', 'b16806e81ac566088ab9111c92b3039f72d8cff4', '2011-10-15 10:27:56', 0, 0, NULL, NULL, NULL, '0', 0, NULL),
	(9, 'kronos1', 'kronos1', '', '', 1, '', '20111015102947', '0d818e0e39158f94dc428e193d3472648f6ae591', '2011-10-15 10:29:47', 0, 0, NULL, NULL, NULL, '0', 0, NULL),
	(10, 'kronos2', 'kronos2', '', '', 1, '', '20111015103023', '54502d0315ac2c50c756d0cc6c9a4bfccd0c97a0', '2011-10-15 10:30:23', 0, 0, NULL, NULL, NULL, '0', 0, NULL),
	(11, 'kronos4', 'kronos4', '', '', 1, '', '20111015103228', 'a0efceca1b155d707de3d85bcde58f018e913519', '2011-10-15 10:32:28', 0, 0, NULL, NULL, NULL, '0', 0, NULL);
/*!40000 ALTER TABLE `account` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
