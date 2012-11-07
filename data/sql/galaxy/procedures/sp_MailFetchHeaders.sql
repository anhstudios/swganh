
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

DROP PROCEDURE IF EXISTS `sp_MailFetchHeaders`;

DELIMITER //

CREATE PROCEDURE `sp_MailFetchHeaders`(IN receiver_id BIGINT)
BEGIN

    SELECT chat_mail.id,
           chat_mail.sender,
           chat_mail.sender_game,
           chat_mail.sender_galaxy,
           chat_mail.status,
           chat_mail.sent_time,
           chat_mail.subject
    FROM chat_mail
    WHERE chat_mail.receiver_id = receiver_id
    AND chat_mail.deleted = 0;

END//

DELIMITER ;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
