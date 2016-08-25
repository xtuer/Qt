<sqls namespace="User">
    <define id="fields">id, username, password</define>

    <sql id="selectById">
        SELECT <include refid="fields"/> FROM user WHERE id=%1
    </sql>

    <sql id="selectAll">
        SELECT id, username, password FROM user
    </sql>

    <sql id="insert">
        INSERT INTO user (username, password) VALUES (:username, :password)
    </sql>

    <sql id="update">
        UPDATE user SET username=:username, password=:password WHERE id=:id
    </sql>
</sqls>
