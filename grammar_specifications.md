# Context-Free Grammar Specification
This grammar defines the following expresions:
a. Basic arithmetic expressions such as addition, subtraction, multiplication, and division. 
b. Conditions check for if-else statements.
c. Logic expressions using True or False

## Formal Definition
The grammar G is a 4-tuple (V, Σ, R, S):

### V (Variables/Non-terminals):
`V = {E, T, F, N, O, P, Q, R, U}`

### Σ (Terminals):
`Σ = {+, -, *, /, (, ), number, true, false, &&, ||, if, else, else if}`

### S (Start Symbol):

Write the Start Symbol here.

Example: `S = {E}`

### R (Production Rules):
```ebnf
S -> E 
E -> T
E -> E + T
E -> E - T

T -> F
T -> T * F
T -> T / F

F -> N
F -> (E)
N -> number

----------
O -> P
P -> P || Q
P -> Q

Q -> R
R -> R && U

U -> (O)
U -> true
U -> false
U -> E > E
U -> E < E
U -> E >= E
U -> E <= E
```


## LR(1) Automaton
Screenshot of the diagram can be found in LR(1).png


## LALR Verification

### Item Sets: 
States that can be merged:
- S10 & S11
- S13 & S17


### Parse Table and Conflict Check
Did not find any shift-reduce conflict


### LALR Automaton
Screenshot of the diagram can be found in LALR.png



