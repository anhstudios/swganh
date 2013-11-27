-- --------------------------------------------------------------------------------
-- Routine DDL
-- Note: comments before and after the routine body will not be stored by the server
-- --------------------------------------------------------------------------------
DELIMITER $$

CREATE DEFINER=`swganh`@`192.168.1.%` PROCEDURE `sp_PersistSkill`(IN `creature_id_` BIGINT, IN `skill_` varchar(45))
BEGIN
DECLARE creature_skill_id_ int;
SELECT skill.id FROM skill WHERE skill.name = skill_ INTO creature_skill_id_;
IF NOT EXISTS(SELECT * FROM creatures_skills WHERE creatures_skills.creature_id = creature_id_ and  creatures_skills.skill_id = creature_skill_id_) THEN
	INSERT INTO creatures_skills SET creature_id = creature_id_, skill_id = creature_skill_id_;
END IF;
END