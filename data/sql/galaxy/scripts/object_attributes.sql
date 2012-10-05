/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

DROP TABLE IF EXISTS `object_attributes`;
CREATE TABLE IF NOT EXISTS `object_attributes` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `object_id` bigint(20) DEFAULT NULL,
  `attribute_id` int(11) unsigned DEFAULT NULL,
  `attribute_value` varchar(255) DEFAULT '',
  PRIMARY KEY (`id`),
  KEY `FK_object_attributes_swganh_static.attributes` (`attribute_id`),
  CONSTRAINT `FK_object_attributes_swganh_static.attributes` FOREIGN KEY (`attribute_id`) REFERENCES `swganh_static`.`attributes` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COMMENT='stores attributes for objects';

DELETE FROM `object_attributes`;
/*!40000 ALTER TABLE `object_attributes` DISABLE KEYS */;
/*!40000 ALTER TABLE `object_attributes` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
