#ifndef __GEJSON_H_INCLUDED__
#define __GEJSON_H_INCLUDED__

struct gejson_parser {
	char* buffer;
};

struct gejson_parser *gejson_crete_parser(int flags);

#endif
