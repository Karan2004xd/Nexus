const express = require('express');
const { Kafka } = require('kafkajs');
const cors = require('cors');

const app = express();
const port = 3000;

// Enable CORS
app.use(cors());
app.use(express.json());

// Initialize KafkaJS
const kafka = new Kafka({
  clientId: 'Nexus Client',
  brokers: ['localhost:9092'],
});

const producer = kafka.producer();
const consumer = kafka.consumer({ groupId: 'nexus-frontend-group' });

// Connect the producer and consumer
const run = async () => {
  await producer.connect();
  await consumer.connect();
  await consumer.subscribe({ topic: 'nexus-frontend-topic', fromBeginning: false });

  consumer.run({
    eachMessage: async ({ topic, partition, message }) => {
      console.log(`Received message: ${message.value.toString()}`);
    },
  });
};

run().catch(console.error);

// Endpoint to produce messages to Kafka
app.post('/dfs', async (req, res) => {
  const { requestId, payload } = req.body;

  try {
    await producer.send({
      topic: 'nexus-backend-topic',
      messages: [{ key: requestId, value: JSON.stringify(payload) }],
    });
    res.status(200).send(JSON.stringify(payload));
  } catch (error) {
    res.status(500).send('Failed to send request to DFS');
  }
});

app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});

