const amqp = require('amqplib/callback_api'); 
const args = process.argv.splice(2);
const generateUid = require('./generateUid');
const config = require('./config.json')

if(args.length == 0){
    console.log("Usage: roc_client.js num"); 
    process.exit(1);
}

const RPC_QUEUE = 'rpc_queue'

const amqpConnect = () => {
    return new Promise(resolve => {
        amqp.connect(config.url, (error0, connection) => {
            if(error0) throw error0; 
            resolve(connection)
        })
    })
}; 

const createChannel = (connection) => {
    return new Promise(resolve => {
        connection.createChannel((error1, channel) => {
            if(error1) throw error1; 
            resolve(channel)
        })
    })
}

const createQueue = (channel) => {
    return new Promise(resolve => {
        channel.assertQueue('', {
            exclusive: true
        }, (error2, q) => {
            if(error2) throw error2; 
    
            const correlationId = generateUid();
            resolve({ correlationId, replyQueue: q.queue })
        })
    })
   
}

const sendMessageToQueue = (messageInfo) => {
    
    const  { channel, message, correlationId, replyQueue } = messageInfo;

    const buffer = encodeMessage(message)
    channel.sendToQueue(RPC_QUEUE, buffer, {
        correlationId: correlationId, 
        replyTo: replyQueue
    })
}

const encodeMessage = (message) => {
    if (typeof message === "string") return Buffer.from(message)
    return Buffer.from(message.toString());
}


const onQueueReply = (queueInfo, cb) => {
    
    const { channel, replyQueue, correlationId } = queueInfo; 

    channel.consume(replyQueue, (msg) => {
        if(msg.properties.correlationId === correlationId){
            cb(msg.content.toString())
        }
    }, {
        noAck: true
    })
}

const disconnect = (connection) => {
    setTimeout(() => {
        connection.close()
        process.exit(0)
    }, 500)
}



const main = async () => {
    const connection = await amqpConnect(); 
    const channel = await createChannel(connection); 
    const queueInfo = await createQueue(channel); 

    const n = parseInt(args[0])
    const { replyQueue, correlationId } = queueInfo
    
    sendMessageToQueue({ channel, replyQueue, correlationId, message: n}); 

    onQueueReply({ channel, replyQueue, correlationId }, (message) => {
        console.log(`[.] server replied ${message}`); 
        disconnect(connection)
    })
}

main(); 