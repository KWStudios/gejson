#include "gejson.h"

#include <stdlib.h>

struct gejson_parser *gejson_create_parser(int flags)
{
	struct gejson_parser *parser;
	parser = malloc(sizeof(struct gejson_parser));

	return parser;
}
