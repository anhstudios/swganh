
DROP PROCEDURE IF EXISTS `sp_AllocateIdRange`;

DELIMITER //
CREATE PROCEDURE `sp_AllocateIdRange`(IN `range_size` BIGINT)
BEGIN

	DECLARE object_id BIGINT(20);

	SELECT MAX(id) + range_size FROM object INTO object_id FOR UPDATE;

	IF object_id IS NULL THEN
		SET object_id = 8589934593;
	END IF;
END//