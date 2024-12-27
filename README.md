# Sip (Treewalking Lox Interpreter)

The current state of Sip is an ongoing interpreter for the Lox language, built in C++.

# Instructions

1. Clone the repository,
```
$ git clone https://github.com/Lactorias/Sip.git

$ cd Sip
```
2. Run the build!
```
$ make
```
3. We can now use the language, write up a small .txt file with the details of your program following the syntax guide, and execute it!
```
$ ./compile /path/to/your/test_file.txt
```
4. Or, get a feel for the language by simply running with no provided file, in this mode you can test small syntax lines to a get a feel for the language.
```
$ ./compile

print 1 + 1;
```
# Syntax Guide

## Types

Lox is a dynamically typed language, this means type checking is left to runtime and you do not need to explicitly state your variable types!

For example, 
```
var a = 8; // this is a integer of value 8
var b = 7.8; // this is a double of value 7.8
var c = true; // this is a bool of value true
var d = "hello"; // this is a string of value "hello"
```

## Native Functions

Lox provides two native functions for you,

```
print 1 + 1; // this will output 2 to the terminal.

print clock(); // this will return a time, feel free to set this before an expensive operation to see how long it takes!
```
Print is especially useful, it can print any variable or expression you give it.

## If Statements

If you would like to control the flow of logic in your program, if statements are most likely the tool you should use,

```
if (5 < 10) {
    print "5 is less than 10!";
}
```
As you can see the condition within the if statement '5 < 10' evaluated to true so the above segment would indeed print "5 is less than 10!".

## Loops

Lox provides the lovable "while" and "for" loops,
```
for (var a = 0; a < 10; a = a + 1) {
    print a;
}

////////////////////////////////////

var a = 0;
while (a < 10) {
    print a;
    a = a + 1;
}
```
Both of these loops will print the value contained in the variable a, 10 times.
Experiment with them!

## Functions

Yep, lox has functions. You can use them to modularise your statements into a single callable statement!
Need to print something x times? Use functions!

Secret: Psst, recursion also works so feel free to integrate these with 'return' statements. :)

```
fun print_x_times(x, target) {
    for (var a = 0; a < x; a = a + 1) {
        print target;
    }
}

print_x_times(10, "how fun!");
```

## Classes

If you want to organise your data into neat blocks, and also keep the methods that can operate on them organised. Classes are what you want!

```
class lox_showcase {
  lox_me() {
    print "lox class here!";
  }
}

class derived_class < lox_showcase {}

lox_showcase().lox_me();
derived_class().lox_me();
```
Indeed, both of the statements at the bottom will print "lox class here!". The first seems to own it as the function is defined within it. But how does the second class get to also use this?

Lox has inheritance! You can extend a class with the '<' operator to fill your base class with all the methods from a base class!

Lox also provides you with the ability to create a constructor, use a method called "init" and this function will execute as soon as your class instance is created!

```
class lox_showcase {
  init() {
    print "lox class here!";
  }
}

class derived_class < lox_showcase {}

lox_showcase();
derived_class();
```

Notice how the above prints out even without needing to do anything? Just by creating two instances of the classes the init() function will automatically be called on creation!

# Error Handling

I have tried to provide errors in cases where possible, please listen to them and try to diagnose errors that occur within your code that you try to have interpreted!

# Plans

As I learn more about how languages are implemented, I hope to either revisit Sip and turn it into my own compiled static language, or do the same but with my own grammar and interpretation of the language.

# Acknowledgements

Robert Nystrom for making what seemed impossible a reality :) [Crafting Interpreters](https://craftinginterpreters.com/)

