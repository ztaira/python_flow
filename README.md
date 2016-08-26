Python Flow Visualizer
======================

Hack-A-Week 2: A python flow visualizer, inspired by git log --graph. Example
image down below. Built to solve the following problem: 

"Why does it feel like I'm spending too much time finding stuff? I need to
find a better way to organize my code. If I put things that call each other
together, it would be faster, but first I need to find out what calls
what..."

### Usage:
- Executable is named 'python\_flow' by default (see makefile)
- To run, execute it as follows:

`./python_flow python_file_to_scan.py`

### Features:
- Different line styles to track normal movement and cross-code function calls

### What it does:
- Traces all normal function calls in a single script from start to finish
- Outputs handy visual data to cout
- Allows you to optimize your code for maximum visual/navigation efficiency

### What it doesn't do:
- Account for calls nested inside classes or imported libraries
- Visually color the output for easier usability

### Reasons for not doing things:
- Wow, I'm rusty at C++. 

### Included files:
```
- python_flow.cpp............Main C++ file. Make with g++ -o output filename
- helloworld.py..............Example python file to test from 
- README.md..................This readme file
```

### Example Output:
Key: Solid lines is code execution, dotted lines are jumps

Code execution:
```
 *
 |
 *
```

Code jump:
```
 .v
  :
  >.
```

### Efficient File Example - smooth, easily traceable code flow:

![alt text][outputimage]
[outputimage]: https://github.com/ztaira14/python_flow/blob/master/example.png "Python Flow output on helloworld.py"


### Inefficient File Example - choppy, erratic code flow:
![alt text][outputimage2]
[outputimage2]: https://github.com/ztaira14/python_flow/blob/master/example_ugly.png "Python Flow output on helloworld_ugly.py"

