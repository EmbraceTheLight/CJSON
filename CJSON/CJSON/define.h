#pragma once

// ����JSONֵ��ö������
enum ValueType {
    STRING,
    INTEGER,
    FLOAT,
    BOOLEAN,
    ARRAY,
    OBJECT
};

typedef struct JsonValue {
    enum Valuetype type;
    union {
        char* string;
        double number;
        bool boolean;
        void* null;
        struct JsonValue* Array;
        struct JsonValue* Object;
    };
}JsonValue;

typedef struct KeyValue {
    char* key;
    JsonValue value;
}KV;

