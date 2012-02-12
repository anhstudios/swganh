
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `species` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `health_min` int(11) NOT NULL,
  `health_max` int(11) NOT NULL,
  `strength_min` int(11) NOT NULL,
  `strength_max` int(11) NOT NULL,
  `constitution_min` int(11) NOT NULL,
  `constitution_max` int(11) NOT NULL,
  `action_min` int(11) NOT NULL,
  `action_max` int(11) NOT NULL,
  `quickness_min` int(11) NOT NULL,
  `quickness_max` int(11) NOT NULL,
  `stamina_min` int(11) NOT NULL,
  `stamina_max` int(11) NOT NULL,
  `mind_min` int(11) NOT NULL,
  `mind_max` int(11) NOT NULL,
  `focus_min` int(11) NOT NULL,
  `focus_max` int(11) NOT NULL,
  `willpower_min` int(11) NOT NULL,
  `willpower_max` int(11) NOT NULL,
  `total` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=latin1;

DELETE FROM `species`;
    
/*!40000 ALTER TABLE `species` DISABLE KEYS */;
INSERT INTO `species` (`id`, `name`, `health_min`, `health_max`, `strength_min`, `strength_max`, `constitution_min`, `constitution_max`, `action_min`, `action_max`, `quickness_min`, `quickness_max`, `stamina_min`, `stamina_max`, `mind_min`, `mind_max`, `focus_min`, `focus_max`, `willpower_min`, `willpower_max`, `total`) VALUES
	(1, 'human', 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 400, 1100, 5400),
	(2, 'rodian', 300, 1000, 300, 500, 300, 400, 300, 1200, 300, 650, 450, 850, 300, 1000, 300, 500, 350, 550, 5400),
	(3, 'trandoshan', 550, 1250, 600, 800, 700, 800, 300, 1000, 300, 450, 300, 400, 300, 1000, 300, 500, 300, 600, 5550),
	(4, 'moncal', 300, 1000, 300, 500, 300, 400, 300, 1000, 300, 450, 450, 550, 600, 1300, 600, 800, 450, 650, 5400),
	(5, 'wookiee', 650, 1350, 650, 850, 450, 550, 500, 1200, 400, 550, 400, 500, 400, 1100, 450, 650, 400, 600, 6100),
	(6, 'bothan', 300, 1000, 300, 500, 300, 400, 600, 1300, 600, 750, 400, 500, 400, 1100, 400, 600, 300, 500, 5400),
	(7, 'twilek', 300, 1000, 300, 500, 550, 650, 550, 1250, 600, 750, 300, 400, 400, 1100, 300, 500, 300, 500, 5400),
	(8, 'zabrak', 500, 1200, 300, 500, 300, 400, 600, 1300, 300, 450, 300, 400, 300, 1000, 300, 500, 700, 900, 5400),
	(9, 'ithorian', 300, 1400, 300, 600, 300, 500, 600, 1100, 300, 750, 300, 500, 400, 1300, 400, 600, 300, 500, 5400),
	(10, 'sullustan', 300, 1200, 300, 500, 300, 400, 600, 1400, 300, 750, 300, 500, 400, 1200, 400, 600, 300, 600, 5400);
/*!40000 ALTER TABLE `species` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
