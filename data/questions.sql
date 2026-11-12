-- Question bank for the Online Quiz System (SQLite).
-- Build the database with:  make db   (runs: sqlite3 data/quiz.db < data/questions.sql)

DROP TABLE IF EXISTS questions;
CREATE TABLE questions (
    id       INTEGER PRIMARY KEY,
    text     TEXT NOT NULL,
    option_a TEXT NOT NULL,
    option_b TEXT NOT NULL,
    option_c TEXT NOT NULL,
    option_d TEXT NOT NULL,
    correct  TEXT NOT NULL CHECK (correct IN ('A','B','C','D'))
);

INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which keyword defines a constant in C?', 'const', 'final', 'static', 'fixed', 'A');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which function reads a line of text safely?', 'gets', 'fgets', 'scanf', 'getch', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('What is the size of int on most 32-bit systems?', '1 byte', '2 bytes', '4 bytes', '8 bytes', 'C');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which header declares printf?', 'stdlib.h', 'string.h', 'math.h', 'stdio.h', 'D');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which operator gives the remainder of a division?', '%', '/', '*', '&', 'A');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which symbol is used to get the address of a variable?', '*', '&', '#', '@', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which loop always runs its body at least once?', 'for', 'while', 'do-while', 'foreach', 'C');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('What does the strlen function return for "Hi"?', '1', '2', '3', 'It depends', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which keyword exits a loop immediately?', 'exit', 'stop', 'break', 'continue', 'C');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('What is the index of the first element of a C array?', '0', '1', '-1', 'It depends', 'A');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which format specifier prints an int?', '%f', '%c', '%s', '%d', 'D');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which header declares strcpy and strlen?', 'stdio.h', 'string.h', 'stdlib.h', 'math.h', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('What does the continue statement do inside a loop?', 'Ends the loop', 'Skips to the next iteration', 'Exits the program', 'Restarts the program', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which function opens a file in C?', 'open_file', 'fopen', 'fread', 'fload', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which operator is used to access a member of a struct through a pointer?', '->', '.', '::', '&', 'A');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('What is the result of 7 / 2 when both are int in C?', '3.5', '4', '3', '2', 'C');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which function releases memory taken by malloc?', 'delete', 'remove', 'release', 'free', 'D');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which keyword creates a user-defined type that groups several variables?', 'int', 'struct', 'goto', 'void', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('What does a pointer store?', 'A character', 'A memory address', 'A file name', 'A function result', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which of these is a correct comment in C99?', '// note', '## note', '-- note', '<!-- note -->', 'A');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('What is the correct way to end a statement in C?', 'A colon', '. (a dot)', 'A semicolon', 'A comma', 'C');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which function reads a whole line from a file or the keyboard?', 'fgets', 'putchar', 'fclose', 'printf', 'A');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which value does main usually return on success?', '-1', '0', '1', '100', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('Which operator checks whether two values are equal?', '=', '==', '!=', '=>', 'B');
INSERT INTO questions (text, option_a, option_b, option_c, option_d, correct) VALUES
    ('What does the preprocessor line #include do?', 'Runs a loop', 'Adds the contents of a header file', 'Deletes a file', 'Starts the compiler', 'B');
