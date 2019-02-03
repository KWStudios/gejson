#include "gejson.h"

#include <stdlib.h>

#define IS_WHITESPACE(X) \
	(X == '\x20' || X == '\x09' || X == '\x0A' || X =='\x0D') 

struct gejson_parser *gejson_create_parser(int flags)
{
	struct gejson_parser *parser;
	parser = calloc(1, sizeof(struct gejson_parser));

	return parser;
}

int gejson_push_fragmet(struct gejson_parser *parser, char *fragment)
{
	int incomplete = 1;
	for(unsigned int i = 0; fragment[i] != '\0'; i++) {
		parser->consumed_chars++;
		if ((parser->state == GEJSON_START ||
		     parser->state == GEJSON_OBJECT_START ||
		     parser->state == GEJSON_ARRAY_START) &&
		     IS_WHITESPACE(fragment[i]) ) {
			continue;
		}
		switch(parser->state) {
		case GEJSON_START:
			/* this indicates that there is no parent */
			parser->object.parent.type = JSON_NULL;
			if(fragment[i] == '{' && fragment[i] != '[') {
				parser->state = GEJSON_OBJECT_START;
				parser->current_element = &parser->object;
				continue;
			} else if(fragment[0] == '[') {
				parser->object.size = 1;
				parser->object.key = malloc(sizeof(char *));
				if(parser->object.key == 0)
					return GEJSON_ERROR_NOMEM;
				*parser->object.key = NULL;
				parser->object.value =
					calloc(1, sizeof(struct json_value));
				if(parser->object.value == 0)
					return GEJSON_ERROR_NOMEM;
				parser->object.value->type = JSON_ARRAY;
				parser->object.value->array =
					calloc(1, sizeof(struct gejson_array));
				if(parser->object.value.array == 0)
					return GEJSON_ERROR_NOMEM;
				praser->current_element = 
					parser->object.value.array;
				parser->state = GEJSON_ARRAY_START;
				continue;
			} else {
				return GEJSON_ERROR_INVALID;
			}
			break;
		case GEJSON_OBJECT_START:
			struct gejson_obj object = parser->current_element;
			if(fragment[i] == '\"') {
				parser->state = GEJSON_OBJECT_KEY;
				continue;
			} else if(fragment[i] == '}') {
				switch(oject->parent.type) {
				case JSON_NULL:
					return 0;
					break;
				case JSON_OBJECT:
					parser->current_element =
						object->parent.object;
					parser->state = 
						GEJSON_OBJECT_AFTERVALUE;
					break;
				case JSON_ARRAY:
					parser->current_element =
						object->parent.array;
					parser->state = 
						GEJSON_ARRAY_AFTERVALUE;
					break;
				default:
					return GEJSON_ERROR_INTERNAL;
				}
			} else {
				return GEJSON_ERROR_INVALID;
			}
			break;
		case GEJSON_ARRAY_START:
			struct gejson_array array = parser->current_element;
			if(fragment[i] == ']') {
				switch(array->parent.type) {
				case JSON_NULL:
					return 0;
					break;
				case JSON_OBJECT:
					parser->current_element =
						array->parent.object;
					parser->state = 
						GEJSON_OBJECT_AFTERVALUE;
					break;
				case JSON_ARRAY:
					parser->current_element =
						array->parent.array;
					parser->state = 
						GEJSON_ARRAY_AFTERVALUE;
					break;
				default:
					return GEJSON_ERROR_INTERNAL;
			} else {
				parser->state = GEJSON_DECIDEVALUE;
			break;
		case GEJSON_OBJECT_KEY:
			struct gejson_object *object = parser->current_element;
			if(object->key[size - 1] == NULL) {
				object->key[size - 1] = calloc(2, sizeof(char));
				if(object->key[size - 1] == NULL)
					return GEJSON_ERROR_NOMEM;
			}
			if(string_incrementor(
					object->key[size - 1],
					fragment[i] )) {
				parser->state = GEJSON_OBJECT_AFTERVALUE;
			} else {
				object->key[size - 1] =
					realloc(object->key[size - 1],
						strlen(object->key[size - 1]));
				if(object->key[size - 1] == NULL)
					return GEJSON_ERROR_NOMEM;
				object->key[size - 1][strlen(
						object->key[size - 1]) + 1] =
					'\0';
			}
			break;
		case GEJSON_OBJECT_AFTERVALUE:
			break;
		case GEJSON_DECIDEVALUE:
			break;
		default:
			return GEJSON_ERROR_INTERNAL;
		}
	}

	return incomplete;
}

/* Returns 1 if string is complete.
 * Returns 0 if string is incoplete */
int string_incrementor(char *string, char c)
{
	if(c == '"' && string[strlen(string) - 1] != '\\') {
		return 1;
	} else {
		string[strlen(string)] = c;
		return 0;
	}
}
