# Generate Mathematica Associations in C

The header file
```
#include "mmaformatter.h"
```
provides functions for converting C data structures to strings of Mathematica objects, and producing
files which Mathematica can read through `Get` as an [association](http://reference.wolfram.com/language/ref/Association.html).

For example,
```C
int length = 5;
int degits = 2;  // digits after decimal in scientific notation
double data[5] = {1.013, 10.331, .59102, 5.0, 6.0};
	
FILE* file = openAssocWrite("assoc.txt");
writeStringToAssoc(file, "patient", "Tyson");
writeIntToAssoc(file, "trial", 112);
writeDoubleArrToAssoc(file, "conductivity", data, length, digits);
closeAssocWrite(file);
```
produces a file "assoc.txt" with contents
```
<|
"patient" -> "Tyson",
"trial" -> 112,
"conductivity" -> {1.01*10^+00, 1.03*10^+01, 5.91*10^-01, 5.00*10^+00, 6.00*10^+00}
|>
```
which can then be read into Mathematica via
```Mathematica
SetDirectory @ NotebookDirectory[];
assoc = Get["assoc.txt"]

ListLinePlot @ assoc["conductivity"]
```
which produces the very-terrific graph

![a terrible graph](https://qtechtheory.org/wp-content/uploads/2017/12/assoctestplot.png)
