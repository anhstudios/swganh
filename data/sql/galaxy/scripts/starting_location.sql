
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `starting_location` (
  `location_id` int(11) NOT NULL AUTO_INCREMENT,
  `scene_id` int(11) DEFAULT NULL,
  `location` varchar(100) NOT NULL,
  `x` double NOT NULL,
  `y` double NOT NULL,
  `z` double NOT NULL,
  `description` varchar(100) NOT NULL,
  `radius` int(11) NOT NULL,
  `heading` int(11) NOT NULL,
  PRIMARY KEY (`location_id`),
  KEY `IDX_6D2A488E166053B4` (`scene_id`),
  CONSTRAINT `FK_6D2A488E166053B4` FOREIGN KEY (`scene_id`) REFERENCES `scene` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=latin1;

DELETE FROM `starting_location`;
    
/*!40000 ALTER TABLE `starting_location` DISABLE KEYS */;
INSERT INTO `starting_location` (`location_id`, `scene_id`, `location`, `x`, `y`, `z`, `description`, `radius`, `heading`) VALUES
	(1, 9, 'bestine', -1290, 0, -3590, 'Bestine, Tatooine', 3, 180),
	(2, 9, 'mos_espa', -2902, 0, 2130, 'Mos Espa, Tatooine', 3, 180),
	(3, 9, 'mos_eisley', 3528, 0, -4804, 'Mos Eisley, Tatooine', 3, 180),
	(4, 9, 'mos_entha', 1291, 0, 3138, 'Mos Entha, Tatooine', 3, 180),
	(5, 1, 'coronet', -137, 0, -4723, 'Coronet, Corellia', 3, 180),
	(6, 1, 'tyrena', -5045, 0, -2294, 'Tyrena, Corellia', 3, 180),
	(7, 1, 'kor_vella', -3138, 0, 2808, 'Kor Vella, Corellia', 3, 180),
	(8, 1, 'doaba_guerfel', 3336, 0, 5525, 'Doaba Guerfel, Corellia', 3, 180),
	(9, 8, 'dearic', 335, 0, -2931, 'Dearic, Talus', 3, 180),
	(10, 8, 'nashal', 4371, 0, 5165, 'Nashal, Talus', 3, 180),
	(11, 7, 'narmle', -5310, 0, -2221, 'Narmle, Rori', 3, 180),
	(12, 7, 'restuss', 5362, 0, 5663, 'Restuss, Rori', 3, 180),
	(13, 6, 'theed', -4856, 0, 4162, 'Theed, Naboo', 3, 180),
	(14, 6, 'moenia', 4800, 0, -4700, 'Moenia, Naboo', 3, 180),
	(15, 6, 'keren', 1441, 0, 2771, 'Keren, Naboo', 3, 180),
	(16, 6, 'kaadara', 5209, 0, 6677, 'Kaadara, Naboo', 3, 180),
	(17, 42, 'tutorial', 0, 0, 0, 'Tutorial', 3, 180),
	(18, 42, 'default_location', 27.4, -42, -161.100006, 'Skipped Tutorial', 3, 180);
/*!40000 ALTER TABLE `starting_location` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
