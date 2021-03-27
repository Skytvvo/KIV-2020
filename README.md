# KIV-2020
# Compiler for self language (KIV)

## Compiler consist of 3 main parts
 
 * Analizator:
  > Lexical analizator <br>
  > Syntax analizator <br>
  > Semantic analizator <br>
 * Polish notation generator
 * Code generator
 
## Analiator:
### Lexical analizator
* Takes word and push him to token analiz. If this looks ok then token analiz return token of him
* Then anlizator takes token and will push him to special table
> If is variable, function that analizator push this toke n to ID Table <br>
> If is value (literal or another variable) of variable that analizator push him to Lexical Table with info about him variable and push info about this value for relevant variable
> If is token of syntax sign or another token which not a value or function or variable that analizator take him and push to Lexical Table without additional info

### Syntax analizator (based on Greibach tape logic)
* Takes Lexical Table and call Function with her as arguments.
Then Function start check for matches of prepared tapes of tokens and sequence from Lexical Table (tokens from her)
* If all is ok then analizator finishing work

### Semantic analicator:
* Checking logic of expressions of Tables, example string variable cannot use number literal

## Polish notation 
> Takes data from Lexical Table and generate polish exrpession

## Code generator 
> Takes polish expression and generate assembly code based on him
