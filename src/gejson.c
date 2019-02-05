#include "gejson.h"

#include <string.h>
#include <stdlib.h>

#define IS_WHITESPACE(X) \
	(X == '\x20' || X == '\x09' || X == '\x0A' || X =='\x0D') 

struct gejson_parser *gejson_create_parser(int flags)
{
	struct gejson_parser *parser;
	parser = calloc(1, sizeof(struct gejson_parser));

	return parser;
}

int gejson_start(struct gejson_parser *parser, char c);
int gejson_key_before(struct gejson_parser *parser, char c);
int gejson_key(struct gejson_parser *parser, char c);
int gejson_key_after(struct gejson_parser *parser, char c);
int gejson_value_before(struct gejson_parser *parser, char c);
int gejson_value(struct gejson_parser *parser, char c);
int gejson_value_after(struct gejson_parser *parser, char c);

int string_incrementor(char *string, char c);


int gejson_push_fragment(struct gejson_parser *parser, char *fragment)
{
	for(unsigned int i = 0; fragment[i] != '\0'; i++) {
		int result;
		parser->consumed_chars++;
		if(parser->state != GEJSON_KEY &&
				parser->state != GEJSON_VALUE &&
				IS_WHITESPACE(fragment[i])) {
			continue;
		}

		switch(parser->state) {
		case GEJSON_START:
			result = gejson_start(parser, fragment[i]);
			break;
		case GEJSON_KEY_BEFORE:
			result = gejson_key_before(parser, fragment[i]);
			break;
		case GEJSON_KEY:
			result = gejson_key(parser, fragment[i]);
			break;
		case GEJSON_KEY_AFTER:
			result = gejson_key_after(parser, fragment[i]);
			break;
		case GEJSON_VALUE_BEFORE:
			result = gejson_value_before(parser, fragment[i]);
			break;
		case GEJSON_VALUE:
			result = gejson_value(parser, fragment[i]);
			break;
		case GEJSON_VALUE_AFTER:
			result = gejson_value_after(parser, fragment[i]);
			break;
		default: 
			return GEJSON_ERROR_INTERNAL;
		}
		if(result != 1)
			return result;
	}
	return 1;
}

int gejson_start(struct gejson_parser *parser, char c)
{
	/* this indicates that there is no parent */
	parser->object.parent.type = JSON_NULL;
	if(c == '{') {
		parser->current.type = JSON_OBJECT;
		parser->current.object = &parser->object;
		parser->state = GEJSON_KEY_BEFORE;
		return 1;
	} else if(c == '[') {
		/* Make a new array as child of the top object.
		 * The key will be NULL because it is not an actual child
		 * but the representation does not allow a toplevel array */
		parser->object.size = 1;
		parser->object.key = malloc(sizeof(char *));
		if(parser->object.key == 0)
			return GEJSON_ERROR_NOMEM;
		parser->object.key[0] = NULL;
		parser->object.value =
			calloc(1, sizeof(struct gejson_value));
		if(parser->object.value == 0)
			return GEJSON_ERROR_NOMEM;
		parser->object.value->type = JSON_ARRAY;
		parser->object.value->array =
			calloc(1, sizeof(struct gejson_array));
		if(parser->object.value.array == 0)
			return GEJSON_ERROR_NOMEM;
		praser->current_element = 
			parser->object.value.array;
		parser->state = GEJSON_VALUE_BEFORE;
		return 1;
	} else {
		return GEJSON_ERROR_INVALID;
	}
}

int gejson_key_before(struct gejson_parser *parser, char c)
{
	struct gejson_obj *object = parser->current.object;
	if(c == '\"') {
		parser->state = GEJSON_KEY;
		return 1;
	} else if(c == '}') {
		switch(oject->parent.type) {
		case JSON_NULL:
			return 0;
			break;
		case JSON_OBJECT:
			parser->current.type = GEJSON_OBJECT;
			parser->current.object = object->parent.object;
			break;
		case JSON_ARRAY:
			parser->current.type = GEJSON_ARRAY;
			parser->current.array = object->parent.array;
			break;
		default:
			return GEJSON_ERROR_INTERNAL;
		}
		parser->state = GEJSON_VALUE_AFTER;
		return 1;
	}
	return GEJSON_ERROR_INVALID;
}

int gejson_key(struct gejson_parser *parser, char c)
{
	struct gejson_obj *object = parser->current.object;
	if(object->key[size - 1] == NULL) {
		object->key[size - 1] = calloc(2, sizeof(char));
		if(object->key[size - 1] == NULL)
			return GEJSON_ERROR_NOMEM;
	}
	if(string_incrementor(object->key[size - 1], c)) {
		parser->state = GEJSON_KEY_AFTER;
	} else {
		object->key[size - 1] = realloc(
				object->key[size - 1],
				strlen(object->key[size - 1]));
		if(object->key[size - 1] == NULL)
			return GEJSON_ERROR_NOMEM;
		object->key[size - 1][strlen(object->key[size - 1]) + 1] = '\0';
	}
	return 1;
}

