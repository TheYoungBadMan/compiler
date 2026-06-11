# Grammar

## Lexical grammar

```ebnf
ident = ident-start ident-cont* ;
ident-start = letter | "_" ;
ident-cont = letter | digit | "_" ;

integer = (bin-integer | oct-integer | dec-integer | hex-integer) int-suffix? ;
bin-integer = "0b" bin-digit+ ;
oct-integer = "0o" oct-digit+ ;
dec-integer = digit+ ;
hex-integer = "0x" hex-digit+ ;
int-suffix = "i" | "u" ;

float = dec-integer "." dec-integer? exp? |
        dec-integer exp ;

exp = exp-start sign? dec-integer ;

letter = [a-zA-Z] ;
digit = [0-9] ;
bin-digit = [01] ;
oct-digit = [0-7] ;
hex-digit = [0-9a-fA-F] ;
exp-start = [eE] ;
sign = [+-] ;
```

### Tokens

```ebnf
token = ident | keyword | integer | float | symbol | EOF ;

keyword = "var" | "const" | "fn" | "record" | "alias" | "namespace"
        | "if" | "else" | "while" | "return" | "break" | "continue" | "pass"
        | "inf" | "nan" | "true" | "false" | "null"
        | "as" ;

symbol = three-char-symbol | two-char-symbol | one-char-symbol ;
three-char-symbol = "<<=" | ">>=" ;
two-char-symbol = "==" | "!=" | "<=" | ">=" | "&&" | "||" | "+=" | "-=" | "*=" | "/=" | "%="
                | "&=" | "|=" | "^=" | "<<" | ">>" | "->" | "::" ;
one-char-symbol = "+" | "-" | "*" | "/" | "%"
                | "=" | "!" | "<" | ">"
                | "&" | "|" | "^" | "~"
                | "(" | ")" | "[" | "]" | "{" | "}"
                | ";" | ":" | "," | "." ;
```

### Trivia

```ebnf
trivia = (whitespace | comment)* ;
whitespace = [ \t\r\n]+ ;
comment = line-comment | block-comment ;
line-comment = "//" [^\n]* ;
block-comment = "/*" (block-comment | [^*] | "*" [^/])* "*/" ;
```

## Syntax grammar

### Module

```ebnf
module = decl* EOF ;
```

### Declarations

```ebnf
decl = var-decl
     | fn-decl
     | record-decl
     | alias-decl
     | namespace-decl ;

var-decl = ("var" | "const") ident ":" type ("=" expr)? ";" ;

fn-decl = "fn" ident "(" param-list? ")" "->" type block ;
param-list = param ("," param)* ","? ;
param = "const"? ident ":" type ;

record-decl = "record" ident "{" field* "}" ;
field = ident ":" type ";" ;

alias-decl = "alias" ident "=" type ";" ;

namespace-decl = "namespace" ident "{" decl* "}" ;
```

### Statements

```ebnf
stmt = decl-stmt
     | expr-stmt
     | assign-stmt
     | if-stmt
     | while-stmt
     | return-stmt
     | break-stmt
     | continue-stmt
     | pass-stmt ;

decl-stmt = var-decl

expr-stmt = expr ";" ;

assign-stmt = expr assign-op expr ";" ;
assign-op = "=" | "+=" | "-=" | "*=" | "/=" | "%="
            | "&=" | "|=" | "^=" | "<<=" | ">>=" ;

if-stmt = "if" "(" expr ")" block ("else" block)? ;

while-stmt = "while" "(" expr ")" block ;

return-stmt = "return" expr? ";" ;

break-stmt = "break" ";" ;

continue-stmt = "continue" ";" ;

pass-stmt = "pass" ";" ;
```

### Expressions

```ebnf
expr = logical-or-expr ;

logical-or-expr = logical-and-expr ("||" logical-and-expr)* ;

logical-and-expr = equality-expr ("&&" equality-expr)* ;

equality-expr = relational-expr (equality-op relational-expr)* ;
equality-op = "==" | "!=" ;

relational-expr = bitwise-or-expr (relational-op bitwise-or-expr)* ;
relational-op = "<" | ">" | "<=" | ">=" ;

bitwise-or-expr = bitwise-xor-expr ("|" bitwise-xor-expr)* ;

bitwise-xor-expr = bitwise-and-expr ("^" bitwise-and-expr)* ;

bitwise-and-expr = shift-expr ("&" shift-expr)* ;

shift-expr = additive-expr (shift-op additive-expr)* ;
shift-op = "<<" | ">>" ;

additive-expr = multiplicative-expr (add-op multiplicative-expr)* ;
add-op = "+" | "-" ;

multiplicative-expr = prefix-expr (mul-op prefix-expr)* ;
mul-op = "*" | "/" | "%" ;

prefix-expr = prefix-op prefix-expr
            | postfix-expr ;
prefix-op = "+" | "-" | "~" | "!" | "*" | "&" ;

postfix-expr = primary-expr (postfix-op)* ;
postfix-op = "(" expr-list? ")" | "[" expr "]" | "." ident | "as" type ;

primary-expr = parenthesized-expr
             | array-expr
             | record-expr
             | ident-expr
             | literal-expr ;

parenthesized-expr = "(" expr ")" ;

array-expr = "[" expr-list "]" ;

record-expr = "{" field-init-list? "}" ;
field-init-list = field-init ("," field-init)* ","? ;
field-init = ident "=" expr ;

ident-expr = name ;

literal-expr = integer
              | float
              | "inf"
              | "nan"
              | "true"
              | "false"
              | "null" ;


expr-list = expr ("," expr)* ","? ;
```

### Types

```ebnf

type = postfix-type ;

postfix-type = primary-type (postfix-type-op)* ;
postfix-type-op = "*" | "[" expr "]" ;

primary-type = parenthesized-type
             | fn-type
             | named-type ;

parenthesized-type = "(" type ")" ;

fn-type = "fn" "(" type-list? ")" "->" type ;

named-type = name ;

type-list = type ("," type)* ","? ;
```

### Block

```ebnf
block = "{" stmt* "}" ;
```

### Name

```ebnf
name = (ident "::")* ident ;
```
