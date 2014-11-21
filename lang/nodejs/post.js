var http = require('http'),
    assert = require('assert')

var opts = {
    host : 'localhost',
    port : '9000',
    path : '/send',
    method : 'POST',
    headers : {'content-type' : 'application/x-www-form-urlencoded'}
}

var req = http.request(opts, function(res) {
    res.setEncoding('utf8')
    var data = ""
    res.on('data', function(d) {
        data += d
    })

    res.on('end', function() {
        console.log(data)
    })
})

req.write('tweet=test')
req.end()
