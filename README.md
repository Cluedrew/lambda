# lambda
A simple lambda calculator for working with lambda calculus.

This project was created because I like lambda calculus and I wanted to do
some work with my own languages, but a 'true' programming language is far
beyond me and the time I have right now.



## Usage

Reads a single element from standard input, parses it and echoes the parsed
element to standard output. If the element is also an expression it will print
the result of evaluating that expression. There are three types of elements.

### variable

A variable is a single lower case letter: a, b, ... & z. A variable is bound
if it appears within a function with a parameter that matches the variable.

### function

A function is a '.' with a variable to its left (its parameter) and any
element to its right (its body). A function is closed if all variables in its
body are bound. A function is an expression if it is closed.

### application

An application is made from a pair of elements enclosed in brackets, '(' &
')'. An application is an expression if its left and right hand sides are
expressions.

### Note
Avoid unbound variables, this implementation doesn't really handle that it is
focused on closed functions.



## Future Develpment

This project is not currently under development. If I return to it there are a
few particular features I would like to add.

* Definitions so common functions can be defined. ex. ID=x.x
* A definitions window to store the above.
* Step-by-step display of operations.
* LambdaTree class, a special pointer for trees of lambda element.
  (It is like a shared pointer for reading, but a unique pointer for writing.)
* Context wrapper instead of a vector of pointers to variables.
* LambdaElement dirty flag system.
* Sub-folders in the source directory? Split up the stages of analysis.
