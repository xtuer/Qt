DROP TABLE IF EXISTS name_value;
CREATE TABLE name_value (
    id integer primary key autoincrement,
    name text,
    value text
);

INSERT INTO name_value (name, value) VALUES ('users', '');
INSERT INTO name_value (name, value) VALUES ('comments', '');
INSERT INTO name_value (name, value) VALUES ('platform', 'product');

DROP TABLE IF EXISTS user;
CREATE TABLE user (
    id integer primary key autoincrement,
    username text,
    nickname text
);

DROP TABLE IF EXISTS comment;
CREATE TABLE comment (
    id integer primary key autoincrement,
    content text
);
