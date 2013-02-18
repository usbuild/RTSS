BEGIN;
CREATE TABLE user(
    id VARCHAR(15), 
    password VARCHAR(40), 
    card VARCHAR(40),
    phone VARCHAR(15),
    type INTEGER DEFAULT 0,
    PRIMARY KEY(id)
);                                                         

CREATE TABLE station(
    id INTEGER PRIMARY KEY ASC,
    name VARCHAR(15)
);

CREATE TABLE ticket(
    id VARCHAR(10),
    start_id VARCHAR(15),
    end_id VARCHAR(15),
    stime VARCHAR(20),
    etime VARCHAR(20),
    price REAL,
    distance INTEGER,
    num INTEGER,
    PRIMARY KEY(id), 
    FOREIGN KEY(start_id) REFERENCES station(id),
    FOREIGN KEY(end_id) REFERENCES station(id)
);
CREATE TABLE user_ticket(
    id INTEGER PRIMARY KEY ASC,
    user_id VARCHAR(15),
    ticket_id VARCHAR(10),
    FOREIGN KEY(user_id) REFERENCES user(id),
    FOREIGN KEY(ticket_id) REFERENCES ticket(id)
);

-- insert some test data --
INSERT INTO user VALUES('usbuild', '12345', '091170087', '1311111111', 0);
INSERT INTO user VALUES('admin', '12345', '', '', 1);
INSERT INTO station VALUES(1, 'Beijing');
INSERT INTO station VALUES(2, 'Shanghai');
INSERT INTO ticket VALUES("K8955", 1, 2, "2013-02-01 19:00", "2013-02-02 18:00", 123.00, 199, 500);
COMMIT;
