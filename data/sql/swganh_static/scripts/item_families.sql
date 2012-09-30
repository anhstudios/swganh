-- ---------------------------------------------------------------------------------------
-- This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)
--
-- For more information, visit http://www.swganh.com
--
-- Copyright (c) 2006 - 2012 The SWG:ANH Team
-- ---------------------------------------------------------------------------------------
-- This library is free software; you can redistribute it and/or
-- modify it under the terms of the GNU Lesser General Public
-- License as published by the Free Software Foundation; either
-- version 2.1 of the License, or (at your option) any later version.
--
-- This library is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-- Lesser General Public License for more details.
--
-- You should have received a copy of the GNU Lesser General Public
-- License along with this library; if not, write to the Free Software
-- Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
-- ---------------------------------------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

USE swganh_static;

--
-- Table structure for table `item_families`
--

DROP TABLE IF EXISTS `item_families`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `item_families` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT COMMENT 'Item Family ID',
  `family_name` varchar(255) DEFAULT NULL COMMENT 'Item Family Name',
  `crate_size` int(10) unsigned NOT NULL DEFAULT '25' COMMENT 'Item Family Crate Size',
  `crate_type` int(10) unsigned NOT NULL DEFAULT '2772' COMMENT 'Item Family Crate Type',
  PRIMARY KEY (`id`),
  UNIQUE KEY `pk_item_family_name` (`family_name`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8 ROW_FORMAT=REDUNDANT COMMENT='Item Families';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `item_families`
--

LOCK TABLES `item_families` WRITE;
/*!40000 ALTER TABLE `item_families` DISABLE KEYS */;
INSERT INTO `item_families` VALUES (1,'survey_tool',25,2775),
(2,'travel_ticket',25,2775),
(3,'crafting_tool',25,2775),
(4,'crafting_station',25,2775),
(5,'furniture',25,2781),
(6,'food',25,2776),
(7,'wearable',25,2779),
(8,'manufacture schematic',25,2772),
(9,'generic',25,2775),
(10,'weapon',25,2782),
(11,'instrument',25,2775),
(12,'bug_jar',25,2772),
(13,'component',25,2775),
(14,'crafting',25,2772),
(15,'deed',0,0),
(16,'dice',25,2775),
(17,'droid',25,2777),
(18,'droid_battery',25,2777),
(19,'firework',25,2775),
(20,'fishing',25,2775),
(21,'item',25,2772),
(22,'medicine',25,2780),
(23,'mission',25,2772),
(24,'powerup',25,2782),
(25,'scout',25,2772),
(26,'ship',25,2772),
(27,'slicing',25,2775),
(28,'parrot_cage',25,2775),
(29,'hopper',25,2772),
(30,'crate',0,0);
/*!40000 ALTER TABLE `item_families` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-08-27  9:42:09
