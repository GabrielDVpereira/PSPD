const amqp = require('amqplib/callback_api'); 
const fibonacci = require('./fibonacci');

const RPC_QUEUE = 'rpc_queue'

amqp.connect('amqp://localhost', (error0, connection) => {
    if(error0) throw error0; 

    connection.createChannel((error1, channel) => {
        if(error1) throw error1

        channel.assertQueue(RPC_QUEUE, {
            durable: false
        }); 

        channel.prefetch(1); // used to spread the load equally over multiple servers
        console.log('[x] Awaiting RPC requests'); 

        channel.consume(RPC_QUEUE, (msg) => {
            const n = parseInt(msg.content.toString()); 
            console.log(`[.] fib(${n})`)
            const fib = fibonacci(n);

            channel.sendToQueue(msg.properties.replyTo, 
                Buffer.from(fib.toString()), 
                { correlationId: msg.properties.correlationId }
            );

            channel.ack(msg)
        })
    })
})