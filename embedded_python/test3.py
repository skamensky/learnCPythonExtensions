import sqlite3

conn = sqlite3.connect(":memory:")
curs = conn.cursor()
curs.execute("CREATE TABLE hello(message varchar)")
curs.execute(
    "INSERT INTO hello (message) VALUES ('Hello from sqlite embedded in python embedded in c!')"
)
curs.execute("SELECT message from hello")
print(curs.fetchone()[0])
