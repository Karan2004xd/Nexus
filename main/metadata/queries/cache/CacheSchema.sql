CREATE TABLE Cache (
  id INT AUTO_INCREMENT,
  PRIMARY KEY(id),
  file_id INT,
  FOREIGN KEY(file_id) REFERENCES File(id)
);
