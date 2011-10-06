# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-05 16:05:31
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
# Dumping data for table galaxy.object: ~1 rows (approximately)
/*!40000 ALTER TABLE `object` DISABLE KEYS */;
INSERT IGNORE INTO `object` (`id`, `scene_id`, `parent_id`, `shared_template_string`, `x_position`, `y_position`, `z_position`, `x_orientation`, `y_orientation`, `z_orientation`, `w_orientation`, `complexity`, `stf_name_file`, `stf_name_string`, `custom_name`, `volume`, `created_at`, `updated_at`, `deleted_at`, `discr`) VALUES
	(0, 1, NULL, 'shared_object_string', 0, 0, 0, 0, 0, 0, 0, 0, 'object_file', 'object', 'object', 0, '2011-10-05 09:06:05', '2011-10-05 09:06:06', '2011-10-05 09:06:07', 'description');
/*!40000 ALTER TABLE `object` ENABLE KEYS */;

# Dumping data for table galaxy.tangible: ~1 rows (approximately)
/*!40000 ALTER TABLE `tangible` DISABLE KEYS */;
INSERT IGNORE INTO `tangible` (`id`, `customization`, `options_bitmask`, `incap_timer`, `condition_damage`, `max_condition`, `is_moveable`) VALUES
	(0, '0', 0, 0, 2000, 2000, 1);
/*!40000 ALTER TABLE `tangible` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
