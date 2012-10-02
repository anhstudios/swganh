/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_DeleteObject`;
DELIMITER //
CREATE PROCEDURE `sp_DeleteObject`(IN `object_id` BIGINT, IN `object_type` INT)
BEGIN
IF 		 object_type = 1112885583	THEN 	--building
	delete from intangible i where i.id = object_id;
ELSE IF  object_type = 1129465167	THEN 	--creature
	delete from creature c where c.id = object_id;
ELSE IF  object_type = 1145850183	THEN 	--guild
	delete from guild g where g.id = object_id;
ELSE IF  object_type = 1178818900	THEN 	--factory_crate
	delete from factory_crate fc where fc.id = object_id;
ELSE IF  object_type = 1196578128	THEN 	--group
	delete from group gr where gr.id = object_id;
ELSE IF  object_type = 1229869903	THEN 	--installation
	delete from installation inst where inst.id = object_id;
ELSE IF  object_type = 1230261839	THEN 	--intangible
	delete from intangible intan where intan.id = object_id;
ELSE IF  object_type = 1296651087	THEN 	--mission
	delete from misison m where m.id = object_id;
ELSE IF  object_type = 1297302351	THEN 	--manufacture_schematic
	delete from manufacture_schematic mfs where mfs.id = object_id;
ELSE IF  object_type = 1347174745	THEN 	--player
	delete from player p where p.id = object_id;
ELSE IF  object_type = 1380142671	THEN 	--resource_container
	delete from resource_container rc where rc.id = object_id;
ELSE IF  object_type = 1396919380	THEN 	--cell
	delete from cell c where c.id = object_id;
ELSE IF  object_type = 1398030671	THEN 	--static
	delete from static st where st.id = object_id;
ELSE IF  object_type = 1413566031	THEN 	--tangible
	delete from tangible tano where tano.id = object_id;
ELSE IF  object_type = 1463900496	THEN 	--waypoint
	delete from waypoint way where way.id = object_id;
ELSE IF  object_type = 1789660414	THEN 	--weapon
	delete from weapon w where w.id = object_id;
ELSE IF  object_type = 1874303456	THEN 	--ship
	delete from ship sh where sh.id = object_id;

delete from object o where o.id = object_id;
	
END IF;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
