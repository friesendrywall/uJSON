/*
 * uJson.c
 * micro JSON parser
 */

/*
MIT License

Copyright (c) 2023 Erik Friesen
Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

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

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "./uJson.h"

static void fillObjString(char * name,
  char levels[UJSON_MAX_LEVELS][UJSON_MAX_OBJ_LENGTH + 1], 
  int32_t maxLen, uint32_t depth,
  int32_t ar) {
  /* */
  int32_t i, index;
  index = snprintf(name, maxLen, "%s", levels[0]);
  for (i = 1; i < depth; i++) {
    if (maxLen - index <= 0) {
      return;
    }
    index += snprintf(&name[index], maxLen - index, ".%s", levels[i]);
  }
  if(maxLen - index > 0 && ar >= 0) {
    snprintf(&name[index], maxLen - index, "[%i]", ar);
  }
}

 /* cJSON_Minify Original source */
void uJson_Minify(char *json) {
 
  char *into = json;
  while (*json) {
    if (*json == ' ') json++;
    else if (*json == '\t') json++; /* Whitespace characters. */
    else if (*json == '\r') json++;
    else if (*json == '\n') json++;
    else if (*json == '/' && json[1] == '/') while (*json && *json != '\n') json++; /* double-slash comments, to end of line. */
    else if (*json == '/' && json[1] == '*') {
      while (*json && !(*json == '*' && json[1] == '/')) json++;
      json += 2;
    }/* multiline comments. */
    else if (*json == '\"') {
      *into++ = *json++;
      while (*json && *json != '\"') {
        if (*json == '\\') *into++ = *json++;
        *into++ = *json++;
      }
      *into++ = *json++;
    }/* string literals, which are \" sensitive. */
    else *into++ = *json++; /* All other characters. */
  }
  *into = 0; /* and null-terminate. */
}

/**
 * 
 * @param json null terminated JSON
 * @param cb callback
 * @param ctx optional callback param
 * @return UJSON_ERR
 */
int uJsonEmitter(char * json, uJsonCallback cb, void * ctx) {
  int i, j, ar;
  char levels[UJSON_MAX_LEVELS][UJSON_MAX_OBJ_LENGTH + 1] = {0};
  char value[UJSON_MAX_OBJ_LENGTH + 1] = {0};
  char name[UJSON_MAX_LEVELS * UJSON_MAX_OBJ_LENGTH + 1] = {0};
  uint32_t len = strlen(json);
  enum {
    ST_OBJ_NAME_ST,
    ST_OBJ_NAME_FILL,
    ST_OBJ_COLON,
    ST_OBJ_VALUE,
    ST_OBJ_ARRAY,
    ST_OBJ_NEXT
  };
  int st = ST_OBJ_NAME_ST;
  int braces = 0;
  for (i = 0; i < len; i++) {
    if(json[i] == '{') {
      braces++;
      st = ST_OBJ_NAME_ST;
      if (braces > UJSON_MAX_LEVELS) {
        return UJSON_ERR_MAX_LEVELS;
      }
    } else if (json[i] == '}') {
      if(st == ST_OBJ_VALUE){
        value[j] = 0;
        fillObjString(name, levels, sizeof(name), braces, -1);
        cb(name, value, ctx);
        st = ST_OBJ_NEXT;
      }
      braces--;
    } else {
      switch(st){
      case ST_OBJ_NAME_ST:
        if(json[i]!= '"'){
          return UJSON_ERR_STRUCTURE;
        }
        st = ST_OBJ_NAME_FILL;
        j = 0;
        break;
      case ST_OBJ_NAME_FILL:
        if (json[i] == '"') {
          st = ST_OBJ_COLON;
          levels[braces - 1][j] = 0;
        } else {
          levels[braces - 1][j++] = json[i];
          if (j == UJSON_MAX_OBJ_LENGTH) {
            return UJSON_MAX_OBJ_LENGTH;
          }
        }
        break;
      case ST_OBJ_COLON:
        if (json[i] != ':') {
          return UJSON_ERR_STRUCTURE;
        }
        st = ST_OBJ_VALUE;
        /* Remove starting quotes */
        if (i < len - 1 && json[i + 1] == '"') {
          i++;
        }
        if (i < len - 1 && json[i + 1] == '[') {
          i++;
          st = ST_OBJ_ARRAY;
          ar = 0;
        }
        j = 0;
        break;
      case ST_OBJ_VALUE:
        if (json[i] == '"') {
          value[j] = 0;
          fillObjString(name, levels, sizeof(name), braces, -1);
          cb(name, value, ctx);
          st = ST_OBJ_NEXT;
        } else if (json[i] == ',') {
          value[j] = 0;
          fillObjString(name, levels, sizeof(name), braces, -1);
          cb(name, value, ctx);
          st = ST_OBJ_NAME_ST;
        } else {
          value[j++] = json[i];
          if (j == UJSON_MAX_OBJ_LENGTH) {
            return UJSON_MAX_OBJ_LENGTH;
          }
        }
        break;
      case ST_OBJ_ARRAY:
        if (json[i] == ']') {
          value[j] = 0;
          fillObjString(name, levels, sizeof(name), braces, ar);
          cb(name, value, ctx);
          st = ST_OBJ_NEXT;
        } else if (json[i] == ',') {
          value[j] = 0;
          fillObjString(name, levels, sizeof(name), braces, ar);
          cb(name, value, ctx);
          j = 0;
          ar++;
        } else {
          value[j++] = json[i];
          if (j == UJSON_MAX_OBJ_LENGTH) {
            return UJSON_MAX_OBJ_LENGTH;
          }
        }
        break;
      case ST_OBJ_NEXT:
        if (json[i] == ',') {
          st = ST_OBJ_NAME_ST;
        }
        break;
      }
    }    
    if (braces == 0) {
      return UJSON_ERR_NONE;
    }
  }
}

