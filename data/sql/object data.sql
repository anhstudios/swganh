# --------------------------------------------------------
# Host:                         127.0.0.1
# Server version:               5.5.16
# Server OS:                    Win32
# HeidiSQL version:             6.0.0.3603
# Date/time:                    2011-10-08 00:18:02
# --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
# Dumping data for table galaxy.ability_list: ~3 rows (approximately)
/*!40000 ALTER TABLE `ability_list` DISABLE KEYS */;
INSERT IGNORE INTO `ability_list` (`id`, `player_id`, `ability`) VALUES
	(1, 123456789, 'jedi uber master'),
	(2, 123456789, 'jedi healer master'),
	(3, 123456789, 'jedi defender yo');
/*!40000 ALTER TABLE `ability_list` ENABLE KEYS */;

# Dumping data for table galaxy.appearance: ~0 rows (approximately)
/*!40000 ALTER TABLE `appearance` DISABLE KEYS */;
/*!40000 ALTER TABLE `appearance` ENABLE KEYS */;

# Dumping data for table galaxy.building: ~0 rows (approximately)
/*!40000 ALTER TABLE `building` DISABLE KEYS */;
/*!40000 ALTER TABLE `building` ENABLE KEYS */;

# Dumping data for table galaxy.cell: ~0 rows (approximately)
/*!40000 ALTER TABLE `cell` DISABLE KEYS */;
/*!40000 ALTER TABLE `cell` ENABLE KEYS */;

# Dumping data for table galaxy.command: ~0 rows (approximately)
/*!40000 ALTER TABLE `command` DISABLE KEYS */;
/*!40000 ALTER TABLE `command` ENABLE KEYS */;

# Dumping data for table galaxy.creature: ~0 rows (approximately)
/*!40000 ALTER TABLE `creature` DISABLE KEYS */;
/*!40000 ALTER TABLE `creature` ENABLE KEYS */;

# Dumping data for table galaxy.creatures_skills: ~0 rows (approximately)
/*!40000 ALTER TABLE `creatures_skills` DISABLE KEYS */;
/*!40000 ALTER TABLE `creatures_skills` ENABLE KEYS */;

# Dumping data for table galaxy.draft_schematic_list: ~0 rows (approximately)
/*!40000 ALTER TABLE `draft_schematic_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `draft_schematic_list` ENABLE KEYS */;

# Dumping data for table galaxy.factory_crate: ~0 rows (approximately)
/*!40000 ALTER TABLE `factory_crate` DISABLE KEYS */;
/*!40000 ALTER TABLE `factory_crate` ENABLE KEYS */;

# Dumping data for table galaxy.force_sensative_quest_list: ~0 rows (approximately)
/*!40000 ALTER TABLE `force_sensative_quest_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `force_sensative_quest_list` ENABLE KEYS */;

# Dumping data for table galaxy.friend_list: ~1 rows (approximately)
/*!40000 ALTER TABLE `friend_list` DISABLE KEYS */;
INSERT IGNORE INTO `friend_list` (`id`, `player_id`, `friend_id`) VALUES
	(1, 123456789, 12345);
/*!40000 ALTER TABLE `friend_list` ENABLE KEYS */;

# Dumping data for table galaxy.hopper_item: ~0 rows (approximately)
/*!40000 ALTER TABLE `hopper_item` DISABLE KEYS */;
/*!40000 ALTER TABLE `hopper_item` ENABLE KEYS */;

# Dumping data for table galaxy.ignore_list: ~0 rows (approximately)
/*!40000 ALTER TABLE `ignore_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `ignore_list` ENABLE KEYS */;

# Dumping data for table galaxy.installation: ~0 rows (approximately)
/*!40000 ALTER TABLE `installation` DISABLE KEYS */;
/*!40000 ALTER TABLE `installation` ENABLE KEYS */;

# Dumping data for table galaxy.intangible: ~0 rows (approximately)
/*!40000 ALTER TABLE `intangible` DISABLE KEYS */;
/*!40000 ALTER TABLE `intangible` ENABLE KEYS */;

# Dumping data for table galaxy.manufacture_schematic: ~0 rows (approximately)
/*!40000 ALTER TABLE `manufacture_schematic` DISABLE KEYS */;
/*!40000 ALTER TABLE `manufacture_schematic` ENABLE KEYS */;

