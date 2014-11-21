require 'rubygems'
require 'rev'
PORT=10240

class EchoServerConnection < Rev::TCPSocket
	def on_read(data)
		write 'You said: ' + data
	end
end

server = Rev::TCPServer.new('127.0.0.1',PORT,EchoServerConnection)
server.attach(Rev::Loop.default)

puts "Listening on localhost:#{PORT}"
Rev::Loop.default.run
