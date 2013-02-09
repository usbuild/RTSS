CREATE TABLE user(
    id VARCHAR(15), 
    password VARCHAR(40), 
    card VARCHAR(40),
    phone VARCHAR(15),
    PRIMARY KEY(id)
);                                                                  
CREATE TABLE ticket(
    id VARCHAR(10),
    start VARCHAR(15),
    end VARCHAR(15)
    p,rimary KEY(id)
    stime VARCHAR(20),
    etime VARCHAR(20),
    price REAL,
    distance INTEGER,
    num INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE user_ticket(
    id INTEGER PRIMARY KEY ASC,
    user_id VARCHAR(15),
    ticket_id VARCHAR(10),
    FOREIGN KEY(user_id) REFERENCES user(id),
    FOREIGN KEY(ticket_id) REFERENCES ticket(id)
);

