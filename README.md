# Chess_Engine

## Automation tests

Automation tests are fully console based module of the application. To activate them use following command:

```bash
.\Automation_Tests.exe -f <test_file_name>
```

Argument `-f` is require for test to work. There are also one optional argument which can be specify. By inputting `-t` argument following with types of tests that you want to execute.

### List of supported test types:
| Name            | Argument value | Test file value | Arguments                                                                         | Description                                  |
|-----------------|----------------|-----------------|-----------------------------------------------------------------------------------|----------------------------------------------|
| Move piece      | PIECE_MOVE     | PieceMove       | <ul><li>move command,</li><li>which turn it is.</li></ul>                         | Perform piece movement defined in test case. |
| Move generation | MOVE_GENERATE  | MoveGeneration  | <ul><li>x coordinates of the piece,</li><li>y coordinates of the piece.</li></ul> | Generate legal moves for given piece.        |

> **__NOTE:__** While using `-f` argument, specify file name without its extension. While using `-t` argument use 'Argument value' specify in table above.

### Test file
Before running automation tests it is require to create proper test file. File have to be placed in `test_scenarios` location and needs to have `.test` extension. Test file also needs proper content. Sample test file is presented below:

```
<--- Scenario --->
|bR|bN|bB|bQ|bK|bB|bN|bR|
|bP|bP|bP|bP|bP|bP|bP|bP|
|x |x |x |x |x |x |x |x |
|x |x |x |x |x |x |x |x |
|x |x |x |x |x |x |x |x |
|x |x |x |x |x |x |x |x |
|wP|wP|wP|wP|wP|wP|wP|wP|
|wR|wN|wB|wQ|wK|wB|wN|wR|

# Tests #
    @PieceMove
    {Pa7a5, w} | true
    {0-0-0, w} | false
    (END)

    @MoveGeneration
    {0, 0} | 0
    {6, 0} | 2
    (END)
```
As it can be seen every test scenario consists of 2 sections. First one represent state of the board for which tests should be performed. It needs to start with sequence: `<--- Scenario --->`.

Second component is a list of test cases that needs to be perform. Everyone of test cases needs to have the same structure starting with `@` sign and `Test file value` presented in table above. This section needs to start with sequence: `# Tests #`. After that user can specify number of test cases.

> **__IMPORTANT:__** All specified test cases needs to be the same type as in its header.

While specifying test cases remember to insert arguments in `{ }` brackets and separate expected result with `|` sign. Table above presents all arguments that is require for specific type of test case.

If specific test case passed on console will be printed `Test -> P` in other case message will be `Test -> F`. After all test cases will be process complete report fro test run will be printed.