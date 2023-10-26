/*
 * uJson.h
 * micro JSON parser
 */

/*
MIT License

Copyright (c) 2023 Erik Friesen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#ifndef UJSON_H
#define UJSON_H

typedef void (*uJsonCallback) (char * emitName, char * emitValue, void * ctx);
void uJson_Minify(char *json);
int uJsonEmitter(char * json, uJsonCallback cb, void * ctx);

#define UJSON_MAX_OBJ_LENGTH 8
#define UJSON_MAX_LEVELS 3
typedef enum {
    UJSON_ERR_NONE,
    UJSON_ERR_STRUCTURE,
    UJSON_ERR_NOT_MINIFIED,
    UJSON_ERR_MAX_LEVELS,
    UJSON_ERR_LENGTH
} UJSON_ERR;

#endif /* UJSON_H */

