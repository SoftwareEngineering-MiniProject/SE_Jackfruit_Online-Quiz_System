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
