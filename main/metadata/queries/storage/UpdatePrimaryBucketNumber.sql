UPDATE Chunk
  SET primary_bucket_id = NULL 
WHERE file_id = $file_id;
