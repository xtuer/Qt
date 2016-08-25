drop table teacher;
create table teacher (
    id      integer primary key autoincrement,
    name    text not null unique,
    info    text
);

drop table clazz;
create table clazz (
    id    integer primary key autoincrement,
    name  text not null unique
);

drop table course;
create table course (
    id   integer primary key autoincrement,
    name text not null unique
);

drop table time_table_item;
create table time_table_item (
    id          integer primary key autoincrement,
    clazz_id    integer,
    teacher_id  integer,
    course_id   integer,
    editable    integer,
    row integer,
    col integer
);

insert into teacher(name) values('Biao');
insert into clazz(name) values('One');
insert into course(name) values('语文');
insert into time_table_item(clazz_id, teacher_id, course_id) values(1, 1, 1);