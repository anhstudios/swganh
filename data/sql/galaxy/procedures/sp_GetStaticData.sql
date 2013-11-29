-- --------------------------------------------------------------------------------
-- Routine DDL
-- Note: comments before and after the routine body will not be stored by the server
-- --------------------------------------------------------------------------------
DELIMITER $$

CREATE DEFINER=`swganh`@`192.168.1.%` PROCEDURE `sp_GetStaticData`()
BEGIN
    
	SELECT s.skill_name, s.skill_is_title, s.skill_is_profession, s.skill_money_required,
	s.skill_points_required, xp.xp_type, s.skill_xp_cost, s.skill_xp_cap, s.skill_is_badge FROM skills s
	LEFT JOIN xp_types xp ON xp.id = s.skill_xp_type ORDER BY s.skill_id;

	SELECT s.skill_name, s2.skill_name FROM skills_skill_skillsrequired r
	LEFT JOIN skills s ON r.skill_id = s.skill_id
	LEFT JOIN skills s2 ON r.required_skill = s2.skill_id;

	SELECT s.skill_name, smod.skillmod_name, m.value FROM skills_skillmods m
	LEFT JOIN skills s ON s.skill_id = m.skill_id
	LEFT JOIN skillmods smod ON smod.skillmod_id = m.skillmod_id;

	SELECT s.skill_name, sc.name FROM swganh_static.skills_skillcommands ssc 
	INNER JOIN swganh_static.skillcommands sc ON sc.id = ssc.skillcommand_id 
	INNER JOIN swganh_static.skills s ON ssc.skill_id = s.skill_id
	ORDER BY ssc.skill_id;
END