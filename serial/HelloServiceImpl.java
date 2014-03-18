package service.demo;
import org.apache.thrift.TException;

public class HelloServiceImpl implements Hello.Iface {
    @Override
    public boolean helloBoolean(boolean para) throws TException
    {
        return para;
    }
    @Override
    public int helloInt(int para) throws TException
    {
        try{
            Thread.sleep(para);
        }catch(InterruptedException ex){
            e.printStackTrace();
        }
        return para;
    }
    @Override
    public String helloNull() throws TException {
        return null;
    }
    @Override
    public String helloString() throws TException {
        return para;
    }
    @Override
    public void helloVoid() throws TException {
        System.out.println("Hello world");
    }
}
