const net = require('net');
const crypto = require('crypto');

const SIGN_OFF = 'amongers/o';
const KEY = 'amongusballs';

let server = net.createServer(function(socket) {
    // socket handling logic
    console.log('client connected');

    socket.on('error', function (exc) {
        if(exc.message !== 'read ECONNRESET' && exc.message !== 'write after end') throw exc;
    });

    socket.on('data', function(data) {
        let message = data.toString();
        let cDate = new Date();

        console.log(`[${cDate.getHours()}:${cDate.getMinutes()}:${cDate.getSeconds()}] ${message}`);

        let splitMessage = message.split(SIGN_OFF);

        splitMessage.pop();

        console.log(splitMessage);

        splitMessage.forEach(newMessage => {
            switch(newMessage) {
                case 'socket/close': {
                    socket.end();
                    break;
                }
                case 'initialization/ping': {
                    socket.write(`initialization/pong${SIGN_OFF}`);
                    break;
                }
                case 'auth/init': {
                    let authKey = crypto.randomBytes(16).toString('hex');
                    socket.write(`auth/recieved/${authKey}${SIGN_OFF}`);
                    break;
                }
            }
        });
    });
});

server.listen(27015);
console.log('server listening on port 27015');