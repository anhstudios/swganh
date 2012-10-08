/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DELIMITER //
DROP PROCEDURE IF EXISTS `sp_DeleteObject`;
CREATE PROCEDURE `sp_DeleteObject`(IN `object_id` BIGINT, IN `object_type` INT)
BEGIN
	IF 		 object_type = 1112885583	THEN 	/*building*/
		delete from intangible where id = object_id;
	ELSEIF  object_type = 1129465167	THEN 	/*creature*/
		delete from creature where id = object_id;
	ELSEIF  object_type = 1145850183	THEN 	/*guild*/
		delete from guild where id = object_id;
	ELSEIF  object_type = 1178818900	THEN 	/*factory_crate*/
		delete from factory_crate where id = object_id;
	ELSEIF  object_type = 1196578128	THEN 	/*group*/
		delete from galaxy.group where id = object_id;
	ELSEIF  object_type = 1229869903	THEN 	/*installation*/
		delete from installation where id = object_id;
	ELSEIF  object_type = 1230261839	THEN 	/*intangible*/
		delete from intangible where id = object_id;
	ELSEIF  object_type = 1296651087	THEN 	/*mission*/
		delete from misison where id = object_id;
	ELSEIF  object_type = 1297302351	THEN 	/*manufacture_schematic*/
		delete from manufacture_schematic where id = object_id;
	ELSEIF  object_type = 1347174745	THEN 	/*player*/
		delete from player where id = object_id;
	ELSEIF  object_type = 1380142671	THEN 	/*resource_container*/
		delete from resource_container where id = object_id;
	ELSEIF  object_type = 1396919380	THEN 	/*cell*/
		delete from cell where id = object_id;
	ELSEIF  object_type = 1398030671	THEN 	/*static*/
		delete from static where id = object_id;
	ELSEIF  object_type = 1413566031	THEN 	/*tangible*/
		delete from tangible where id = object_id;
	ELSEIF  object_type = 1463900496	THEN 	/*waypoint*/
		delete from waypoint where id = object_id;
	ELSEIF  object_type = 1789660414	THEN 	/*weapon*/
		delete from weapon where id = object_id;
	ELSEIF  object_type = 1874303456	THEN 	/*ship*/
		delete from ship where id = object_id;
	END IF;
	
	delete from object where id = object_id;
	
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
