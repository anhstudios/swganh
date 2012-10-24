-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:17
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.inventory_types
DROP TABLE IF EXISTS `inventory_types`;
CREATE TABLE IF NOT EXISTS `inventory_types` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Inventory Type ID',
  `object_string` varchar(255) NOT NULL DEFAULT 'object/tangible/inventory/shared_character_inventory.iff' COMMENT 'Inventory Object String',
  `slots` int(11) unsigned NOT NULL DEFAULT '50' COMMENT 'Inventory Slots',
  `name` varchar(255) NOT NULL DEFAULT 'inventory' COMMENT 'Inventory Name',
  `file` varchar(255) NOT NULL DEFAULT 'item_n' COMMENT 'Inventory File',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 COMMENT='Inventory Types';

-- Dumping data for table swganh_static.inventory_types: ~14 rows (approximately)
/*!40000 ALTER TABLE `inventory_types` DISABLE KEYS */;
INSERT INTO `inventory_types` (`id`, `object_string`, `slots`, `name`, `file`) VALUES
	(1, 'object/tangible/inventory/shared_character_inventory.iff', 80, 'inventory', 'item_n'),
	(2, 'object/tangible/inventory/shared_creature_inventory.iff', 50, 'inventory', 'item_n'),
	(3, 'object/tangible/inventory/shared_creature_inventory_1.iff', 50, 'inventory', 'item_n'),
	(4, 'object/tangible/inventory/shared_creature_inventory_2.iff', 50, 'inventory', 'item_n'),
	(5, 'object/tangible/inventory/shared_creature_inventory_3.iff', 50, 'inventory', 'item_n'),
	(6, 'object/tangible/inventory/shared_creature_inventory_4.iff', 50, 'inventory', 'item_n'),
	(7, 'object/tangible/inventory/shared_creature_inventory_5.iff', 50, 'inventory', 'item_n'),
	(8, 'object/tangible/inventory/shared_creature_inventory_6.iff', 50, 'inventory', 'item_n'),
	(9, 'object/tangible/inventory/shared_vendor_inventory.iff', 50, 'inventory', 'item_n'),
	(10, 'object/tangible/inventory/shared_lightsaber_inventory_training.iff', 50, 'inventory', 'item_n'),
	(11, 'object/tangible/inventory/shared_lightsaber_inventory_1.iff', 50, 'inventory', 'item_n'),
	(12, 'object/tangible/inventory/shared_lightsaber_inventory_2.iff', 50, 'inventory', 'item_n'),
	(13, 'object/tangible/inventory/shared_lightsaber_inventory_3.iff', 50, 'inventory', 'item_n'),
	(14, 'object/tangible/inventory/shared_lightsaber_inventory_4.iff', 50, 'inventory', 'item_n');
/*!40000 ALTER TABLE `inventory_types` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
