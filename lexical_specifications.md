# Scanner part 1 - Lexical Specifications

## Input language:
Input language: C

## Token Categories/Types:
### Keywords:
- int
- float
- char
- void
- if
- else
- if else
- true
- false

### Identifiers:
- For any name that comes typically after the keyword, either for variable declaration or function definition.

### Numbers:
- integers (positive & negative)
- fractions
- floating point

### Operators:
- \+
- \-
- \*
- /
- =
- ==
- !=
- \>
- <
- \>=
- <=

### Characters
- Single character wrapped by ''
- Single small letter
- Single capital letter
- Only 1 special char support: _ (for now)

### Strings
- Group of characters bundled by ""
- Small letters
- Capital letters
- Numbers represented as char
- Only 1 special char support: _ (for now)

## Formal Specification
### Keywords
#### Int: 
- int\s+[a-zA-Z_][a-zA-Z0-9_]*\s*(=\s*[+-]?[0-9]+)?;
- Example:
- int _name;
- int n45_=3;
- int  negative = -10000
#### Float:
- float\s+[a-zA-Z][a-zA-Z0-9_]*\s*(=\s*[+-]?[0-9]+(\.[0-9]+)?)?;
- Example:
- float number;
- float pi = 3.14;
- float num3_Decimal = 3;    # This should also be valid
#### Char:
- char\s+[a-zA-Z][a-zA-Z0-9_]*\s*(=\s*'.')?;
- Example:
- char letter;
- char space=' ';
- char l1='s';
- char num_char='9';
- char __capital_='S';
#### String:
- char\s+[a-zA-Z][a-zA-Z0-9_]*\[[0-9]*\]\s*(=\s*".*")?;
- Example:
- char testString[255];
- char emptyString[] = "";
- char a_string[128] = "test123";
- char str[10]="_";
#### If statement:
- if\s*\(\s*((".*")|.*)\s*==\s*((".*"|'.')|.*|true|false)\s*\)     # Ignoring "{}"
- Example:
- if(t==10)
- if (4==29 )
- if(str=="lemon")
- if ("char1" == "char2")
- if(character=="M" )
- if(identifier1 == identifier2)
#### Else statement:
- else\s    # Ignoring "{}"
#### Else-if statement:
- else\sif\s*\(\s*((".*")|.*)\s*==\s*((".*"\'.')|.*|true|false)\s*\)     # Ignoring "{}"
- Example:
- else if(bool==True)
- else if(chr =  's')