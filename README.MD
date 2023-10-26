# µJSON 

This is a very basic json parser without all the bells and whistles.
The intent is memory safe parsing, with safe failing.  All memory is
either statically allocated or on the stack.

## Principles

Rather than parse into tokens or a tree, this library inverts the
operation and uses a callback that the user function can use to
handle the parsed values.  All values are null terminated strings.

## Usage
Provide a callback
```
void foundValues(char * name, char * value){
  printf("%s = %s\r\n", name, value);
}
```

Parse the JSON.
```
uJson_Minify(testData4);/* Minify required */
int res = uJsonEmitter(json, foundValues);
```

```json
{
  "pb": {
    "p": 75,
    "i": 0.5,
    "d": 30,
    "iMin": -50,
    "iMax": 50
  },
  "pe": {
    "p": 0.9,
    "i": 0.001,
    "d": 2.12,
    "iMin": 0,
    "iMax": 0
  },
  "p1": {
    "p2": {
        "p3": "12345",
        "p4": [ 1, 2, 3, 4, 5 ]
    }
  },
  "bt": 24.5,
  "et": 4.8,
  "serial":"23A:001"
}
```
With the above examples prints:
```text
pb.p = 75
pb.i = 0.5
pb.d = 30
pb.iMin = -50
pb.iMax = 50
pe.p = 0.9
pe.i = 0.001
pe.d = 2.12
pe.iMin = 0
pe.iMax = 0
p1.p2.p3 = 12345
p1.p2.p4[0] = 1
p1.p2.p4[1] = 2
p1.p2.p4[2] = 3
p1.p2.p4[3] = 4
p1.p2.p4[4] = 5
bt = 24.5
et = 4.8
serial = 23A:001
```

