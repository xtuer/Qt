# 用户表，默认创建了超级管理员

DROP TABLE IF EXISTS user;
CREATE TABLE user (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  username varchar(256) NOT NULL DEFAULT '',
  password varchar(256) NOT NULL DEFAULT '',
  creator  varchar(256) NOT NULL DEFAULT '',
  permission int(11) NOT NULL
) ;

CREATE UNIQUE INDEX IF NOT EXISTS index_user_username ON user (username);
INSERT INTO user(username, password, creator, permission) VALUES('admin', 'admin', 'SUPERMAN', 1);
