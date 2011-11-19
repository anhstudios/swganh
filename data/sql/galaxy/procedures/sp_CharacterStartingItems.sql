/*
---------------------------------------------------------------------------------------
This source file is part of SWG:ANH (Star Wars Galaxies - A New Hope - Server Emulator)

For more information, visit http://www.swganh.com

Copyright (c) 2006 - 2010 The SWG:ANH Team
---------------------------------------------------------------------------------------
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
---------------------------------------------------------------------------------------
*/

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


--
-- Use schema swganh
--

--
-- Definition of procedure `sp_CharacterStartingItems`
--

DROP PROCEDURE IF EXISTS `sp_CharacterStartingItems`;

DELIMITER $$

/*!50003 SET @TEMP_SQL_MODE=@@SQL_MODE, SQL_MODE='STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER' */ $$
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_CharacterStartingItems`(
	IN owner_id BIGINT(20),
	IN race_id INT(8),
	IN profession_id INT(8),
	IN gender_id INT(3))
BEGIN

    DECLARE object_id BIGINT(20);
	DECLARE tmpId BIGINT(20);
    DECLARE iff_template_id INT;
    DECLARE template CHAR(255);
    DECLARE loopEnd INT DEFAULT 0;
    DECLARE cur_1 CURSOR FOR SELECT creation_item.template FROM creation_item WHERE creation_item.species_id=race_id AND creation_item.gender=gender_id AND creation_item.profession_id=profession_id;
    DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET loopEnd = 1;

	OPEN cur_1;
        REPEAT
            FETCH cur_1 INTO template;
            IF NOT loopEnd THEN
                SELECT MAX(id) + 10 FROM object INTO object_id FOR UPDATE;

                SELECT iff_templates.id FROM iff_templates WHERE iff_templates.iff_template LIKE template INTO iff_template_id;

                INSERT INTO `object` VALUES (object_id, NULL, owner_id, iff_template_id, 0,0,0,0,0,0,1, 0, '', '', '' ,0, NOW(), NOW(), null, 1413566031);
                INSERT INTO `tangible` VALUES (object_id, '', 0, 0, 0, 0, 0);
            END IF;
        UNTIL loopEnd END REPEAT;
    CLOSE cur_1;
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