-- --------------------------------------------------------------------------------
-- Routine DDL
-- Note: comments before and after the routine body will not be stored by the server
-- --------------------------------------------------------------------------------
DELIMITER $$

CREATE DEFINER=`swganh`@`192.168.1.%` PROCEDURE `sp_UpdateExperience`(IN `object_id_` BIGINT, IN `label_` char(35), IN `value_` INT)
BEGIN
DECLARE type_id_ INT(10);
DECLARE id_ INT(10);

SELECT x.id FROM galaxy.xp_list x, galaxy.xp_type t WHERE x.player_id = object_id_ AND x.xp_type_id = t.id AND t.label = label_  INTO id_;
SELECT t.id FROM  galaxy.xp_type t WHERE t.label = label_ INTO type_id_;

IF (id_ = NULL) THEN	
	
	INSERT INTO galaxy.xp_list SET player_id = object_id_, xp_type_id = type_id_, value = value_;
	
ELSEIF (id_ > 0) THEN 

UPDATE galaxy.xp_list x SET x.value = value_ WHERE x.id = id_;
	
END IF;

	
END