/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

DROP PROCEDURE IF EXISTS `sp_PersistWaypoint`;
DELIMITER //
CREATE PROCEDURE `sp_PersistWaypoint`(
    IN `in_object_id` bigint,
    IN `in_owner_id` bigint,
    IN `in_x_coordinate` double,
    IN `in_y_coordinate` double,
    IN `in_z_coordinate` double,
    IN `in_is_active` tinyint(1),
    IN `in_planet` varchar(255),
    IN `in_name` varchar(255),
    IN `in_color` smallint(6))
BEGIN

    update waypoint w
    set w.owner_id = in_owner_id,
        w.x_coordinate = in_x_coordinate,
        w.y_coordinate = in_y_coordinate,
        w.z_coordinate = in_z_coordinate,
        w.is_active = in_is_active,
        w.planet = in_planet,
        w.name = in_name,
        w.color = in_color
    where w.id = in_object_id;

END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