# Dumping data for table galaxy.mission: ~0 rows (approximately)
/*!40000 ALTER TABLE `mission` DISABLE KEYS */;
/*!40000 ALTER TABLE `mission` ENABLE KEYS */;

# Dumping data for table galaxy.namegen_firstname: ~0 rows (approximately)
/*!40000 ALTER TABLE `namegen_firstname` DISABLE KEYS */;
/*!40000 ALTER TABLE `namegen_firstname` ENABLE KEYS */;

# Dumping data for table galaxy.namegen_lastname: ~0 rows (approximately)
/*!40000 ALTER TABLE `namegen_lastname` DISABLE KEYS */;
/*!40000 ALTER TABLE `namegen_lastname` ENABLE KEYS */;

# Dumping data for table galaxy.name_developer: ~0 rows (approximately)
/*!40000 ALTER TABLE `name_developer` DISABLE KEYS */;
/*!40000 ALTER TABLE `name_developer` ENABLE KEYS */;

# Dumping data for table galaxy.name_profane: ~0 rows (approximately)
/*!40000 ALTER TABLE `name_profane` DISABLE KEYS */;
/*!40000 ALTER TABLE `name_profane` ENABLE KEYS */;

# Dumping data for table galaxy.name_reserved: ~0 rows (approximately)
/*!40000 ALTER TABLE `name_reserved` DISABLE KEYS */;
/*!40000 ALTER TABLE `name_reserved` ENABLE KEYS */;

# Dumping data for table galaxy.object: ~4 rows (approximately)
/*!40000 ALTER TABLE `object` DISABLE KEYS */;
INSERT IGNORE INTO `object` (`id`, `scene_id`, `parent_id`, `shared_template_string`, `x_position`, `y_position`, `z_position`, `x_orientation`, `y_orientation`, `z_orientation`, `w_orientation`, `complexity`, `stf_name_file`, `stf_name_string`, `custom_name`, `volume`, `created_at`, `updated_at`, `deleted_at`, `discr`) VALUES
	(0, 1, NULL, 'shared_object_string', 0, 0, 0, 0, 0, 0, 0, 0, 'object_file', 'object', 'object', 0, '2011-10-05 09:06:05', '2011-10-05 09:06:06', '2011-10-05 09:06:07', 'description'),
	(9999, 1, 123456789, 'waypoint', 1501, 5, 120, 0, 0, 0, 0, 0, 'waypoint', 'waypoint', 'Waypoint for Kronos', 0, '2011-10-07 23:20:48', '2011-10-07 23:20:49', NULL, ''),
	(12345, 1, 0, 'shared_player_human', 12345, 12, 157, 0, 0, 0, 0, 0, 'player', 'human', 'Kronos\' Friend', 0, '2011-10-07 23:18:05', '2011-10-07 23:18:06', '2011-10-07 23:18:06', 'Kronos\' Friend'),
	(123456789, 1, 0, 'shared_player_human', 12345, 12, 157, 0, 0, 0, 0, 0, 'player', 'human', 'Kronos', 0, '0000-00-00 00:00:00', '0000-00-00 00:00:00', '2011-10-07 23:13:38', 'This is Kronos');
/*!40000 ALTER TABLE `object` ENABLE KEYS */;

