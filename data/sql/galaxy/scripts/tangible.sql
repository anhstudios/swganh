# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.2-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-21 14:46:06
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

# Dumping structure for table galaxy.tangible
CREATE TABLE IF NOT EXISTS `tangible` (
  `id` bigint(20) NOT NULL,
  `customization` varchar(255) NOT NULL,
  `options_bitmask` int(11) NOT NULL,
  `incap_timer` int(11) NOT NULL,
  `condition_damage` int(11) NOT NULL,
  `max_condition` int(11) NOT NULL,
  `is_moveable` tinyint(1) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `FK_80F8D49EBF396750` FOREIGN KEY (`id`) REFERENCES `object` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.tangible: ~10 rows (approximately)
DELETE FROM `tangible`;
/*!40000 ALTER TABLE `tangible` DISABLE KEYS */;
INSERT INTO `tangible` (`id`, `customization`, `options_bitmask`, `incap_timer`, `condition_damage`, `max_condition`, `is_moveable`) VALUES
	(8589934603, '#ÿgÿ$tÿô\rí	ÿÿf xË!ÿÿ\'ºÿÿÿÚjÿŸÿ¹ê\nÿ#%ÿ$ÿÿÿÿ', 0, 0, 0, 0, 1),
	(8589934609, '', 0, 0, 0, 0, 0),
	(8589934619, 'ÿ3ÿÁÿ\rÿ	ÿäÿÿ;(\'ÿ¨4Yÿÿ<ÿŸÿä~\nÿ	ÿ', 0, 0, 0, 0, 1),
	(8589934625, '	', 0, 0, 0, 0, 0),
	(8589934635, 'Íÿÿÿb\rÿ	ã1ÿØÿ(Ó\'ÿ\nËˆ²ÿÿ#ÿÿfü\nÿ	ÿ', 0, 0, 0, 0, 1),
	(8589934641, '', 0, 0, 0, 0, 0),
	(8589934651, '#ÿËÿÊ|ÿ\rò	ÿPÿ „¦!ÿÿhÿÿÿÇÿ¨0ÿ£\nÿ#	%$ÿÿÿÿ', 0, 0, 0, 0, 1),
	(8589934657, '', 0, 0, 0, 0, 0),
	(8589934667, '#ÿÝÿÿËÿ\r`	ÿ—ÿ ˜ÿ!ÿeÿÇÿtÿLÿ(ÿ\nè#%$ÿÿÿ', 0, 0, 0, 0, 1),
	(8589934673, '	', 0, 0, 0, 0, 0),
	(8589934683, '#ÿÌ6ÿÿÿ \rÿ	°ÿ÷ iÿ!ÿk/ÿµÿåÿ†ÿ4ÿ$…\nÿ6%%5ÿ$ÿ*ÿ"ÿÿ', 0, 0, 0, 0, 1),
	(8589934689, '', 0, 0, 0, 0, 0);
/*!40000 ALTER TABLE `tangible` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
