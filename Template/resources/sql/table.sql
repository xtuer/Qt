drop table user;
create table user (
    id          integer primary key autoincrement,
    username    text not null unique,
    password    text not null
);
