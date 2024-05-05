SELECT 
  bucket_num, chunk_key, object_key 
FROM 
  File 
JOIN Chunk ON 
  Chunk.file_id = File.id 
WHERE File.name = '${name}';
