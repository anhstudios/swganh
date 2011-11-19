# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.3.1-MariaDB
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-16 19:39:50
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

use galaxy;

# Dumping structure for table galaxy.command
CREATE TABLE IF NOT EXISTS `command` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `character_ability` varchar(255) NOT NULL,
  `script_hook` varchar(255) NOT NULL,
  `fail_script_hook` varchar(255) NOT NULL,
  `default_time` int(11) NOT NULL,
  `max_range_to_target` int(11) NOT NULL,
  `add_to_combat_queue` tinyint(1) NOT NULL,
  `health_cost` int(11) NOT NULL,
  `health_cost_multiplier` int(11) NOT NULL,
  `action_cost` int(11) NOT NULL,
  `action_cost_multiplier` int(11) NOT NULL,
  `mind_cost` int(11) NOT NULL,
  `mind_cost_multiplier` int(11) NOT NULL,
  `force_cost` int(11) NOT NULL,
  `force_cost_multiplier` int(11) NOT NULL,
  `damage_multiplier` int(11) NOT NULL,
  `delay_multiplier` int(11) NOT NULL,
  `animation` varchar(255) NOT NULL,
  `required_weapon_group` int(11) NOT NULL,
  `combat_spam` varchar(255) NOT NULL,
  `trail1` int(11) NOT NULL,
  `trail2` int(11) NOT NULL,
  `health_hit_chance` int(11) NOT NULL,
  `action_hit_chance` int(11) NOT NULL,
  `mind_hit_chance` int(11) NOT NULL,
  `knockdown_chance` int(11) NOT NULL,
  `dizzy_chance` int(11) NOT NULL,
  `blind_chance` int(11) NOT NULL,
  `stun_chance` int(11) NOT NULL,
  `intimidate_chance` int(11) NOT NULL,
  `posture_down_chance` int(11) NOT NULL,
  `allow_in_locomotion` bigint(20) NOT NULL,
  `allow_in_posture` bigint(20) NOT NULL,
  `allow_in_state` bigint(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

# Dumping data for table galaxy.command: ~0 rows (approximately)
DELETE FROM `command`;
/*!40000 ALTER TABLE `command` DISABLE KEYS */;
/*!40000 ALTER TABLE `command` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
