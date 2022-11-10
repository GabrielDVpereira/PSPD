const amqp = require("amqplib/callback_api")

amqp.connect('amqp://localhost', function(error0, connection) {
  if (error0) {
    throw error0;
  }

  // Creating the broker channel
  connection.createChannel(function(error1, channel) {
    const queue = 'task_queue';
    const msg =  process.argv.slice(2).join(' ') || "Hello World"

    // Creating a queue to send the message
    channel.assertQueue(queue, { durable: true })

    channel.sendToQueue(queue, Buffer.from(msg), { persistent: true }); 
    console.log(`[x] sent ${msg}`)
  });

  // close connection and leave
  setTimeout(function() {
    connection.close();
    process.exit(0)
  }, 500);

});