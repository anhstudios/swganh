-- MySQL Administrator dump 1.4
--
-- ------------------------------------------------------
-- Server version	5.5.14


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Create schema galaxy_manager
--

CREATE DATABASE IF NOT EXISTS galaxy_manager;
USE galaxy_manager;

--
-- Definition of table `account`
--

DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) NOT NULL,
  `username_canonical` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `email_canonical` varchar(255) NOT NULL,
  `enabled` tinyint(1) NOT NULL,
  `algorithm` varchar(255) NOT NULL,
  `salt` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `created_at` datetime NOT NULL,
  `updated_at` datetime NOT NULL,
  `last_login` datetime DEFAULT NULL,
  `locked` tinyint(1) NOT NULL,
  `expired` tinyint(1) NOT NULL,
  `expires_at` datetime DEFAULT NULL,
  `confirmation_token` varchar(255) DEFAULT NULL,
  `password_requested_at` datetime DEFAULT NULL,
  `roles` longtext NOT NULL COMMENT '(DC2Type:array)',
  `credentials_expired` tinyint(1) NOT NULL,
  `credentials_expire_at` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UNIQ_7D3656A492FC23A8` (`username_canonical`),
  UNIQUE KEY `UNIQ_7D3656A4A0D96FBF` (`email_canonical`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `account`
--

/*!40000 ALTER TABLE `account` DISABLE KEYS */;
INSERT INTO `account` (`id`,`username`,`username_canonical`,`email`,`email_canonical`,`enabled`,`algorithm`,`salt`,`password`,`created_at`,`updated_at`,`last_login`,`locked`,`expired`,`expires_at`,`confirmation_token`,`password_requested_at`,`roles`,`credentials_expired`,`credentials_expire_at`) VALUES 
 (1,'Kronos','kronos','kronos@swganh.org','kronos@swganh.org',1,'sha512','1iv1y8vzz3dw48w0cwwwk4s8gsscs0g','e63bb34b0b1ece3e78e787c4858b620a73ac9936ccaca62ca7f4dabdbdd004808aae95cc34b395afa8f53470be616eb4d8e12101c1119bbb947695fd8489893a','2011-07-19 21:01:18','2011-07-19 21:01:18',NULL,0,0,NULL,'4n0jl2ajqzs4os4o4wkwock0cgg04coogk8g00o80sgcc04o4g',NULL,'a:0:{}',0,NULL);
/*!40000 ALTER TABLE `account` ENABLE KEYS */;


--
-- Definition of table `appearance`
--

DROP TABLE IF EXISTS `appearance`;
CREATE TABLE `appearance` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `entity_id` int(11) DEFAULT NULL,
  `baseModel` varchar(255) NOT NULL,
  `scale` double NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UNIQ_AAB3829081257D5D` (`entity_id`),
  CONSTRAINT `appearance_ibfk_1` FOREIGN KEY (`entity_id`) REFERENCES `entity` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `appearance`
--

/*!40000 ALTER TABLE `appearance` DISABLE KEYS */;
INSERT INTO `appearance` (`id`,`entity_id`,`baseModel`,`scale`) VALUES 
 (1,1001,'object/creature/player/human_male.iff',1.1),
 (2,1002,'object/creature/player/human_male.iff',1.021);
/*!40000 ALTER TABLE `appearance` ENABLE KEYS */;


--
-- Definition of table `characters`
--

