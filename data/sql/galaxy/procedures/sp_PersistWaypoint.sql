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
    DECLARE does_exist INT;

    select count(*) from waypoint w where w.id = in_object_id into does_exist;
    if does_exist > 0 then
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
    else
        insert into waypoint (id, owner_id, x_coordinate,
            y_coordinate, z_coordinate, is_active,
            planet, name, color)
        values (in_object_id,
            in_owner_id, in_x_coordinate, in_y_coordinate,
            in_z_coordinate, in_is_active, in_planet,
            in_name, in_color);
    end if;
END//
DELIMITER ;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
