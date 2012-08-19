/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

DROP TABLE IF EXISTS `container_permissions`;
CREATE TABLE IF NOT EXISTS `container_permissions` (
  `type` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(35) DEFAULT NULL,
  PRIMARY KEY (`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `container_permissions` VALUES (1, 'default');
INSERT INTO `container_permissions` VALUES (2, 'world');
INSERT INTO `container_permissions` VALUES (3, 'creature');
INSERT INTO `container_permissions` VALUES (4, 'creature container');
INSERT INTO `container_permissions` VALUES (5, 'rideable');

/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
