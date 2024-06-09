CREATE TABLE User (
  id INT AUTO_INCREMENT,
  PRIMARY KEY(id),
  username VARCHAR(50) NOT NULL UNIQUE,
  password VARCHAR(40) NOT NULL,
  date_created DATE DEFAULT CURRENT_DATE()
);

CREATE TABLE Bucket (
  id INT AUTO_INCREMENT,
  PRIMARY KEY(id),
  name VARCHAR(50) NOT NULL UNIQUE,
  region VARCHAR(60) NOT NULL
);

CREATE TABLE File (
  id INT AUTO_INCREMENT,
  PRIMARY KEY(id),
  name VARCHAR(40) NOT NULL,
  type CHAR(10) NOT NULL,
  date_created DATE DEFAULT CURRENT_DATE(),
  time_created TIME DEFAULT CURRENT_TIME(),
  user_id INT,
  FOREIGN KEY(user_id) REFERENCES User(id)
);

CREATE TABLE Chunk (
  id INT AUTO_INCREMENT,
  PRIMARY KEY(id),
  chunk_key VARCHAR(40) UNIQUE NOT NULL,
  primary_bucket_id INT,
  replicated_bucket_id INT,
  object_key VARCHAR(100) NOT NULL UNIQUE,
  file_id INT,
  FOREIGN KEY(file_id) REFERENCES File(id),
  FOREIGN KEY(primary_bucket_id) REFERENCES Bucket(id),
  FOREIGN KEY(replicated_bucket_id) REFERENCES Bucket(id)
);

CREATE TABLE TrashFile (
  id INT,
  PRIMARY KEY(id),
  name VARCHAR(40) NOT NULL UNIQUE,
  type CHAR(10) NOT NULL,
  date_created DATE DEFAULT CURRENT_DATE(),
  time_created TIME DEFAULT CURRENT_TIME(),
  user_id INT NOT NULL,
  FOREIGN KEY(user_id) REFERENCES User(id)
);

CREATE TABLE TrashChunk (
  id INT AUTO_INCREMENT,
  PRIMARY KEY(id),
  chunk_key VARCHAR(40) UNIQUE NOT NULL,
  replicated_bucket_id INT,
  object_key VARCHAR(100) NOT NULL UNIQUE,
  file_id INT,
  FOREIGN KEY(file_id) REFERENCES TrashFile(id),
  FOREIGN KEY(replicated_bucket_id) REFERENCES Bucket(id)
);

