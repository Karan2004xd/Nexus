const { Kafka } = require('kafkajs')

// clientId is used to identify the application in Kafka logs and metrics
const kafka = new Kafka({
  clientId: 'my-app',
  brokers: ['localhost:9092']
});

// Producer example with clientId
const producer = kafka.producer();

const produceMessage = async () => {
  await producer.connect()
  await producer.send({
    topic: 'nexus-backend-topic',
    messages: [{
      value: {
        operation: 'list-data',
        username: 'user_1'
      }
    }],
  })
  await producer.disconnect()
}

// Consumer example with groupId
const consumer = kafka.consumer({ groupId: 'nexus-frontend-group' })

const consumeMessages = async () => {
  await consumer.connect()
  await consumer.subscribe({ topic: 'nexus-frontend-topic', fromBeginning: true })

  await consumer.run({
    eachMessage: async ({ topic, partition, message }) => {
      console.log({
        partition,
        offset: message.offset,
        value: message.value.toString(),
      })
    },
  })
}

// Run the examples
produceMessage().catch(console.error)
// consumeMessages().catch(console.error)

