import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
public class split{
    public static void main(String[] args)
        throws FileNotFoundException
    {
        Scanner in;
        try{
            File infile = new File(args[0]);
            in = new Scanner(infile);
        }catch (FileNotFoundException ex){
            return;
        }
        in.nextInt();
        in.nextInt();
        in.nextLine();
        String s = in.nextLine();
        String[] ss = s.split(" ");
        System.out.println(s);
        for(int i=0; i< ss.length; i++)
            System.out.println(ss[i]);
        return;
    }
}
