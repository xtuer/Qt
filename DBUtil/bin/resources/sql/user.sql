<?xml version="1.0" encoding="UTF-8"?>

<!--
表结构:
CREATE TABLE `user` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(256) DEFAULT NULL,
  `password` varchar(256) DEFAULT NULL,
  `email`    varchar(256) DEFAULT NULL,
  `mobile`   varchar(64)  DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB;

INSERT INTO `user` (`id`, `username`, `password`, `email`, `mobile`)
VALUES
	(1,'Alice','Passw0rd','alice@gmail.com','18001166441'),
	(2,'Bob','Passw0rd','bob@gmail.com','18001166442'),
	(3,'Carry','Passw0rd','carry@gmail.com','18001166443');
-->
<sqls namespace="User">
    <define id="fields">id, username, password, email, mobile</define>

    <sql id="findByUserId">
        SELECT <include defineId="fields"/> FROM user WHERE id=%1
    </sql>

    <sql id="findAll">
        SELECT id, username, password, email, mobile FROM user
    </sql>

    <sql id="insert">
        INSERT INTO user (username, password, email, mobile)
        VALUES (:username, :password, :email, :mobile)
    </sql>

    <sql id="update">
        UPDATE user SET username=:username, password=:password, email=:email, mobile=:mobile
        WHERE id=:id
    </sql>
</sqls>
