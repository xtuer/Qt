<sqls namespace="Teacher">
    <sql id="selectAllTeachers">
        SELECT id, name, info FROM teacher
    </sql>

    <sql id="selectTeacherName">
        SELECT name FROM teacher WHERE id=:teacherId
    </sql>
</sqls>
