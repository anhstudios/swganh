
DROP TABLE IF EXISTS `buffs`;

CREATE TABLE `buffs` (
	`id` BIGINT(20) NOT NULL,
	`name` VARCHAR(50),
	`duration` INT(11),
	PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;