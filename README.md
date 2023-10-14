# DOSATO

## Description

Dosato is a general purpose, interpereted programming language. It is designed with a DO SET MAKE model,
where the user can create variables, functions, and classes. It is designed to be easy to use,
and easy to understand. It is also designed to be easy to implement, and easy to extend.

## Usage

Dosato is a programming language, so it is used to write programs. It interprets TO files, filled with code, and executes it.<br>
Dosato is a DO SET MAKE oriented, these are the three main keywords of the language. Any and all lines of code must start with one of these three keywords.<br>
DO is used to execute code, SET is used to set variables, and MAKE is used to create functions and variables.<br>

## Syntax

### DO

DO is used to execute a function. It is followed by a function identifier. Following the identifier is a list of arguments, seperated by commas.<br>
Theres more! You can then also add an extension to the call, things like WHEN and ELSE. These are used to add conditions to the call.<br>
WHEN is used to execute the function only if the condition is true, and ELSE is used to execute the function only if the condition is false.<br>
INTO is used to store the return value of the function into a variable. It is followed by a variable identifier.<br>
Theres many more extensions to DO, such as WHILE, CATCH, and THEN. These are used to add conditions to the code block and can be stacked!<br>
<br>DO is also used to execute code blocks. A code block is a list of lines of code, surrounded by curly braces. The code block is executed line by line.<br>

### SET

SET is used to set a variable. It is followed by a variable identifier, and then an equals sign. After the equals sign is the value to set the variable to.<br>

### MAKE

MAKE is used to create a function or a variable. It is followed by a function or variable identifier. After the identifier is a list of arguments, seperated by commas.<br>
If the identifier is followed by an equals sign, then it is a variable. If it is followed by a colon, then it is a function. After the colon is the code block of the function.<br>

## Examples

### Comments

```
// This is a comment
```
Comments are started with two forward slashes. They are used to add notes to code, and are ignored by the interpreter.

### Hello World

```
DO SAY ("Hello World!");
```
SAY is a function that prints the argument to the console. all code lines must end with a semicolon.

### Variables

```
MAKE INT x = 5;
DO SAY (x * 10);
```
Dosato is a strongly typed language, so variables must be declared with a type. The type is followed by the identifier, then an equals sign, then the value to set the variable to.

### Functions

```
MAKE FUNC sum (DOUBLE a, DOUBLE b) {
    DO RETURN (a + b);
};

DO sum (5, 10) THEN SAY (_);
```
Functions are declared with MAKE FUNC. The identifier is followed by a list of arguments, seperated by commas. After the arguments is a code block, surrounded by curly braces.<br>
Functions return a value with RETURN. RETURN is followed by the value to return. The value can be any type.<br>
THEN is used to execute code after the function is called. It is followed by a code block, surrounded by curly braces. The return value of the function is stored in the variable _.

### SET

```
MAKE INT x = 5;
SET x = 10;
SET x *= 2;
DO SAY (x); // 20
```
SET is used to set variables. It is followed by a variable identifier, then an equals sign, then the value to set the variable to.<br>

## Types

### INT

```
MAKE INT x = 5;
```
INT is a type that stores integers. It is declared with MAKE INT, followed by the identifier, then an equals sign, then the value to set the variable to.<br>

### DOUBLE

```
MAKE DOUBLE x = 5.5;
```
DOUBLE is a type that stores floating point numbers. It is declared with MAKE DOUBLE, followed by the identifier, then an equals sign, then the value to set the variable to.<br>

### STRING

```
MAKE STRING x = "Hello World!";
```
STRING is a type that stores strings. It is declared with MAKE STRING, followed by the identifier, then an equals sign, then the value to set the variable to.<br>

### BOOL

```
MAKE BOOL x = TRUE;
```
BOOL is a type that stores booleans. It is declared with MAKE BOOL, followed by the identifier, then an equals sign, then the value to set the variable to.<br>

### ARRAY

```
MAKE ARRAY INT x = [1, 2, 3];
```
ARRAY is a type that stores arrays. It is declared with MAKE ARRAY, followed by a type identifier, then the identifier, then an equals sign, then the value to set the variable to.<br>

## Extensions

### WHEN

```
DO SAY ("Hello World!") WHEN (condition);
```
WHEN is used to execute code only if the condition is true. It is followed by a condition, surrounded by parentheses.<br>

### ELSE

```
DO SAY ("Hello World!") WHEN (condition) ELSE SAY ("Goodbye World!");
```
ELSE is used to execute code when the WHEN condition is false. It is followed by a code block, surrounded by curly braces just like the DO call.<br>

### INTO

```
DO sum (1,2) INTO x;
```
INTO is used to store the return value of a function into a variable. It is followed by a variable identifier.<br>

### WHILE

```
DO SAY ("Hello World!") WHILE (condition);
```
WHILE is used to execute code while the condition is true. It is followed by a condition, surrounded by parentheses.<br>
This is used to create loops.<br>

### CATCH

```
DO SAY ("Hello World!") CATCH (condition);
```
CATCH is used to execute code when the code returns an error. It is followed by a code block, surrounded by curly braces just like the DO call.<br>

### THEN

```
DO sum (1,2) THEN SAY (_);
```
THEN is used to execute code after the function is called. It is followed by a code block, surrounded by curly braces just like the DO call.<br>

## Built In Functions

### SAY

```
DO SAY ("Hello World!");
```
SAY is a function that prints the argument to the console. It is followed by an argument, surrounded by parentheses.<br>

### RETURN

```
DO RETURN (5);
```
RETURN is a function that returns a value from a function. It is followed by an argument, surrounded by parentheses.<br>

### LISTEN

```
DO LISTEN (x);
```
LISTEN is a function that gets input from the user. It is followed by a variable identifier, surrounded by parentheses.<br>