# Dumping data for table galaxy.player: ~2 rows (approximately)
/*!40000 ALTER TABLE `player` DISABLE KEYS */;
INSERT IGNORE INTO `player` (`id`, `profession_tag`, `born_date`, `total_playtime`, `csr_tag`, `current_force`, `max_force`, `experimentation_enabled`, `crafting_stage`, `nearest_crafting_station`, `experimentation_points`, `accomplishment_counter`, `current_language`, `current_stomach`, `max_stomach`, `current_drink`, `max_drink`, `jedi_state`) VALUES
	(12345, 'friend', '2011-10-07', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	(123456789, 'awesome', '2011-10-07', 0, 0, 2000, 2000, 0, 0, 0, 0, 99, 0, 0, 0, 0, 0, 1);
/*!40000 ALTER TABLE `player` ENABLE KEYS */;

# Dumping data for table galaxy.players_profile_flags: ~1 rows (approximately)
/*!40000 ALTER TABLE `players_profile_flags` DISABLE KEYS */;
INSERT IGNORE INTO `players_profile_flags` (`player_id`, `profile_flag_id`) VALUES
	(123456789, 1);
/*!40000 ALTER TABLE `players_profile_flags` ENABLE KEYS */;

# Dumping data for table galaxy.players_status_flags: ~1 rows (approximately)
/*!40000 ALTER TABLE `players_status_flags` DISABLE KEYS */;
INSERT IGNORE INTO `players_status_flags` (`player_id`, `status_flag_id`) VALUES
	(123456789, 1);
/*!40000 ALTER TABLE `players_status_flags` ENABLE KEYS */;

# Dumping data for table galaxy.player_account: ~0 rows (approximately)
/*!40000 ALTER TABLE `player_account` DISABLE KEYS */;
/*!40000 ALTER TABLE `player_account` ENABLE KEYS */;

# Dumping data for table galaxy.player_accounts_players: ~0 rows (approximately)
/*!40000 ALTER TABLE `player_accounts_players` DISABLE KEYS */;
/*!40000 ALTER TABLE `player_accounts_players` ENABLE KEYS */;

# Dumping data for table galaxy.player_session: ~0 rows (approximately)
/*!40000 ALTER TABLE `player_session` DISABLE KEYS */;
/*!40000 ALTER TABLE `player_session` ENABLE KEYS */;

# Dumping data for table galaxy.profile_flag: ~1 rows (approximately)
/*!40000 ALTER TABLE `profile_flag` DISABLE KEYS */;
INSERT IGNORE INTO `profile_flag` (`id`, `label`, `description`, `flag`) VALUES
	(1, 'profile', 'there\'s some description here', 1);
/*!40000 ALTER TABLE `profile_flag` ENABLE KEYS */;

# Dumping data for table galaxy.quest_journal_list: ~0 rows (approximately)
/*!40000 ALTER TABLE `quest_journal_list` DISABLE KEYS */;
/*!40000 ALTER TABLE `quest_journal_list` ENABLE KEYS */;

# Dumping data for table galaxy.resource: ~0 rows (approximately)
/*!40000 ALTER TABLE `resource` DISABLE KEYS */;
/*!40000 ALTER TABLE `resource` ENABLE KEYS */;

# Dumping data for table galaxy.resource_container: ~0 rows (approximately)
/*!40000 ALTER TABLE `resource_container` DISABLE KEYS */;
/*!40000 ALTER TABLE `resource_container` ENABLE KEYS */;

# Dumping data for table galaxy.scene: ~1 rows (approximately)
/*!40000 ALTER TABLE `scene` DISABLE KEYS */;
INSERT IGNORE INTO `scene` (`id`, `name`, `label`, `description`, `terrain`) VALUES
	(1, 'corellia', 'corellia', 'corellia', 'corellia.trn');
/*!40000 ALTER TABLE `scene` ENABLE KEYS */;

# Dumping data for table galaxy.schematic_customization: ~0 rows (approximately)
/*!40000 ALTER TABLE `schematic_customization` DISABLE KEYS */;
/*!40000 ALTER TABLE `schematic_customization` ENABLE KEYS */;

# Dumping data for table galaxy.schematic_experiment: ~0 rows (approximately)
/*!40000 ALTER TABLE `schematic_experiment` DISABLE KEYS */;
/*!40000 ALTER TABLE `schematic_experiment` ENABLE KEYS */;

# Dumping data for table galaxy.schematic_property: ~0 rows (approximately)
/*!40000 ALTER TABLE `schematic_property` DISABLE KEYS */;
/*!40000 ALTER TABLE `schematic_property` ENABLE KEYS */;

# Dumping data for table galaxy.schematic_slot: ~0 rows (approximately)
/*!40000 ALTER TABLE `schematic_slot` DISABLE KEYS */;
/*!40000 ALTER TABLE `schematic_slot` ENABLE KEYS */;

# Dumping data for table galaxy.ship: ~0 rows (approximately)
/*!40000 ALTER TABLE `ship` DISABLE KEYS */;
/*!40000 ALTER TABLE `ship` ENABLE KEYS */;

# Dumping data for table galaxy.skill: ~0 rows (approximately)
/*!40000 ALTER TABLE `skill` DISABLE KEYS */;
/*!40000 ALTER TABLE `skill` ENABLE KEYS */;

# Dumping data for table galaxy.skills_commands: ~0 rows (approximately)
/*!40000 ALTER TABLE `skills_commands` DISABLE KEYS */;
/*!40000 ALTER TABLE `skills_commands` ENABLE KEYS */;

# Dumping data for table galaxy.skills_preclusion_skills: ~0 rows (approximately)
/*!40000 ALTER TABLE `skills_preclusion_skills` DISABLE KEYS */;
/*!40000 ALTER TABLE `skills_preclusion_skills` ENABLE KEYS */;

# Dumping data for table galaxy.skills_required_skills: ~0 rows (approximately)
/*!40000 ALTER TABLE `skills_required_skills` DISABLE KEYS */;
/*!40000 ALTER TABLE `skills_required_skills` ENABLE KEYS */;

# Dumping data for table galaxy.skills_skill_mods: ~0 rows (approximately)
/*!40000 ALTER TABLE `skills_skill_mods` DISABLE KEYS */;
/*!40000 ALTER TABLE `skills_skill_mods` ENABLE KEYS */;

# Dumping data for table galaxy.skill_mod: ~0 rows (approximately)
/*!40000 ALTER TABLE `skill_mod` DISABLE KEYS */;
/*!40000 ALTER TABLE `skill_mod` ENABLE KEYS */;

# Dumping data for table galaxy.species: ~0 rows (approximately)
/*!40000 ALTER TABLE `species` DISABLE KEYS */;
/*!40000 ALTER TABLE `species` ENABLE KEYS */;

# Dumping data for table galaxy.starting_location: ~0 rows (approximately)
/*!40000 ALTER TABLE `starting_location` DISABLE KEYS */;
/*!40000 ALTER TABLE `starting_location` ENABLE KEYS */;

# Dumping data for table galaxy.status_flag: ~1 rows (approximately)
/*!40000 ALTER TABLE `status_flag` DISABLE KEYS */;
INSERT IGNORE INTO `status_flag` (`id`, `label`, `description`, `flag`) VALUES
	(1, 'status label', 'status description', 1);
/*!40000 ALTER TABLE `status_flag` ENABLE KEYS */;

# Dumping data for table galaxy.tangible: ~1 rows (approximately)
/*!40000 ALTER TABLE `tangible` DISABLE KEYS */;
INSERT IGNORE INTO `tangible` (`id`, `customization`, `options_bitmask`, `incap_timer`, `condition_damage`, `max_condition`, `is_moveable`) VALUES
	(0, '0', 0, 0, 2000, 2000, 1);
/*!40000 ALTER TABLE `tangible` ENABLE KEYS */;

# Dumping data for table galaxy.tangibles_defenders: ~0 rows (approximately)
/*!40000 ALTER TABLE `tangibles_defenders` DISABLE KEYS */;
/*!40000 ALTER TABLE `tangibles_defenders` ENABLE KEYS */;

# Dumping data for table galaxy.waypoint: ~1 rows (approximately)
/*!40000 ALTER TABLE `waypoint` DISABLE KEYS */;
INSERT IGNORE INTO `waypoint` (`id`, `player_id`, `is_active`, `name`, `color`) VALUES
	(0, 123456789, 1, 'Kronos waypoint', 1);
/*!40000 ALTER TABLE `waypoint` ENABLE KEYS */;

# Dumping data for table galaxy.weapon: ~0 rows (approximately)
/*!40000 ALTER TABLE `weapon` DISABLE KEYS */;
/*!40000 ALTER TABLE `weapon` ENABLE KEYS */;

# Dumping data for table galaxy.xp_list: ~1 rows (approximately)
/*!40000 ALTER TABLE `xp_list` DISABLE KEYS */;
INSERT IGNORE INTO `xp_list` (`id`, `player_id`, `xp_type_id`, `value`) VALUES
	(1, 123456789, 1, 500000);
/*!40000 ALTER TABLE `xp_list` ENABLE KEYS */;

# Dumping data for table galaxy.xp_type: ~1 rows (approximately)
/*!40000 ALTER TABLE `xp_type` DISABLE KEYS */;
INSERT IGNORE INTO `xp_type` (`id`, `label`, `name`, `cap`) VALUES
	(1, 'jedi', 'jedi', 9999999);
/*!40000 ALTER TABLE `xp_type` ENABLE KEYS */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
