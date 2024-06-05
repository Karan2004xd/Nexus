const express = require('express');
const { Kafka } = require('kafkajs');
const cors = require('cors');

const app = express();
const port = 3000;

const responseMap = new Map();
const responseList = [];

// Enable CORS
app.use(cors());
app.use(express.json());

// Initialize KafkaJS
const kafka = new Kafka({
  clientId: 'Nexus Client',
  brokers: ['localhost:9092'],
  connectionTimeout: 6000
});

const consumerConfig = {
  groupId: 'nexus-frontend-group',
  sessionTimeout: 6000,
  autoCommitInterval: 100,
  autoOffsetReset: 'earliest',
  maxPollInterval: 60000
};

const producerConfig = {
  idempotent: true
};

const producer = kafka.producer(producerConfig);
const consumer = kafka.consumer(consumerConfig);

// Connect the producer and consumer
const run = async () => {
  await producer.connect();
  await consumer.connect();
  await consumer.subscribe({ topic: 'nexus-frontend-topic', fromBeginning: true });

  await consumer.run({
    eachMessage: async ({ topic, partition, message }) => {
      const response = message.value.toString();

      console.log(`Received message: ${response}`);
      console.log(responseList.length);
      let res;
      while (responseList.length > 0) {
        res = responseList.shift();
        res.status(200).send(response);
      }
      
      // if (responseMap.has(requestId)) {
      //   res.status(200).send(responseMap.get(requestId));
      // }
    },
  });
};

run().catch(console.error);

// Endpoint to produce messages to Kafka
app.post('/dfs', async (req, res) => {
  const { requestId, payload } = req.body;

  try {
    const requestData = JSON.stringify(payload);
    producer.send({
      topic: 'nexus-backend-topic',
      messages: [{ key: requestId, value: requestData }],
    });
    
    // responseMap.set(requestId, res);
    responseList.push(res);
    // await producer.flush();
    // res.status(200).send(response);
  } catch (error) {
    res.status(500).send('Failed to send request to DFS');
  }
});

app.listen(port, () => {
  console.log(`Server is running at http://localhost:${port}`);
});