int gejson_key_after(struct gejson_parser *parser, char c)
{
	if(c == ':') {
		parser->state = GEJSON_VALUE_BEFORE;
		return 1;
	}
	return GEJSON_ERROR_INVALID;
}

int gejson_value_before(struct gejson_parser *parser, char c)
{
	struct gejson_parent *current = &parser->current;
	struct gejson_value *value;
	unsigned long *ao_size;
	/* fill the local value nd size depending on the type of the parent */
	if(current->type == JSON_ARRAY) {
		struct gejson_array *array = current->array;
		value = array->value;
		ao_size = &array->size;
	} else if(current->type == JSON_OBJECT) {
		struct gejson_object *object = current->object;
		value = object->value;
		ao_size = &object->size;
	} else {
		return GEJSON_ERROR_INTERNAL;
	}
	/* reserve a value in the current object */
	if(*size == 0) {
		value = calloc(1, sizeof(struct gejson_value));
		if(value == 0)
			return GEJSON_ERROR_NOMEM;
	} else {
		value = realloc(*size + 1, sizeof(struct gejson_value));
		memset(value + *size,
				0,
				sizeof(struct gejson_value));
		if(value == 0)
			return GEJSON_ERROR_NOMEM;
	}
	value += *size;
	(*size)++;

	if(c >= '0' && c <= '9' || c == '-') {
		/* start of number */
		value->type = JSON_NUMBER;
		value->number.string = calloc(2, sizeof(char));
		if(value->number.string == NULL)
			return GEJSON_ERROR_NOMEM;
		value->number.string[0] = c;

		parser->state = GEJSON_VALUE;
	} else {
		switch(c) {
		case '\"':
			/* start of string */
			value->type = JSON_STRING;
			value->string = calloc(2, sizeof(char));
			if(value->string == NULL)
				return GEJSON_ERROR_NOMEM;
			value->string[0] = c;

			parser->state = GEJSON_VALUE;
			break;
		case '{':
			/* start of object */
			value->type = JSON_OBJECT;
			value->object = calloc(1, sizeof(struct gejson_object));
			if(value->object == NULL)
				return GEJSON_ERROR_NOMEM;
			value->object->parent = current;
			current->type = GEJSON_OBJECT;
			current->object = value;

			parser->state = GJESON_KEY_BEFORE;
			break;
		case '[':
			/* start of array */
			value->type = JSON_ARRAY;
			value->array = calloc(1, sizeof(struct gejson_array));
			if(value->array == NULL)
				return GEJSON_ERROR_NOMEM;
			value->array->parent = current;
			current->type = GEJSON_ARRAY;
			current->array = value->array;

			parser->state = GEJSON_VALUE_BEFORE;
			break;
		case 't':
			/* start of true */
			/* the string element of value will be abused here
			 * to check for incorrect "true", "false" ans "null"
			 * values */
			value->type = JSON_TRUE;
			value->string = calloc(2, sizeof(char));
			if(value->string == NULL)
				return GEJSON_ERROR_NOMEM;
			value->string[0] = c;

			parser->state = GEJSON_VALUE;
			break;
		case 'f':
			/* start of false */
			value->type = JSON_FALSE;
			value->string = calloc(2, sizeof(char));
			if(value->string == NULL)
				return GEJSON_ERROR_NOMEM;
			value->string[0] = c;

			parser->state = GEJSON_VALUE;
			break;
		case 'n':
			/* start of null */
			value->type = JSON_NULL;
			value->string = calloc(2, sizeof(char));
			if(value->string == NULL)
				return GEJSON_ERROR_NOMEM;
			value->string[0] = c;

			parser->state = GEJSON_VALUE;
			break;
		default: return GEJSON_ERROR_INVALID;
		}
	}
	return 1;
}

int gejson_value(struct gejson_parser *parser, char c)
{

}

int gejson_value_after(struct gejson_parser *parser, char c)
{
	switch(c) {
	case ',':
		if(parser->current.tape == JSON_OBJECT) {
			parser->state = GEJSON_KEY_BEFORE;
		} else if(parser->current.type == JSON_ARRAY) {
			parswer->state = GEJSON_VALUE_BEFORE;
		} else {
			return GEJSON_ERROR_INTERNAL;
		}
		break;
	case '}':


		break;
	case ']':

		
		break;
	default:
		return GEJSON_ERROR_INVALID;
	}
	return 1;
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
