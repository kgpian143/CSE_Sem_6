import java.io.*;

public class ma1
{
    public static void main(String[] args)
    {
        String obj="abc";
        byte b[]=obj.getBytes();
        ByteArrayInputStream obj1=new ByteArrayInputStream(b);
        ByteArrayOutputStream obj2=new ByteArrayOutputStream();
        for(int i=0; i<1; ++i)
        {
            int c;
            while((c=obj1.read())!=-1)
            {
                if(i==0)
                {
                    System.out.print(Character.toUpperCase((char)c));
                    obj2.write(1);
                }
            }
            System.out.print(obj2);
        }
    }
}