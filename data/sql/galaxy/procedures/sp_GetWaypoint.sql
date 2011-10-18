# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-16 19:39:53
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

use galaxy;

# Dumping structure for procedure galaxy.sp_GetWaypoint
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_GetWaypoint`(IN `object_id` BIGINT)
BEGIN
select o.id, o.x_position, o.y_position, o.z_position, o.x_orientation, o.y_orientation, o.z_orientation,
o.w_orientation, o.complexity, o.stf_name_file, o.stf_name_string, o.custom_name, o.volume, o.discr,
w.is_active, w.name, w.color from waypoint w, object o where w.player_id = object_id and o.id = w.id;
END//
DELIMITER ;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
