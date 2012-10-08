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
-- Table structure for table `travel_planet_routes`
--

DROP TABLE IF EXISTS `travel_planet_routes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `travel_planet_routes` (
  `srcId` int(11) NOT NULL COMMENT 'Source Planet ID',
  `destId` int(11) NOT NULL DEFAULT '0' COMMENT 'Destination Planet ID',
  `price` int(11) unsigned NOT NULL DEFAULT '1000' COMMENT 'Travel Route Price',
  PRIMARY KEY (`srcId`,`destId`),
  KEY `travel_route_destination_planet_id` (`destId`),
  CONSTRAINT `travel_route_destination_planet_id` FOREIGN KEY (`destId`) REFERENCES `planet` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `travel_route_source_planet_id` FOREIGN KEY (`srcId`) REFERENCES `planet` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `travel_planet_routes`
--

LOCK TABLES `travel_planet_routes` WRITE;
/*!40000 ALTER TABLE `travel_planet_routes` DISABLE KEYS */;
INSERT INTO `travel_planet_routes` VALUES (0,0,100),
(0,1,1000),
(0,2,2000),
(0,3,4000),
(0,5,500),
(0,7,300),
(0,8,600),
(0,9,3000),
(0,42,1250),
(1,0,1000),
(1,1,100),
(2,0,2000),
(2,2,100),
(3,0,4000),
(3,3,100),
(3,5,1750),
(4,4,100),
(4,5,1250),
(4,8,1250),
(5,0,500),
(5,3,1750),
(5,4,1250),
(5,5,100),
(5,6,300),
(5,8,500),
(6,5,300),
(6,6,100),
(7,0,300),
(7,7,100),
(8,0,600),
(8,4,1250),
(8,5,500),
(8,8,100),
(9,0,3000),
(9,9,100),
(42,0,1250),
(42,42,100);
/*!40000 ALTER TABLE `travel_planet_routes` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-08-27  9:42:10
