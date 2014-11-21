var net = require('net')

var chatServer = net.createServer(),
    clientList = []

chatServer.on('connection', function(client) {
    client.name = client.remoteAddress + ':' + client.remotePort
    console.log(client.name + ' login')
    client.write('Hi! ' + client.name + '!\n');

    clientList.push(client);

    client.on('data', function(data) {
        console.log(client.name + ' >> ' + data)
        broadcast(data, client)
    })

    client.on('end', function() {
        console.log(client.name + ' quit')
        clientList.splice(clientList.indexOf(client), 1)
    })

    client.on('error', function(e) {
        console.log('ERROR: ' + e)
    })
})

chatServer.listen(9000)

function broadcast(msg, client) {
    var cleanup = []
    var target
    for (var i = 0; i < clientList.length; i++) {
        target = clientList[i]
        if (client !== target) {
            if (target.writable){
                target.write(client.name + ' >> ' + msg)
            }else{
                cleanup.push(target)
            }
        }
    }
    for (var i = 0; i < cleanup.length; i++) {
        clientList.splice(clientList.indexOf(cleanup[i]), 1)
        clientList[i].destory()
    }
}
