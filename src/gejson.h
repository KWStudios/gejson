#ifndef __GEJSON_H_INCLUDED__
#define __GEJSON_H_INCLUDED__

enum json_type {
	JSON_STRING,
	JSON_NUMBER,
	JSON_OBJECT,
	JSON_ARRAY,
	JSON_TRUE,
	JSON_FALSE,
	JSON_NULL
};

enum gejson_parser_state {
	GEJSON_START,
	GEJSON_KEY_BEFORE,
	GEJSON_KEY,
	GEJSON_KEY_AFTER,
	GEJSON_VALUE_BEFORE,
	GEJSON_VALUE,
	GEJSON_VALUE_AFTER
};

enum gejson_error {
	GEJSON_ERROR_INTERNAL = -256,
	GEJSON_ERROR_NOMEM,
	GEJSON_ERROR_INVALID
};

struct gejson_number {
	/* TODO find a way to make numbers useful
	 * idea: first keep the string,
	 * then let the user convert it to his choice */
};

/* resolve circular reference */
struct gejson_value;

struct gejson_parent {
	enum gejson_type type;
	union {
		struct gejson_array *array;
		struct gejson_obj *object;
	};
};

/* "value" will be a nullpointer in case of true, false
 * and null */
struct gejson_obj {
	unsigned long size;
	char **key;
	union json_value *value;
	struct gejson_parent parent;
};

struct gejson_array {
	unsigned long size;
	struct json_value *value;
	struct gejson_parent parent;
};

struct json_value {
	enum json_type type;
	union {
		char *string;
		struct gejson_number number;
		struct gejson_obj *object;
		struct gejson_array *array;
	};
};

struct gejson_parser {
	struct gejson_obj object;
	struct gejson_parent current;
	enum gejson_parser_state state;
	unsigned long consumed_chars;
};



struct gejson_parser *gejson_create_parser(int flags);
struct gejson_encoder *gejson_create_encoder(int flags);

/* Consumes consecuteiv fragemnts of a json string.
 * Returns 1 if the parsing was successful and the json
 * is not complete yet.
 * Returns 0 if the json has been completed by the
 * current fragment.
 * Returns a negative Value if an error occured */
/* TODO define error types */
int gejson_push_fragment(
		struct gejson_parser* parser,
		char *fragment);

/* Retrieves the gejson_obj after the parsing has finished.
 * Also resets the encoder state,
 * making it ready for a new object */
struct gejson_obj *gejson_finish_parsing(
		struct gejson_parser* paser);

/* resets the parser to a fresh state,
 * discarding all its data */
void gejson_reset_parser(
		struct gejson_parser* parser);

/* frees the memory, used by parser */
/* this makes parser unusable */
void gejson_free_parser(struct gejson_parser *parser);

/* Recursively frees the complete obect,
 * therefore deleting it from memory */
void gejson_free_obj(
		struct gejson_obj);


#endif
