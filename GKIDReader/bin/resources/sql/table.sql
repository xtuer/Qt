DROP TABLE IF EXISTS `class`;
CREATE TABLE class (
    id   integer primary key autoincrement,
    name text not null unique
);

DROP TABLE IF EXISTS `course`;
CREATE TABLE course (
    id   integer primary key autoincrement,
    name text not null unique
);

DROP TABLE IF EXISTS `teacher`;
CREATE TABLE teacher (
    id   integer primary key autoincrement,
    name text not null unique,
    info text
);

DROP TABLE IF EXISTS `arrange_course`;
CREATE TABLE arrange_course (
    id   integer primary key autoincrement,
    class_id   integer,
    course_id  integer,
    teacher_id integer,
    count      integer DEFAULT 0
);

DROP TABLE IF EXISTS `schedule_item`;
CREATE TABLE schedule_item (
    id   integer primary key autoincrement,
    _day       integer,
    _time      integer,
    class_id   integer,
    course_id  integer,
    teacher_id integer,
    editable   integer
);
