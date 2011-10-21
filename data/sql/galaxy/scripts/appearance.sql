# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.2-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-21 14:46:00
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.appearance
CREATE TABLE IF NOT EXISTS `appearance` (
  `object_id` bigint(20) NOT NULL,
  `scale` double NOT NULL,
  `gender` int(11) NOT NULL,
  `species` varchar(100) NOT NULL,
  `customization_data` longtext NOT NULL,
  PRIMARY KEY (`object_id`),
  CONSTRAINT `FK_AAB38290232D562B` FOREIGN KEY (`object_id`) REFERENCES `object` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.appearance: ~10 rows (approximately)
DELETE FROM `appearance`;
/*!40000 ALTER TABLE `appearance` DISABLE KEYS */;
INSERT INTO `appearance` (`object_id`, `scale`, `gender`, `species`, `customization_data`) VALUES
	(8589934603, 0, 1, 'human', '#ÿgÿ$tÿô\rí	ÿÿf xË!ÿÿ\'ºÿÿÿÚjÿŸÿ¹ê\nÿ#%ÿ$ÿÿÿÿ'),
	(8589934609, 0, 1, 'human', ''),
	(8589934619, 0, 1, 'rodian', 'ÿ3ÿÁÿ\rÿ	ÿäÿÿ;(\'ÿ¨4Yÿÿ<ÿŸÿä~\nÿ	ÿ'),
	(8589934625, 0, 1, 'rodian', '	'),
	(8589934635, 0, 1, 'rodian', 'Íÿÿÿb\rÿ	ã1ÿØÿ(Ó\'ÿ\nËˆ²ÿÿ#ÿÿfü\nÿ	ÿ'),
	(8589934641, 0, 1, 'rodian', ''),
	(8589934651, 0, 1, 'human', '#ÿËÿÊ|ÿ\rò	ÿPÿ „¦!ÿÿhÿÿÿÇÿ¨0ÿ£\nÿ#	%$ÿÿÿÿ'),
	(8589934657, 0, 1, 'human', ''),
	(8589934667, 0, 1, 'human', '#ÿÝÿÿËÿ\r`	ÿ—ÿ ˜ÿ!ÿeÿÇÿtÿLÿ(ÿ\nè#%$ÿÿÿ'),
	(8589934673, 0, 1, 'human', '	'),
	(8589934683, 0, 1, 'zabrak', '#ÿÌ6ÿÿÿ \rÿ	°ÿ÷ iÿ!ÿk/ÿµÿåÿ†ÿ4ÿ$…\nÿ6%%5ÿ$ÿ*ÿ"ÿÿ'),
	(8589934689, 0, 1, 'zabrak', '');
/*!40000 ALTER TABLE `appearance` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
