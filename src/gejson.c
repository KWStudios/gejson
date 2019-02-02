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
		if ((parser->state == GEJSON_START ||
		     parser->state == GEJSON_OBJECT_START ||
		     parser->state == GEJSON_ARRAY_START) &&
		     IS_WHITESPACE(fragment[i]) ) {
			continue;
		}
		switch(parser->state) {
		case GEJSON_START:
			if(fragment[i] == '{' && fragment[i] != '[') {
				parser->state = GEJSON_OBJECT_START;
				continue;
			} else if(fragment[0] == '[') {
				parser->object.key = NULL;
				parser->object.type = JSON_ARRAY;
				parser->object.value.array =
					calloc(1, sizeof(struct gejson_array));
				if(parser->object.value.array == 0)
					return GEJSON_ERROR_INTERNAL;
				
				parser->state = GEJSON_ARRAY_START;
				continue;
			} else {
				return GEJSON_ERROR_INVALID;
			}
			break;
		case GEJSON_OBJECT_START:
			break;
		case GEJSON_ARRAY_START:
			break;
		default:
			return GEJSON_ERROR_INTERNAL;
		}
	}

	return incomplete;
}
