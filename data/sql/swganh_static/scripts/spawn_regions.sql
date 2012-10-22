USE swganh_static;

DROP TABLE IF EXISTS `spawn_regions`;

CREATE TABLE `spawn_regions` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `x` float NOT NULL,
  `y` float,
  `z` float NOT NULL,
  `radius` float NOT NULL,
  `spawn_type_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;