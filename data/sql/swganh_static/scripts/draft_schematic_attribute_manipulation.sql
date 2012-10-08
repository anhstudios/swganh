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
-- Table structure for table `draft_schematic_attribute_manipulation`
--

DROP TABLE IF EXISTS `draft_schematic_attribute_manipulation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `draft_schematic_attribute_manipulation` (
  `ID` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `Draft_Schematic` int(10) unsigned NOT NULL,
  `Attribute` int(10) unsigned NOT NULL,
  `AffectedAttribute` varchar(45) NOT NULL,
  `Manipulation` int(10) unsigned NOT NULL,
  `list_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=34 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `draft_schematic_attribute_manipulation`
--

LOCK TABLES `draft_schematic_attribute_manipulation` WRITE;
/*!40000 ALTER TABLE `draft_schematic_attribute_manipulation` DISABLE KEYS */;
INSERT INTO `draft_schematic_attribute_manipulation` VALUES (8,140,613,'303',1,113),
(9,140,185,'185',1,114),
(10,304,613,'366',1,2137),
(11,304,613,'367',1,2137),
(12,304,185,'204',1,2138),
(13,152,613,'9999',0,134),
(14,1073,15,'303',1,63),
(15,890,94,'460',1,1),
(17,269,613,'372',1,129),
(18,269,185,'204',1,130),
(19,279,613,'375',1,131),
(20,279,185,'204',1,132),
(21,300,613,'367',1,76),
(22,300,613,'366',1,76),
(23,300,185,'204',1,78),
(25,294,185,'23',0,148),
(26,270,613,'372',1,137),
(27,270,185,'204',1,138),
(28,274,613,'373',1,139),
(29,274,185,'23',1,140),
(30,280,613,'375',1,141),
(31,280,185,'23',1,142),
(32,284,613,'377',1,143),
(33,284,185,'23',1,144);
/*!40000 ALTER TABLE `draft_schematic_attribute_manipulation` ENABLE KEYS */;
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
