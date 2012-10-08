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
-- Table structure for table `entertainer_performances`
--

DROP TABLE IF EXISTS `entertainer_performances`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `entertainer_performances` (
  `id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Entertainer Performance ID',
  `performanceName` varchar(32) DEFAULT NULL COMMENT 'Entertainer Performance Name',
  `instrumentAudioId` int(11) DEFAULT NULL,
  `requiredSong` varchar(64) DEFAULT NULL,
  `requiredInstrument` varchar(64) DEFAULT NULL,
  `instrument_type` int(11) DEFAULT NULL,
  `required_dance` varchar(64) DEFAULT NULL,
  `dance_visual_id` int(11) DEFAULT NULL,
  `action_point_per_loop` int(11) DEFAULT NULL,
  `loop_duration` int(11) DEFAULT NULL,
  `type` bigint(20) DEFAULT NULL,
  `base_xp` int(11) DEFAULT NULL,
  `florushXpMod` int(11) DEFAULT NULL,
  `healMindWound` int(11) DEFAULT NULL,
  `healShockWound` int(11) DEFAULT NULL,
  `requiredSkillMod` varchar(32) DEFAULT NULL,
  `mainloop` int(11) DEFAULT NULL,
  `music_visual_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Entertainer / Musician / Dancer Performances';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `entertainer_performances`
--

LOCK TABLES `entertainer_performances` WRITE;
/*!40000 ALTER TABLE `entertainer_performances` DISABLE KEYS */;
INSERT INTO `entertainer_performances` VALUES (0,'starwars1',1,'startmusic+starwars1','slitherhorn',1313,'',0,28,10,866729052,0,2,4,2,'healing_music_ability',5,3),
(1,'starwars1',2,'startmusic+starwars1','fizz',1315,'',0,32,10,866729052,0,3,6,3,'healing_music_ability',5,3),
(2,'starwars1',3,'startmusic+starwars1','fanfar',1318,'',0,36,10,866729052,0,6,6,3,'healing_music_ability',10,3),
(3,'starwars1',4,'startmusic+starwars1','kloohorn',1314,'',0,36,10,866729052,0,7,8,4,'healing_music_ability',10,3),
(4,'starwars1',5,'startmusic+starwars1','mandoviol',1323,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',15,5),
(5,'starwars1',6,'startmusic+starwars1','traz',1322,'',0,40,10,866729052,0,9,10,5,'healing_music_ability',15,3),
(6,'starwars1',7,'startmusic+starwars1','bandfill',1316,'',0,44,10,866729052,0,13,10,5,'healing_music_ability',15,1),
(7,'starwars1',8,'startmusic+starwars1','flutedroopy',1317,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',10,1),
(8,'starwars1',9,'startmusic+starwars1','omnibox',1319,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',10,4),
(9,'starwars1',10,'startmusic+starwars1','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',20,2),
(10,'starwars1',10,'startmusic+starwars1','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',20,2),
(11,'rock',11,'startmusic+rock','slitherhorn',1313,'',0,32,10,866729052,0,3,6,3,'healing_music_ability',10,3),
(12,'rock',12,'startmusic+rock','fizz',1315,'',0,32,10,866729052,0,3,6,3,'healing_music_ability',10,3),
(13,'rock',13,'startmusic+rock','fanfar',1318,'',0,36,10,866729052,0,6,6,3,'healing_music_ability',15,3),
(14,'rock',14,'startmusic+rock','kloohorn',1314,'',0,36,10,866729052,0,7,8,4,'healing_music_ability',15,3),
(15,'rock',15,'startmusic+rock','mandoviol',1323,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',20,5),
(16,'rock',16,'startmusic+rock','traz',1322,'',0,40,10,866729052,0,9,10,5,'healing_music_ability',20,3),
(17,'rock',17,'startmusic+rock','bandfill',1316,'',0,44,10,866729052,0,13,10,5,'healing_music_ability',20,1),
(18,'rock',18,'startmusic+rock','flutedroopy',1317,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',15,1),
(19,'rock',19,'startmusic+rock','omnibox',1319,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',15,4),
(20,'rock',20,'startmusic+rock','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',25,2),
(21,'rock',20,'startmusic+rock','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',25,2),
(22,'starwars2',21,'startmusic+starwars2','slitherhorn',1313,'',0,32,10,866729052,0,4,6,3,'healing_music_ability',20,3),
(23,'starwars2',22,'startmusic+starwars2','fizz',1315,'',0,32,10,866729052,0,4,6,3,'healing_music_ability',20,3),
(24,'starwars2',23,'startmusic+starwars2','fanfar',1318,'',0,36,10,866729052,0,6,6,3,'healing_music_ability',25,3),
(25,'starwars2',24,'startmusic+starwars2','kloohorn',1314,'',0,36,10,866729052,0,7,8,4,'healing_music_ability',25,3),
(26,'starwars2',25,'startmusic+starwars2','mandoviol',1323,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',30,5),
(27,'starwars2',26,'startmusic+starwars2','traz',1322,'',0,40,10,866729052,0,9,10,5,'healing_music_ability',30,3),
(28,'starwars2',27,'startmusic+starwars2','bandfill',1316,'',0,44,10,866729052,0,13,10,5,'healing_music_ability',30,1),
(29,'starwars2',28,'startmusic+starwars2','flutedroopy',1317,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',25,1),
(30,'starwars2',29,'startmusic+starwars2','omnibox',1319,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',25,4),
(31,'starwars2',30,'startmusic+starwars2','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',35,2),
(32,'starwars2',30,'startmusic+starwars2','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',35,2),
(33,'folk',31,'startmusic+folk','slitherhorn',1313,'',0,36,10,866729052,0,6,6,3,'healing_music_ability',30,3),
(34,'folk',32,'startmusic+folk','fizz',1315,'',0,36,10,866729052,0,6,6,3,'healing_music_ability',30,3),
(35,'folk',33,'startmusic+folk','fanfar',1318,'',0,36,10,866729052,0,6,6,3,'healing_music_ability',35,3),
(36,'folk',34,'startmusic+folk','kloohorn',1314,'',0,36,10,866729052,0,7,8,4,'healing_music_ability',35,3),
(37,'folk',35,'startmusic+folk','mandoviol',1323,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',35,5),
(38,'folk',36,'startmusic+folk','traz',1322,'',0,40,10,866729052,0,9,10,5,'healing_music_ability',40,3),
(39,'folk',37,'startmusic+folk','bandfill',1316,'',0,44,10,866729052,0,13,10,5,'healing_music_ability',40,1),
(40,'folk',38,'startmusic+folk','flutedroopy',1317,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',35,1),
(41,'folk',39,'startmusic+folk','omnibox',1319,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',35,4),
(42,'folk',40,'startmusic+folk','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',45,2),
(43,'folk',40,'startmusic+folk','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',45,2),
(44,'starwars3',41,'startmusic+starwars3','slitherhorn',1313,'',0,36,10,866729052,0,7,8,4,'healing_music_ability',40,3),
(45,'starwars3',42,'startmusic+starwars3','fizz',1315,'',0,36,10,866729052,0,7,8,4,'healing_music_ability',40,3),
(46,'starwars3',43,'startmusic+starwars3','fanfar',1318,'',0,36,10,866729052,0,7,8,4,'healing_music_ability',45,3),
(47,'starwars3',44,'startmusic+starwars3','kloohorn',1314,'',0,36,10,866729052,0,7,8,4,'healing_music_ability',45,3),
(48,'starwars3',45,'startmusic+starwars3','mandoviol',1323,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',50,5),
(49,'starwars3',46,'startmusic+starwars3','traz',1322,'',0,40,10,866729052,0,9,10,5,'healing_music_ability',50,3),
(50,'starwars3',47,'startmusic+starwars3','bandfill',1316,'',0,44,10,866729052,0,13,10,5,'healing_music_ability',50,1),
(51,'starwars3',48,'startmusic+starwars3','flutedroopy',1317,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',45,1),
(52,'starwars3',49,'startmusic+starwars3','omnibox',1319,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',45,4),
(53,'starwars3',50,'startmusic+starwars3','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',55,2),
(54,'starwars3',50,'startmusic+starwars3','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',55,2),
(55,'ceremonial',51,'startmusic+ceremonial','slitherhorn',1313,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',45,3),
(56,'ceremonial',52,'startmusic+ceremonial','fizz',1315,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',45,3),
(57,'ceremonial',53,'startmusic+ceremonial','fanfar',1318,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',50,3),
(58,'ceremonial',54,'startmusic+ceremonial','kloohorn',1314,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',50,3),
(59,'ceremonial',55,'startmusic+ceremonial','mandoviol',1323,'',0,40,10,866729052,0,9,8,4,'healing_music_ability',55,5),
(60,'ceremonial',56,'startmusic+ceremonial','traz',1322,'',0,40,10,866729052,0,9,10,5,'healing_music_ability',55,3),
(61,'ceremonial',57,'startmusic+ceremonial','bandfill',1316,'',0,44,10,866729052,0,13,10,5,'healing_music_ability',55,1),
(62,'ceremonial',58,'startmusic+ceremonial','flutedroopy',1317,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',50,1),
(63,'ceremonial',59,'startmusic+ceremonial','omnibox',1319,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',50,4),
(64,'ceremonial',60,'startmusic+ceremonial','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',60,2),
(65,'ceremonial',60,'startmusic+ceremonial','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',60,2),
(66,'ballad',61,'startmusic+ballad','slitherhorn',1313,'',0,40,10,866729052,0,11,10,5,'healing_music_ability',60,3),
(67,'ballad',62,'startmusic+ballad','fizz',1315,'',0,40,10,866729052,0,11,10,5,'healing_music_ability',60,3),
(68,'ballad',63,'startmusic+ballad','fanfar',1318,'',0,40,10,866729052,0,11,10,5,'healing_music_ability',65,3),
(69,'ballad',64,'startmusic+ballad','kloohorn',1314,'',0,40,10,866729052,0,11,10,5,'healing_music_ability',65,3),
(70,'ballad',65,'startmusic+ballad','mandoviol',1323,'',0,40,10,866729052,0,11,10,5,'healing_music_ability',70,5),
(71,'ballad',66,'startmusic+ballad','traz',1322,'',0,40,10,866729052,0,11,10,5,'healing_music_ability',70,3),
(72,'ballad',67,'startmusic+ballad','bandfill',1316,'',0,44,10,866729052,0,13,10,5,'healing_music_ability',70,1),
(73,'ballad',68,'startmusic+ballad','flutedroopy',1317,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',65,1),
(74,'ballad',69,'startmusic+ballad','omnibox',1319,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',65,4),
(75,'ballad',70,'startmusic+ballad','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',75,2),
(76,'ballad',70,'startmusic+ballad','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',75,2),
(77,'waltz',71,'startmusic+waltz','slitherhorn',1313,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',70,3),
(78,'waltz',72,'startmusic+waltz','fizz',1315,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',70,3),
(79,'waltz',73,'startmusic+waltz','fanfar',1318,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',75,3),
(80,'waltz',74,'startmusic+waltz','kloohorn',1314,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',75,3),
(81,'waltz',75,'startmusic+waltz','mandoviol',1323,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',80,5),
(82,'waltz',76,'startmusic+waltz','traz',1322,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',80,3),
(83,'waltz',77,'startmusic+waltz','bandfill',1316,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',80,1),
(84,'waltz',78,'startmusic+waltz','flutedroopy',1317,'',0,44,10,866729052,0,15,12,6,'healing_music_ability',75,1),
(85,'waltz',79,'startmusic+waltz','omnibox',1319,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',75,4),
(86,'waltz',80,'startmusic+waltz','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',85,2),
(87,'waltz',80,'startmusic+waltz','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',85,2),
(88,'jazz',81,'startmusic+jazz','slitherhorn',1313,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',85,3),
(89,'jazz',82,'startmusic+jazz','fizz',1315,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',85,3),
(90,'jazz',83,'startmusic+jazz','fanfar',1318,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',90,3),
(91,'jazz',84,'startmusic+jazz','kloohorn',1314,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',90,3),
(92,'jazz',85,'startmusic+jazz','mandoviol',1323,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',95,5),
(93,'jazz',86,'startmusic+jazz','traz',1322,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',95,3),
(94,'jazz',87,'startmusic+jazz','bandfill',1316,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',95,1),
(95,'jazz',88,'startmusic+jazz','flutedroopy',1317,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',90,1),
(96,'jazz',89,'startmusic+jazz','omnibox',1319,'',0,48,10,866729052,0,17,12,6,'healing_music_ability',90,4),
(97,'jazz',90,'startmusic+jazz','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,2),
(98,'jazz',90,'startmusic+jazz','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,2),
(99,'virtuoso',91,'startmusic+virtuoso','slitherhorn',1313,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,3),
(100,'virtuoso',92,'startmusic+virtuoso','fizz',1315,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,3),
(101,'virtuoso',93,'startmusic+virtuoso','fanfar',1318,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,3),
(102,'virtuoso',94,'startmusic+virtuoso','kloohorn',1314,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,3),
(103,'virtuoso',95,'startmusic+virtuoso','mandoviol',1323,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,5),
(104,'virtuoso',96,'startmusic+virtuoso','traz',1322,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,3),
(105,'virtuoso',97,'startmusic+virtuoso','bandfill',1316,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,1),
(106,'virtuoso',98,'startmusic+virtuoso','flutedroopy',1317,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,1),
(107,'virtuoso',99,'startmusic+virtuoso','omnibox',1319,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,4),
(108,'virtuoso',100,'startmusic+virtuoso','nalargon',1320,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,2),
(109,'virtuoso',100,'startmusic+virtuoso','nalargon_max_rebo',1321,'',0,48,10,866729052,0,20,14,7,'healing_music_ability',100,2),
(110,'western',101,'startmusic+western','slitherhorn',1313,'',0,36,6,866729052,0,7,8,4,'healing_music_ability',40,3),
(111,'western',102,'startmusic+western','fizz',1315,'',0,36,6,866729052,0,7,8,4,'healing_music_ability',40,3),
(112,'western',103,'startmusic+western','fanfar',1318,'',0,36,6,866729052,0,7,8,4,'healing_music_ability',45,3),
(113,'western',104,'startmusic+western','kloohorn',1314,'',0,36,6,866729052,0,7,8,4,'healing_music_ability',45,3),
(114,'western',105,'startmusic+western','mandoviol',1323,'',0,40,6,866729052,0,9,8,4,'healing_music_ability',50,5),
(115,'western',106,'startmusic+western','traz',1322,'',0,40,6,866729052,0,9,10,5,'healing_music_ability',50,3),
(116,'western',107,'startmusic+western','bandfill',1316,'',0,44,6,866729052,0,13,10,5,'healing_music_ability',50,1),
(117,'western',108,'startmusic+western','flutedroopy',1317,'',0,44,6,866729052,0,15,12,6,'healing_music_ability',45,1),
(118,'western',109,'startmusic+western','omnibox',1319,'',0,48,6,866729052,0,17,12,6,'healing_music_ability',45,4),
(119,'western',110,'startmusic+western','nalargon',1320,'',0,48,6,866729052,0,20,14,7,'healing_music_ability',55,2),
(120,'western',110,'startmusic+western','nalargon_max_rebo',1321,'',0,48,6,866729052,0,20,14,7,'healing_music_ability',55,2),
(121,'starwars4',111,'startmusic+starwars4','slitherhorn',1313,'',0,36,6,866729052,0,8,9,2,'healing_music_ability',40,3),
(122,'starwars4',112,'startmusic+starwars4','fizz',1315,'',0,36,6,866729052,0,8,9,3,'healing_music_ability',40,3),
(123,'starwars4',113,'startmusic+starwars4','fanfar',1318,'',0,36,6,866729052,0,8,9,3,'healing_music_ability',45,3),
(124,'starwars4',114,'startmusic+starwars4','kloohorn',1314,'',0,36,6,866729052,0,8,9,4,'healing_music_ability',45,3),
(125,'starwars4',115,'startmusic+starwars4','mandoviol',1323,'',0,40,6,866729052,0,10,9,4,'healing_music_ability',50,5),
(126,'starwars4',116,'startmusic+starwars4','traz',1322,'',0,40,6,866729052,0,10,11,5,'healing_music_ability',50,3),
(127,'starwars4',117,'startmusic+starwars4','bandfill',1316,'',0,44,6,866729052,0,14,11,5,'healing_music_ability',50,1),
(128,'starwars4',118,'startmusic+starwars4','flutedroopy',1317,'',0,44,6,866729052,0,16,13,6,'healing_music_ability',45,1),
(129,'starwars4',119,'startmusic+starwars4','omnibox',1319,'',0,48,6,866729052,0,17,12,6,'healing_music_ability',45,4),
(130,'starwars4',120,'startmusic+starwars4','nalargon',1320,'',0,48,6,866729052,0,20,14,7,'healing_music_ability',55,2),
(131,'starwars4',120,'startmusic+starwars4','nalargon_max_rebo',1321,'',0,48,6,866729052,0,20,14,7,'healing_music_ability',55,2),
(132,'funk',121,'startmusic+funk','slitherhorn',1313,'',0,40,6,866729052,0,13,10,5,'healing_music_ability',60,3),
(133,'funk',122,'startmusic+funk','fizz',1315,'',0,40,6,866729052,0,13,10,5,'healing_music_ability',60,3),
(134,'funk',123,'startmusic+funk','fanfar',1318,'',0,40,6,866729052,0,13,10,5,'healing_music_ability',65,3),
(135,'funk',124,'startmusic+funk','kloohorn',1314,'',0,40,6,866729052,0,13,10,5,'healing_music_ability',65,3),
(136,'funk',125,'startmusic+funk','mandoviol',1323,'',0,40,6,866729052,0,13,10,5,'healing_music_ability',70,5),
(137,'funk',126,'startmusic+funk','traz',1322,'',0,40,6,866729052,0,13,10,5,'healing_music_ability',70,3),
(138,'funk',127,'startmusic+funk','bandfill',1316,'',0,44,6,866729052,0,13,10,5,'healing_music_ability',70,1),
(139,'funk',128,'startmusic+funk','flutedroopy',1317,'',0,44,6,866729052,0,13,10,6,'healing_music_ability',65,1),
(140,'funk',129,'startmusic+funk','omnibox',1319,'',0,48,6,866729052,0,17,12,6,'healing_music_ability',65,4),
(141,'funk',130,'startmusic+funk','nalargon',1320,'',0,48,6,866729052,0,20,14,7,'healing_music_ability',75,2),
(142,'funk',130,'startmusic+funk','nalargon_max_rebo',1321,'',0,48,6,866729052,0,20,14,7,'healing_music_ability',75,2),
(143,'basic',0,'','',0,'startDance+basic',1,28,10,-1788534963,0,4,4,2,'healing_dance_ability',5,1),
(144,'rhythmic',0,'','',0,'startDance+rhythmic',3,28,10,-1788534963,0,4,4,2,'healing_dance_ability',5,1),
(145,'basic2',0,'','',0,'startDance+basic2',2,32,10,-1788534963,0,6,6,3,'healing_dance_ability',10,1),
(146,'rhythmic2',0,'','',0,'startDance+rhythmic2',4,32,10,-1788534963,0,8,6,3,'healing_dance_ability',15,1),
(147,'footloose',0,'','',0,'startDance+footloose',15,36,10,-1788534963,0,12,6,3,'healing_dance_ability',20,1),
(148,'formal',0,'','',0,'startDance+formal',17,36,10,-1788534963,0,14,8,4,'healing_dance_ability',30,1),
(149,'footloose2',0,'','',0,'startDance+footloose2',16,40,10,-1788534963,0,18,8,4,'healing_dance_ability',40,1),
(150,'formal2',0,'','',0,'startDance+formal2',18,40,10,-1788534963,0,18,8,4,'healing_dance_ability',40,1),
(151,'popular',0,'','',0,'startDance+popular',9,40,10,-1788534963,0,18,10,5,'healing_dance_ability',50,1),
(152,'poplock',0,'','',0,'startDance+poplock',13,40,10,-1788534963,0,18,10,5,'healing_dance_ability',50,1),
(153,'popular2',0,'','',0,'startDance+popular2',10,40,10,-1788534963,0,22,10,5,'healing_dance_ability',60,1),
(154,'poplock2',0,'','',0,'startDance+poplock2',14,44,10,-1788534963,0,26,10,5,'healing_dance_ability',70,1),
(155,'lyrical',0,'','',0,'startDance+lyrical',11,44,10,-1788534963,0,30,10,6,'healing_dance_ability',80,1),
(156,'exotic',0,'','',0,'startDance+exotic',5,48,10,-1788534963,0,34,10,6,'healing_dance_ability',90,1),
(157,'exotic2',0,'','',0,'startDance+exotic2',6,48,10,-1788534963,0,34,12,6,'healing_dance_ability',90,1),
(158,'lyrical2',0,'','',0,'startDance+lyrical2',12,48,10,-1788534963,0,40,12,7,'healing_dance_ability',100,1),
(159,'exotic3',0,'','',0,'startDance+exotic3',7,48,10,-1788534963,0,40,12,7,'healing_dance_ability',100,1),
(160,'exotic4',0,'','',0,'startDance+exotic4',8,48,10,-1788534963,0,40,14,7,'healing_dance_ability',100,1),
(161,'theatrical',0,'','',0,'startDance+theatrical',21,36,10,-1788534963,0,12,6,3,'healing_dance_ability',50,1),
(162,'theatrical2',0,'','',0,'startDance+theatrical2',22,40,13,-1788534963,0,18,8,4,'healing_dance_ability',70,1),
(163,'breakdance',0,'','',0,'startDance+breakdance',29,40,10,-1788534963,0,22,10,5,'healing_dance_ability',60,1),
(164,'breakdance2',0,'','',0,'startDance+breakdance2',30,44,10,-1788534963,0,26,10,5,'healing_dance_ability',70,1),
(165,'tumble',0,'','',0,'startDance+tumble',31,44,10,-1788534963,0,30,10,6,'healing_dance_ability',80,1),
(166,'tumble2',0,'','',0,'startDance+tumble2',32,48,10,-1788534963,0,34,10,6,'healing_dance_ability',90,1);
/*!40000 ALTER TABLE `entertainer_performances` ENABLE KEYS */;
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
