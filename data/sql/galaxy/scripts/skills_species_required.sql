
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE TABLE IF NOT EXISTS `skills_species_required` (
  `skill_id` int(11) NOT NULL,
  `species_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`skill_id`,`species_id`),
  KEY `fk_skills_speciesreq_spec_spec` (`species_id`),
  CONSTRAINT `fk_skills_speciesreq_skill_skill` FOREIGN KEY (`skill_id`) REFERENCES `skills` (`skill_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_skills_speciesreq_spec_spec` FOREIGN KEY (`species_id`) REFERENCES `race` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*!40000 ALTER TABLE `skills_species_required` DISABLE KEYS */;
INSERT INTO `skills_species_required` (`skill_id`,`species_id`) VALUES 
 (641,0),
 (642,0),
 (643,0),
 (644,0),
 (677,0),
 (641,1),
 (642,1),
 (643,1),
 (645,1),
 (646,1),
 (647,1),
 (679,1),
 (641,2),
 (642,2),
 (643,2),
 (648,2),
 (649,2),
 (650,2),
 (680,2),
 (641,3),
 (642,3),
 (643,3),
 (651,3),
 (652,3),
 (653,3),
 (678,3),
 (641,4),
 (642,4),
 (643,4),
 (654,4),
 (655,4),
 (656,4),
 (682,4),
 (641,5),
 (642,5),
 (643,5),
 (657,5),
 (658,5),
 (659,5),
 (676,5),
 (641,6),
 (642,6),
 (643,6),
 (660,6),
 (661,6),
 (662,6),
 (681,6),
 (641,7),
 (642,7),
 (643,7),
 (663,7),
 (664,7),
 (665,7),
 (683,7),
 (641,33),
 (642,33),
 (643,33),
 (669,33),
 (670,33),
 (671,33),
 (684,33),
 (641,49),
 (642,49),
 (643,49),
 (672,49),
 (673,49),
 (674,49),
 (685,49);
/*!40000 ALTER TABLE `skills_species_required` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
