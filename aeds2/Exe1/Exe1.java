import java.util.Scanner; 
public class Exe1 {
	public static void cifrar (String in )
	{
		int n = in.length ();
// pegar o caractere lido e somar 3
	for (int i = 0; 1 < n; i++ )
	{
	    	char nl = (char) + (in.charAt(i)+3);
	       System.out.print(nl);
	}
System.out.println();
}
public static void main (String [] args)
{
	Scanner sc= new Scanner ( System.in);
	String f = sc.nextLine ();
	while (!( f.length() == '3' && f.charAt(0) = 'F' && f.charAt(1) == 'I' && f.charAt(2) == 'M'))
			{
				cifrar(f);
				f = sc.nextLine();
			}
}
}