DROP TABLE IF EXISTS `characters`;
CREATE TABLE `characters` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `entity_id` int(11) DEFAULT NULL,
  `createdAt` datetime NOT NULL,
  `updatedAt` datetime NOT NULL,
  `deletedAt` datetime NOT NULL,
  `firstName` varchar(100) NOT NULL,
  `lastName` varchar(100) NOT NULL,
  `jediState` int(11) NOT NULL,
  `birthDate` datetime NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UNIQ_3A29410E81257D5D` (`entity_id`),
  CONSTRAINT `characters_ibfk_1` FOREIGN KEY (`entity_id`) REFERENCES `entity` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `characters`
--

/*!40000 ALTER TABLE `characters` DISABLE KEYS */;
INSERT INTO `characters` (`id`,`entity_id`,`createdAt`,`updatedAt`,`deletedAt`,`firstName`,`lastName`,`jediState`,`birthDate`) VALUES 
 (1,1001,'0000-00-00 00:00:00','0000-00-00 00:00:00','0000-00-00 00:00:00','Kronos','SWGANH',2,'0000-00-00 00:00:00'),
 (2,1002,'0000-00-00 00:00:00','0000-00-00 00:00:00','0000-00-00 00:00:00','Kronos2','Kronos',0,'0000-00-00 00:00:00');
/*!40000 ALTER TABLE `characters` ENABLE KEYS */;


--
-- Definition of table `component`
--

DROP TABLE IF EXISTS `component`;
CREATE TABLE `component` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `label` varchar(255) NOT NULL,
  `description` longtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `component`
--

/*!40000 ALTER TABLE `component` DISABLE KEYS */;
/*!40000 ALTER TABLE `component` ENABLE KEYS */;


--
-- Definition of table `entity`
--

DROP TABLE IF EXISTS `entity`;
CREATE TABLE `entity` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `template_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `UNIQ_E2844685DA0FB8` (`template_id`),
  CONSTRAINT `entity_ibfk_1` FOREIGN KEY (`template_id`) REFERENCES `entity_template` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1003 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `entity`
--

/*!40000 ALTER TABLE `entity` DISABLE KEYS */;
INSERT INTO `entity` (`id`,`template_id`) VALUES 
 (1001,NULL),
 (1002,NULL);
/*!40000 ALTER TABLE `entity` ENABLE KEYS */;


--
-- Definition of table `entity_template`
--

DROP TABLE IF EXISTS `entity_template`;
CREATE TABLE `entity_template` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `label` varchar(255) NOT NULL,
  `description` longtext NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `entity_template`
--

/*!40000 ALTER TABLE `entity_template` DISABLE KEYS */;
/*!40000 ALTER TABLE `entity_template` ENABLE KEYS */;


--
-- Definition of table `galaxy`
--

