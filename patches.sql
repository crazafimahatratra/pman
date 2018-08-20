0|CREATE TABLE version(version INTEGER);
0|INSERT INTO version(version) VALUES(1);
0|CREATE TABLE IF NOT EXISTS project (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR (128) NOT NULL, color VARCHAR(6));
0|CREATE TABLE IF NOT EXISTS task (id INTEGER PRIMARY KEY AUTOINCREMENT, project_id INTEGER REFERENCES project (id) ON DELETE CASCADE ON UPDATE CASCADE, label VARCHAR (128) NOT NULL, date DATETIME NOT NULL, comment TEXT, status CHAR);
0|CREATE TABLE IF NOT EXISTS note (id INTEGER PRIMARY KEY AUTOINCREMENT, project_id INTEGER REFERENCES project (id) ON DELETE CASCADE ON UPDATE CASCADE, title VARCHAR(64), ctime DATETIME, note TEXT, x INTEGER, y INTEGER, width INTEGER, height INTEGER);
0|CREATE TABLE IF NOT EXISTS event (id INTEGER PRIMARY KEY AUTOINCREMENT, project_id INTEGER REFERENCES project (id) ON DELETE CASCADE ON UPDATE CASCADE, ctime DATETIME, title VARCHAR (64), detail TEXT);
0|ALTER TABLE task ADD canceled BOOLEAN;
0|UPDATE task SET canceled = 0;