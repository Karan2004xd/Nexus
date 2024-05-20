SELECT 
  chunk_key, primary_bucket_id, replicated_bucket_id, object_key
FROM Chunk 
WHERE file_id = $file_id;
