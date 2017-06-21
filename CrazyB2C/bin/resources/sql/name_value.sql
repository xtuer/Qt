<?xml version="1.0" encoding="UTF-8"?>
<sqls namespace="NameValue">
    <sql id="getAllNameValues">
        SELECT id, name, value FROM name_value
    </sql>

    <sql id="findValueByName">
        SELECT value FROM name_value WHERE name=:name
    </sql>

    <sql id="insertNameValue">
        INSERT INTO name_value (name, value)
        VALUES (:name, :value)
    </sql>

    <sql id="updateNameValue">
        UPDATE name_value SET value=:value WHERE name=:name
    </sql>
</sqls>
