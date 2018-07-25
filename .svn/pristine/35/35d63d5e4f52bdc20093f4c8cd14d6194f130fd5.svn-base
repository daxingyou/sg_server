CREATE DEFINER=`root`@`%` PROCEDURE `sp_insert_test`(insert_count INT(11))
BEGIN 

DECLARE i INT(11) DEFAULT 0;

DECLARE account_uid BIGINT(11) DEFAULT 0;
DECLARE channel_id INT(11) DEFAULT 0;
DECLARE role_uid BIGINT(11) DEFAULT 0;
DECLARE mobile_uuid BIGINT(11) DEFAULT 0;
DECLARE source_type INT(11) DEFAULT 0;
DECLARE create_date VARCHAR(20) DEFAULT '0000-00-00';

truncate table sg_glog.`account_log`;
truncate table sg_glog.`mobile_log`;

WHILE i < insert_count do
    
    SET account_uid = FLOOR(RAND() * (insert_count - 1) + 1);
    SET source_type = FLOOR(RAND()*3 + 1460);#1460-1462
    IF source_type = 1462 THEN
        SET role_uid = FLOOR(RAND() * (insert_count*2 - (insert_count+1)) + (insert_count+1));
        SET mobile_uuid = 0;
    ELSE
        SET role_uid = 0;
        IF i < (insert_count/2) THEN
            SET mobile_uuid = FLOOR(RAND() * (insert_count*3 - (insert_count*2+1)) + (insert_count*2+1));
        ELSE 
            SET mobile_uuid = 0;
        END IF;
    END IF;
    #SET create_date = CONCAT('2017', '-', LPAD(FLOOR(RAND() * 12 + 1), 2, 0), '-', LPAD(FLOOR(RAND() * 30 + 1), 2, 0));
    SET create_date = CONCAT('2017', '-', '06', '-', LPAD(FLOOR(RAND() * 30 + 1), 2, 0));
    
    INSERT INTO sg_glog.`account_log`(`account_uid`, `channel_id`, `role_uid`, `mobile_uuid`, `source_type`, `create_date`)
    VALUES(account_uid, channel_id, role_uid, mobile_uuid, source_type, create_date);

    IF mobile_uuid > 0 THEN
        INSERT IGNORE INTO sg_glog.`mobile_log`(`uuid`, `create_date`) VALUES(mobile_uuid, create_date);
    END IF;
    
    SET i = i + 1;
    
END WHILE;

SELECT COUNT(`id`) FROM sg_glog.`account_log`;
SELECT COUNT(`id`) FROM sg_glog.`mobile_log`;

END;