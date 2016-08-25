<sqls namespace="Class">
    <sql id="selectAllClasses">
        SELECT id, name FROM class ORDER BY name
    </sql>

    <sql id="selectClassName">
        SELECT name FROM class WHERE id=:classId
    </sql>
</sqls>
