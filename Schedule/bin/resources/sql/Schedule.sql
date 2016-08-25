<sqls namespace="Schedule">
    <define id="schedule_item_fields">
        si.id           AS id,
        si._day         AS day,
        si._time        AS time,
        si.editable     AS editable,
        class.id        AS classId,
        class.name      AS className,
        course.id       AS courseId,
        course.name     AS courseName,
        teacher.id      AS teacherId,
        teacher.name    AS teacherName
    </define>

    <!-- 查找所有班级的课程分配 -->
    <sql id="selectAllArrangeCourse">
        SELECT ac.id        AS id,
            ac.count        AS count
            class.id        AS classId,
            class.name      AS className,
            course.id       AS courseId,
            course.name     AS courseName,
            teacher.id      AS teacherId,
            teacher.name    AS teacherName,
        FROM arrange_course ac
        LEFT JOIN class     ON ac.class_id=class.id
        LEFT JOIN course    ON ac.course_id=course.id
        LEFT JOIN teacher   ON ac.teacher_id=teacher.id
        ORDER BY class.id
    </sql>

    <!-- 查找班级的课程分配 -->
    <sql id="selectArrangeCourseByClassId">
        SELECT ac.id        AS id,
            class.id        AS classId,
            class.name      AS className,
            course.id       AS courseId,
            course.name     AS courseName,
            teacher.id      AS teacherId,
            teacher.name    AS teacherName,
            ac.count        AS count
        FROM arrange_course ac
        LEFT JOIN class     ON ac.class_id=class.id
        LEFT JOIN course    ON ac.course_id=course.id
        LEFT JOIN teacher   ON ac.teacher_id=teacher.id
        WHERE ac.class_id=:classId
    </sql>

    <!-- 查找班级的课表 -->
    <sql id="selectScheduleItemsByClassId">
        SELECT <include refId="schedule_item_fields"/>
        FROM schedule_item si
        LEFT JOIN class     ON si.class_id=class.id
        LEFT JOIN course    ON si.course_id=course.id
        LEFT JOIN teacher   ON si.teacher_id=teacher.id
        WHERE si.class_id = :classId
    </sql>

    <!-- 查找老师的课表 -->
    <sql id="selectScheduleItemsByTeacherId">
        SELECT <include refId="schedule_item_fields"/>
        FROM schedule_item si
        LEFT JOIN class     ON si.class_id=class.id
        LEFT JOIN course    ON si.course_id=course.id
        LEFT JOIN teacher   ON si.teacher_id=teacher.id
        WHERE si.teacher_id = :teacherId
    </sql>

    <!-- 同一个老师，同一时间在其他班的课程数量 -->
    <sql id="countOfScheduleItemInOtherClassByTeacherId">
        SELECT count(si.id) FROM schedule_item si, class
        WHERE si.teacher_id=:teacherId AND si._day=:day AND si._time=:time AND si.class_id!=:classId AND si.class_id=class.id AND si.teacher_id!=0
    </sql>

    <!-- 插入课表 -->
    <sql id="insertScheduleItem">
        INSERT INTO schedule_item (_day, _time, class_id, course_id, teacher_id, editable)
        VALUES (:day, :time, :classId, :courseId, :teacherId, :editable)
    </sql>

    <!-- 删除班级的课表 -->
    <sql id="deleteScheduleItemsByClassId">
        DELETE FROM schedule_item WHERE class_id=:classId
    </sql>
</sqls>