DROP TABLE IF EXISTS `galaxy`;
CREATE TABLE `galaxy` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `primary_id` int(11) DEFAULT NULL,
  `name` varchar(100) NOT NULL,
  `version` varchar(100) NOT NULL,
  `status` int(11) NOT NULL,
  `created_at` datetime NOT NULL,
  `updated_at` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_F6BB137621DE0C6C` (`primary_id`),
  CONSTRAINT `galaxy_ibfk_1` FOREIGN KEY (`primary_id`) REFERENCES `service` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `galaxy`
--

/*!40000 ALTER TABLE `galaxy` DISABLE KEYS */;
/*!40000 ALTER TABLE `galaxy` ENABLE KEYS */;


--
-- Definition of table `player`
--

DROP TABLE IF EXISTS `player`;
CREATE TABLE `player` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `referenceId` int(11) NOT NULL,
  `maxCharacters` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `player`
--

/*!40000 ALTER TABLE `player` DISABLE KEYS */;
INSERT INTO `player` (`id`,`referenceId`,`maxCharacters`) VALUES 
 (1,1,2);
/*!40000 ALTER TABLE `player` ENABLE KEYS */;


--
-- Definition of table `players_characters`
--

DROP TABLE IF EXISTS `players_characters`;
CREATE TABLE `players_characters` (
  `player_id` int(11) NOT NULL,
  `character_id` int(11) NOT NULL,
  PRIMARY KEY (`player_id`,`character_id`),
  UNIQUE KEY `UNIQ_1C6ECE9A1136BE75` (`character_id`),
  KEY `IDX_1C6ECE9A99E6F5DF` (`player_id`),
  CONSTRAINT `players_characters_ibfk_1` FOREIGN KEY (`player_id`) REFERENCES `player` (`id`),
  CONSTRAINT `players_characters_ibfk_2` FOREIGN KEY (`character_id`) REFERENCES `characters` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `players_characters`
--

/*!40000 ALTER TABLE `players_characters` DISABLE KEYS */;
INSERT INTO `players_characters` (`player_id`,`character_id`) VALUES 
 (1,1),
 (1,2);
/*!40000 ALTER TABLE `players_characters` ENABLE KEYS */;


--
-- Definition of table `scene`
--

DROP TABLE IF EXISTS `scene`;
CREATE TABLE `scene` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `description` longtext NOT NULL,
  `terrain` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `scene`
--

/*!40000 ALTER TABLE `scene` DISABLE KEYS */;
/*!40000 ALTER TABLE `scene` ENABLE KEYS */;


--
-- Definition of table `scenes_entities`
--

DROP TABLE IF EXISTS `scenes_entities`;
CREATE TABLE `scenes_entities` (
  `scene_id` int(11) NOT NULL,
  `entity_id` int(11) NOT NULL,
  PRIMARY KEY (`scene_id`,`entity_id`),
  UNIQUE KEY `UNIQ_4629FE5181257D5D` (`entity_id`),
  KEY `IDX_4629FE51166053B4` (`scene_id`),
  CONSTRAINT `scenes_entities_ibfk_1` FOREIGN KEY (`scene_id`) REFERENCES `scene` (`id`),
  CONSTRAINT `scenes_entities_ibfk_2` FOREIGN KEY (`entity_id`) REFERENCES `entity` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `scenes_entities`
--

/*!40000 ALTER TABLE `scenes_entities` DISABLE KEYS */;
/*!40000 ALTER TABLE `scenes_entities` ENABLE KEYS */;


--
-- Definition of table `service`
--

DROP TABLE IF EXISTS `service`;
CREATE TABLE `service` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `galaxy_id` int(11) DEFAULT NULL,
  `name` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  `address` bigint(20) NOT NULL,
  `tcp_port` int(11) NOT NULL,
  `udp_port` int(11) NOT NULL,
  `ping_port` int(11) NOT NULL,
  `version` int(11) NOT NULL,
  `status` int(11) NOT NULL,
  `last_pulse` decimal(17,3) NOT NULL,
  `created_at` datetime NOT NULL,
  `updated_at` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `IDX_E19D9AD2B61FAB2` (`galaxy_id`),
  CONSTRAINT `service_ibfk_1` FOREIGN KEY (`galaxy_id`) REFERENCES `galaxy` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `service`
--

/*!40000 ALTER TABLE `service` DISABLE KEYS */;
/*!40000 ALTER TABLE `service` ENABLE KEYS */;


--
-- Definition of table `templates_components`
--

DROP TABLE IF EXISTS `templates_components`;
CREATE TABLE `templates_components` (
  `entitytemplate_id` int(11) NOT NULL,
  `component_id` int(11) NOT NULL,
  PRIMARY KEY (`entitytemplate_id`,`component_id`),
  KEY `IDX_9BC18798543FED96` (`entitytemplate_id`),
  KEY `IDX_9BC18798E2ABAFFF` (`component_id`),
  CONSTRAINT `templates_components_ibfk_1` FOREIGN KEY (`entitytemplate_id`) REFERENCES `entity_template` (`id`) ON DELETE CASCADE,
  CONSTRAINT `templates_components_ibfk_2` FOREIGN KEY (`component_id`) REFERENCES `component` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `templates_components`
--

/*!40000 ALTER TABLE `templates_components` DISABLE KEYS */;
/*!40000 ALTER TABLE `templates_components` ENABLE KEYS */;


--
-- Definition of procedure `sp_ReturnAccountCharacters`
--

DROP PROCEDURE IF EXISTS `sp_ReturnAccountCharacters`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_ReturnAccountCharacters`(IN account_id INT)
BEGIN
    SELECT A.id, A.firstName, A.lastName, A.jediState, D.baseModel   
    FROM characters A
    INNER JOIN player B ON (account_id = B.referenceId)
    INNER JOIN players_characters C ON (B.id = C.player_id)
    INNER JOIN appearance D ON (A.entity_id = D.entity_id)
    WHERE A.id = C.character_id;
END $$
/*!50003 SET SESSION SQL_MODE=@TEMP_SQL_MODE */  $$

DELIMITER ;



/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
