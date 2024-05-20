UPDATE Chunk
  SET primary_bucket_id = $primary_bucket_id, replicated_bucket_id = $replicated_bucket_id 
WHERE object_key = $object_key;
