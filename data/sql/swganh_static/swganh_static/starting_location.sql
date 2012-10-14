-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:19
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.starting_location
DROP TABLE IF EXISTS `starting_location`;
CREATE TABLE IF NOT EXISTS `starting_location` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Location ID',
  `location` varchar(16) NOT NULL COMMENT 'Starting Location',
  `planet_id` int(11) NOT NULL COMMENT 'Starting Location - Planet',
  `x` float(10,6) NOT NULL COMMENT 'Starting Location - X',
  `y` float(10,6) NOT NULL COMMENT 'Starting Location - Y',
  `z` float(10,6) NOT NULL COMMENT 'Starting Location - Z',
  `description` varchar(64) NOT NULL COMMENT 'Starting Location - Description',
  `radius` float(2,0) unsigned NOT NULL COMMENT 'Starting Location - View Radius?',
  `heading` int(11) unsigned NOT NULL COMMENT 'Starting Location - Orientation',
  PRIMARY KEY (`id`),
  KEY `fk_starting_planet_planet` (`planet_id`),
  CONSTRAINT `starting_location_planet_id` FOREIGN KEY (`planet_id`) REFERENCES `planet` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.starting_location: ~18 rows (approximately)
/*!40000 ALTER TABLE `starting_location` DISABLE KEYS */;
INSERT INTO `starting_location` (`id`, `location`, `planet_id`, `x`, `y`, `z`, `description`, `radius`, `heading`) VALUES
	(1, 'bestine', 8, -1290.000000, 0.000000, -3590.000000, 'Bestine, Tatooine', 3, 180),
	(2, 'mos_espa', 8, -2902.000000, 0.000000, 2130.000000, 'Mos Espa, Tatooine', 3, 180),
	(3, 'mos_eisley', 8, 3528.000000, 0.000000, -4804.000000, 'Mos Eisley, Tatooine', 3, 180),
	(4, 'mos_entha', 8, 1291.000000, 0.000000, 3138.000000, 'Mos Entha, Tatooine', 3, 180),
	(5, 'coronet', 0, -137.000000, 0.000000, -4723.000000, 'Coronet, Corellia', 3, 180),
	(6, 'tyrena', 0, -5045.000000, 0.000000, -2294.000000, 'Tyrena, Corellia', 3, 180),
	(7, 'kor_vella', 0, -3138.000000, 0.000000, 2808.000000, 'Kor Vella, Corellia', 3, 180),
	(8, 'doaba_guerfel', 0, 3336.000000, 0.000000, 5525.000000, 'Doaba Guerfel, Corellia', 3, 180),
	(9, 'dearic', 7, 335.000000, 0.000000, -2931.000000, 'Dearic, Talus', 3, 180),
	(10, 'nashal', 7, 4371.000000, 0.000000, 5165.000000, 'Nashal, Talus', 3, 180),
	(11, 'narmle', 6, -5310.000000, 0.000000, -2221.000000, 'Narmle, Rori', 3, 180),
	(12, 'restuss', 6, 5362.000000, 0.000000, 5663.000000, 'Restuss, Rori', 3, 180),
	(13, 'theed', 5, -4856.000000, 0.000000, 4162.000000, 'Theed, Naboo', 3, 180),
	(14, 'moenia', 5, 4800.000000, 0.000000, -4700.000000, 'Moenia, Naboo', 3, 180),
	(15, 'keren', 5, 1441.000000, 0.000000, 2771.000000, 'Keren, Naboo', 3, 180),
	(16, 'kaadara', 5, 5209.000000, 0.000000, 6677.000000, 'Kaadara, Naboo', 3, 180),
	(17, 'tutorial', 41, 0.000000, 0.000000, 0.000000, 'Tutorial', 3, 180),
	(18, 'default_location', 41, 27.400000, -42.000000, -161.100006, 'Skipped Tutorial', 3, 180);
/*!40000 ALTER TABLE `starting_location` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
