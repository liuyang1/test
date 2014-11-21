package service.server;
import org.apache.thrift.TProcessor;
import org.apache.thrift.protocol.TBinaryProtocol;
import org.apache.thrift.server.TServer;
import org.apache.thrift.server.TThreadPoolServer;
import org.apache.thrift.transport.TServerSocket;
import org.apache.thrift.transport.TTransportException;
import service.demo.Hello;
import service.demo.HelloServiceImpl;

public class HelloServiceServer {
    public static void main(String[] args)
    {
        try{
            int port = 7911;
            TServerSocket serverTransport = new TServerSocket(port);
            Factory proFactory = new TBinaryProtocol.Factory();
            TProcessor processor = new Hello.Processor(new HelloServiceImpl());
            TServer server = new TThreadPoolServer(processor, serverTransport, proFactory);
            System.out.println("Start server on port " + port + " ...");
            server.serve();
        }catch(TTransportException ex){
            ex.printStackTrace();
        }
    }
}
