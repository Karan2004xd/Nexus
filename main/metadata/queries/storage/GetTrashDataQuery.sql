SELECT 
  chunk_key, replicated_bucket_id, object_key
FROM TrashChunk 
WHERE file_id = $file_id;
