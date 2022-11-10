const amqp = require('amqplib/callback_api');

amqp.connect('amqp://localhost', function(error0, connection) {
  if (error0) {
    throw error0;
  }
  connection.createChannel(function(error1, channel) {
    if (error1) {
      throw error1;
    }
    const queue = 'task_queue';

    // Creaing the queue in case it does not exist
    channel.assertQueue(queue, {
      durable: true
    });

    channel.prefetch(1);
 
    channel.consume(queue, function(msg) {
      const secs = msg.content.toString().split('.').length - 1
      console.log("[x] Received %s", msg.content.toString());
      setTimeout(() => {
        console.log("[x] Done.")
        channel.ack(msg)
      }, secs * 1000)
    }, {
        noAck: false
    })
  });
});