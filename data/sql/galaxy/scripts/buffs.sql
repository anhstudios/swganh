
DROP TABLE IF EXISTS `buffs`;

CREATE TABLE `buffs` (
	`id` BIGINT(20) NOT NULL,
	`name` VARCHAR(50),
	`duration` INT(11),
	INDEX `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;