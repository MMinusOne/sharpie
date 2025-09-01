# Sharper / Sharpie Project

## What is it

An interpreted programming language built from scratch with C++.

## Features

- Standard Library: `log`, `logln`, `newLine`, `@import`
- Variables + Static Type
- Functions + Dynamic Return Type + Static Argument Type
- Conditions: `string < string`, `string > string`, `string == string`
- If statements
- For loop statements
- Top level invocations that start with `@`
- Classes with their own mutable and block-scoped state
- `scope` keyword that let's you directly access the variables in the current scope (equivalent to this)

## Examples

### Logging

```
fn main () {
  stdlib.log("Log one");
  stdlib.logln("Log two new line");
  stdlib.newLine();
  stdlib.log("Log three manual new line");
}
```

**Output**: 
```
Log one
Log two new line
Log three manual new line
```

### Variables

```
fn main () {
    var string name = "Michael";
    stdlib.logln(name);
}
```

**Output**:
```

Michael
```

### Loops

```
fn logNormal(int index) {
    stdlib.logln("I am now" index "years old");
}

fn logHappy(int index) {
    stdlib.logln("I AM FINALLY" index "YEARS OLD WIWIWIWIW");
}

fn main() { 
    var int start = 20;
    var int end = 31;
    var int limit = 27;

    for (index;start;end)

        do_if (index < limit) 
            logNormal(index);
        if_end

        do_if (index == 25) 
            logHappy(index);
        if_end

    for_end
}
```

**Output**:
```
I am now 20 years old
I am now 21 years old
I am now 22 years old
I am now 23 years old
I am now 24 years old
I am now 25 years old
I AM FINALLY 25 YEARS OLD WIWIWIWIW
I am now 26 years old
```

### Importing

```
@import [loopTo] from "C:\Users\LENOVO\Desktop\sharpie\Sharpie\examples\4-loopTo.sharpie";

fn main() { 
    var string to = "100";
    loopTo(to);
}
```

```
fn loopTo(string to) { 
    for(index;1;to) 
        stdlib.logln("LOGGING TO" to "CURRENTLY AT" index);
    for_end
}
```

**Output**:
```
LOGGING TO 100 CURRENTLY AT 1
LOGGING TO 100 CURRENTLY AT 2
LOGGING TO 100 CURRENTLY AT 3
LOGGING TO 100 CURRENTLY AT 4
LOGGING TO 100 CURRENTLY AT 5
LOGGING TO 100 CURRENTLY AT 6
LOGGING TO 100 CURRENTLY AT 7

..........

LOGGING TO 100 CURRENTLY AT 98
LOGGING TO 100 CURRENTLY AT 99
```

### Function returns

```
fn makeSentence(string name int age) {
    return name "is" age "years old";
}

fn main() { 
    var string name = "Michael";

    for (index;1;17) 
        var string sentence = < makeSentence(name index) >;
        stdlib.logln(sentence);
    for_end
}
```

**Output**:

```
Michael is 1 years old
Michael is 2 years old
Michael is 3 years old
Michael is 4 years old
Michael is 5 years old
Michael is 6 years old
Michael is 7 years old

........
```

### Class

```
class User 
    var string balance = "1000";

    fn setBalance (string number) {
        var string mutate_class balance = number;
    }

    fn getBalance () {
        return scope.balance;
    }
class_end

fn main() {
  var User user = new User();
  var string balance = user.balance;
  stdlib.logln(balance);

  var string newBlance = "500";
  user.setBalance(newBlance);

  var string balanceThruFunction = < user.getBalance() >;
  
  stdlib.log(balanceThruFunction);
}
```

**Output**:

```

1000 500
```

## Thanks

Thanks for going through my README :)