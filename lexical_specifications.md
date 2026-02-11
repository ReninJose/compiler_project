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

### Identifiers:
- For any name that comes typically after the keyword, either for variable declaration or function definition.

### Numbers:
- integers (positive & negative)
- fractions
- floating point

### Operators:
- +
- -
- *
- /
- =
- ==
- !=
- >
- <
- >=
- <=

## Formal Specification
### Keywords
- int\s+[+-]?[0-9]+
- float\s+[+-]?[0-9]+\.[0-9]*
- char\s+[a-zA-Z]+
- (int|float|void)\s+[a-zA-Z]+
- if\s+(true|false)
- if\s+([0-9a-zA-Z]\s*(==|=!)\s*[0-9a-zA-Z])
- if\s+([0-9]+\s*(>=|<=|>|<)\s*[0-9]+)
- else\s
- else\sif\s+([0-9a-zA-Z]\s*(==|!=)\s*[0-9a-zA-Z])
- else\sif\s+([0-9]+\s*(>=|<=|>|<)\s*[0-9]+)