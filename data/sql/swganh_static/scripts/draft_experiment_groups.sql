-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.1.63-community - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL version:             7.0.0.4053
-- Date/time:                    2012-10-14 17:59:16
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET FOREIGN_KEY_CHECKS=0 */;

-- Dumping structure for table swganh_static.draft_experiment_groups
DROP TABLE IF EXISTS `draft_experiment_groups`;
CREATE TABLE IF NOT EXISTS `draft_experiment_groups` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL DEFAULT 'exp',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=81 DEFAULT CHARSET=utf8;

-- Dumping data for table swganh_static.draft_experiment_groups: ~80 rows (approximately)
/*!40000 ALTER TABLE `draft_experiment_groups` DISABLE KEYS */;
INSERT INTO `draft_experiment_groups` (`id`, `name`) VALUES
	(1, 'exp_accel'),
	(2, 'exp_ammo'),
	(3, 'exp_armor'),
	(4, 'exp_armor_effectiveness'),
	(5, 'exp_armorHpMax'),
	(6, 'exp_booster_accel'),
	(7, 'exp_booster_acceleration'),
	(8, 'exp_booster_consumption'),
	(9, 'exp_booster_energy'),
	(10, 'exp_booster_recharge'),
	(11, 'exp_booster_speed'),
	(12, 'exp_cap_max_energy'),
	(13, 'exp_charges'),
	(14, 'exp_count'),
	(15, 'exp_damage_max'),
	(16, 'exp_damage_min'),
	(17, 'exp_decel'),
	(18, 'exp_droid_command_speed'),
	(19, 'exp_droid_speed'),
	(20, 'exp_durability'),
	(21, 'exp_effective_armor'),
	(22, 'exp_effective_shields'),
	(23, 'exp_effectiveness'),
	(24, 'exp_energy_gen_rate'),
	(25, 'exp_energy_maintenance'),
	(26, 'exp_energy_per_shot'),
	(27, 'exp_engine_accel'),
	(28, 'exp_engine_decel'),
	(29, 'exp_engine_speed'),
	(30, 'exp_filling'),
	(31, 'exp_flavor'),
	(32, 'exp_fltrefirerate'),
	(33, 'exp_hitPointsMax'),
	(34, 'exp_hp'),
	(35, 'exp_hp_max'),
	(36, 'exp_hpMax'),
	(37, 'exp_maintenance'),
	(38, 'exp_mass'),
	(39, 'exp_massMax'),
	(40, 'exp_maximum_chaff_effectiveness'),
	(41, 'exp_min_chaff_effectiveness'),
	(42, 'exp_nutrition'),
	(43, 'exp_pitch'),
	(44, 'exp_quality'),
	(45, 'exp_recharge'),
	(46, 'exp_refire'),
	(47, 'exp_refire_rate'),
	(48, 'exp_repairCharges'),
	(49, 'exp_resistance'),
	(50, 'exp_roll'),
	(51, 'exp_shield_effectiveness'),
	(52, 'exp_shield_max_back_hp'),
	(53, 'exp_shield_max_front_hp'),
	(54, 'exp_shield_recharge'),
	(55, 'exp_speed'),
	(56, 'exp_yaw'),
	(57, 'expAbsorption'),
	(58, 'expAggressionProfile'),
	(59, 'expCharges'),
	(60, 'expDamage'),
	(61, 'expDurability'),
	(62, 'expDuration'),
	(63, 'expEaseOfUse'),
	(64, 'expEffeciency'),
	(65, 'expEffectiveness'),
	(66, 'expIntelligence'),
	(67, 'expMentalProfile'),
	(68, 'expPhysiqueProfile'),
	(69, 'expProwessProfile'),
	(70, 'expPsychologicalProfile'),
	(71, 'expQuality'),
	(72, 'expRange'),
	(73, 'expResistance'),
	(74, 'expSpeed'),
	(75, 'expStorage'),
	(76, 'exp_quantity'),
	(77, 'fs_exp_cure_one'),
	(78, 'fs_exp_cure_two'),
	(79, 'fs_exp_side_effect_one'),
	(80, 'needs_setup');
/*!40000 ALTER TABLE `draft_experiment_groups` ENABLE KEYS */;
/*!40014 SET FOREIGN_KEY_CHECKS=1 */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
