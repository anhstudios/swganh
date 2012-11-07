
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP FUNCTION IF EXISTS `sf_MailCreate`;

DELIMITER //

CREATE FUNCTION `sf_MailCreate`(
    sender char(255),
    sender_game char(255),
    sender_galaxy char(255),
    reciever_id bigint(20),
    subject char(255),
    message text,
    attachments text,
    status tinyint(1) unsigned,
    sent_time int)
RETURNS int(11)
BEGIN
    INSERT INTO chat_mail VALUES (NULL, sender, sender_game, sender_galaxy, reciever_id, subject, message, status, sent_time, attachments, 0);
    RETURN(LAST_INSERT_ID());
END //

DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
