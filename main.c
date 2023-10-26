/*
 * main.c
 * micro JSON parser test
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uJson.h"

unsigned char testData4[340] = {
    0x7B, 0x0D, 0x0A, 0x20, 0x20, 0x22, 0x70, 0x62,
    0x22, 0x3A, 0x20, 0x7B, 0x0D, 0x0A, 0x20, 0x20,
    0x20, 0x20, 0x22, 0x70, 0x22, 0x3A, 0x20, 0x37,
    0x35, 0x2C, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x22, 0x69, 0x22, 0x3A, 0x20, 0x30, 0x2E, 0x35,
    0x2C, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x22,
    0x64, 0x22, 0x3A, 0x20, 0x33, 0x30, 0x2C, 0x0D,
    0x0A, 0x20, 0x20, 0x20, 0x20, 0x22, 0x69, 0x4D,
    0x69, 0x6E, 0x22, 0x3A, 0x20, 0x2D, 0x35, 0x30,
    0x2C, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x22,
    0x69, 0x4D, 0x61, 0x78, 0x22, 0x3A, 0x20, 0x35,
    0x30, 0x0D, 0x0A, 0x20, 0x20, 0x7D, 0x2C, 0x0D,
    0x0A, 0x20, 0x20, 0x22, 0x70, 0x65, 0x22, 0x3A,
    0x20, 0x7B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x22, 0x70, 0x22, 0x3A, 0x20, 0x30, 0x2E, 0x39,
    0x2C, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x22,
    0x69, 0x22, 0x3A, 0x20, 0x30, 0x2E, 0x30, 0x30,
    0x31, 0x2C, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x22, 0x64, 0x22, 0x3A, 0x20, 0x32, 0x2E, 0x31,
    0x32, 0x2C, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x22, 0x69, 0x4D, 0x69, 0x6E, 0x22, 0x3A, 0x20,
    0x30, 0x2C, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x22, 0x69, 0x4D, 0x61, 0x78, 0x22, 0x3A, 0x20,
    0x30, 0x0D, 0x0A, 0x20, 0x20, 0x7D, 0x2C, 0x0D,
    0x0A, 0x20, 0x20, 0x22, 0x70, 0x31, 0x22, 0x3A,
    0x20, 0x7B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x22, 0x70, 0x32, 0x22, 0x3A, 0x20, 0x7B, 0x0D,
    0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x22, 0x70, 0x33, 0x22, 0x3A, 0x20, 0x22,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x22, 0x2C, 0x0D,
    0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x22, 0x70, 0x34, 0x22, 0x3A, 0x20, 0x5B,
    0x20, 0x31, 0x2C, 0x20, 0x32, 0x2C, 0x20, 0x33,
    0x2C, 0x20, 0x34, 0x2C, 0x20, 0x35, 0x20, 0x5D,
    0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x7D, 0x0D,
    0x0A, 0x20, 0x20, 0x7D, 0x2C, 0x0D, 0x0A, 0x20,
    0x20, 0x22, 0x62, 0x74, 0x22, 0x3A, 0x20, 0x32,
    0x34, 0x2E, 0x35, 0x2C, 0x0D, 0x0A, 0x20, 0x20,
    0x22, 0x65, 0x74, 0x22, 0x3A, 0x20, 0x34, 0x2E,
    0x38, 0x2C, 0x0D, 0x0A, 0x20, 0x20, 0x22, 0x73,
    0x65, 0x72, 0x69, 0x61, 0x6C, 0x22, 0x3A, 0x22,
    0x32, 0x33, 0x41, 0x3A, 0x30, 0x30, 0x31, 0x22,
    0x0D, 0x0A, 0x7D, 0x00 
};

typedef struct {
  float bt;
  float et;  
} test_t;

void foundValues(char * name, char * value, void * ctx) {
  test_t * t = (test_t*) ctx;
  printf("%s = %s\r\n", name, value);
  if (memcmp(name, "bt", 2) == 0) {
    t->bt = strtof(value, NULL);
  } else if (memcmp(name, "et", 2) == 0) {
    t->et = strtof(value, NULL);
  }
}

int main(int argc, char** argv) {
  test_t test = {0};
  printf("Parsing\r\n%s\r\n", testData4);
  uJson_Minify(testData4);
  int res = uJsonEmitter(testData4, foundValues, &test);
  printf("test.bt = %f\r\ntest.et = %f\r\n", test.bt, test.et);
  return (EXIT_SUCCESS);
}

