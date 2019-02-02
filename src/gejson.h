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

struct gejson_parser {
	char* buffer;
};

struct gejson_number {
	/* TODO find a way to make numbers useful
	 * idea: first keep the string,
	 * then let the user convert it to his choice */
};

/* resolve circular reference */
struct gejson_obj;
struct gejson_array;

union json_value {
	char *string;
	struct gejson_number number;
	struct gejson_obj *object;
	struct gejson_array *array;
};

/* "value" will be a nullpointer in case of true, false
 * and null */
struct gejson_obj {
	char *key;
	enum json_type type;
	union json_value value;	
};

struct gejson_array {
	unsigned long size;
	enum json_type *type;
	union json_value *value;
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
		char* fragment);

/* Retrieves the gejson_obj after the parsing has finished.
 * Also resets the encoder state,
 * making it ready for a new object */
struct gejson_obj *gejson_finish_parsing(
		struct gejson_parser* paser);

/* Recursively frees the complete obect,
 * therefore deleting it from memory */
void gejson_free_obj(
		struct gejson_obj);


#endif
