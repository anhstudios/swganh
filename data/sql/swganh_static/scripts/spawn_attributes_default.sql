USE swganh_static;

DROP TABLE IF EXISTS `spawn_attributes_default`;

CREATE TABLE `spawn_attributes_default` (
  `spawn_id` BIGINT(20) NOT NULL AUTO_INCREMENT,
  `attribute_id` BIGINT(20) NOT NULL,
  `value` VARCHAR(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;