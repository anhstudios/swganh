/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP FUNCTION IF EXISTS `sf_WaypointCreate`;

DELIMITER //

CREATE FUNCTION `sf_WaypointCreate`(
    in_owner_id BIGINT,
    in_x_coordinate DOUBLE,
    in_y_coordinate DOUBLE,
    in_z_coordinate DOUBLE,
    in_is_active TINYINT(1),
    in_planet VARCHAR(255),
    in_name VARCHAR(255),
    in_color SMALLINT(6))
RETURNS BIGINT(20)
BEGIN
    INSERT INTO waypoint
    VALUES(NULL, in_owner_id, in_x_coordinate,
        in_y_coordinate, in_z_coordinate,
         in_is_active, in_planet, in_name, in_color);

    RETURN(LAST_INSERT_ID());
END //

DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
