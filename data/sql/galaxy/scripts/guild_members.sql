-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:58:57
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table galaxy.guild_members
DROP TABLE IF EXISTS `guild_members`;
CREATE TABLE IF NOT EXISTS `guild_members` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `guild_member_id` bigint(20) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK997283427384` (`guild_member_id`),
  CONSTRAINT `FK997283427384` FOREIGN KEY (`guild_member_id`) REFERENCES `object` (`id`) ON DELETE NO ACTION ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='members of the guild\r\n';

-- Dumping data for table galaxy.guild_members: ~0 rows (approximately)
/*!40000 ALTER TABLE `guild_members` DISABLE KEYS */;
/*!40000 ALTER TABLE `guild_members` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
