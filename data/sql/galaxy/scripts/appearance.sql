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

# Dumping data for table galaxy.appearance: ~9 rows (approximately)
/*!40000 ALTER TABLE `appearance` DISABLE KEYS */;
INSERT INTO `appearance` (`object_id`, `scale`, `gender`, `species`, `customization_data`) VALUES
	(8589934613, 0, 1, 'zabrak', '#ÿ	ÿ#NÙÿ\rÐ	ÿÿ¡ —ò!ÿÿÿÿÿÏvÿ±ÿ»ƒ¶\nÿ6%%ÿ5$ÿ*ÿ"ÿÿ'),
	(8589934623, 0, 1, 'zabrak', '#ÿ	ÿ#NÙÿ\rÐ	ÿÿ¡ —ò!ÿÿÿÿÿÏvÿ±ÿ»ƒ¶\nÿ6%%ÿ5$ÿ*ÿ"ÿÿ'),
	(8589934633, 0, 1, 'zabrak', '#ÿ	ÿ#NÙÿ\rÐ	ÿÿ¡ —ò!ÿÿÿÿÿÏvÿ±ÿ»ƒ¶\nÿ6%%ÿ5$ÿ*ÿ"ÿÿ'),
	(8589934643, 0, 1, 'zabrak', '#ÿ	ÿ#NÙÿ\rÐ	ÿÿ¡ —ò!ÿÿÿÿÿÏvÿ±ÿ»ƒ¶\nÿ6%%ÿ5$ÿ*ÿ"ÿÿ'),
	(8589934649, 0, 1, 'zabrak', ''),
	(8589934659, 0, 1, 'human', '#ÿÆjÿÿÿ—\r`	ÿÔÿ ÿ!ÿÿÿÿ_\rbÿÿ-ÿ/\n\nÿ#%$ÿÿÿ'),
	(8589934665, 0, 1, 'human', ''),
	(8589934675, 0, 1, 'moncal', 'ÿÿÏüÿ\rÿ	Ûÿgÿ5KÿÿÿµÿÿÙd\nÿ;\n&ÿ'),
	(8589934681, 0, 1, 'moncal', ''),
	(8589934691, 0, 1, 'trandoshan', '[ÿJ\r·	ÿ-ÿLÿ¥ÿ8ÿÿÿnÿqÿ\n˜ÿ'),
	(8589934697, 0, 1, 'trandoshan', '');
/*!40000 ALTER TABLE `appearance` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
