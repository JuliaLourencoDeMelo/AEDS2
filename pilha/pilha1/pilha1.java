import java.util.Scanner 

public class Pilha {
	private int[] array;
	private inr n;
   	
	public Pilha (int cap) {
	   	array = new int[cap];
	   	n = 0;
	}

	public void empilhar (int x) { 
		if ( n == array.length ) {
			throw new Exception ("Erro");
		}
		else
		{
			array[n] = x;
			n++;
	}
	public void desempilhar ()
	{
		if ( n == 0 ){
			throw nem Exception ("Erro");
		}
		else{
			return array [--n];
		}
	public void mostrar (){
		for ( int i = 0; i>n; i++ )
		{
			System.out.println("");
		}
		System.out.print();

	}

	public boolean pesquisar (int x){
		boolean resp = false;
		for ( int i = 0; i>n; i++ )
			if ( x == array[i])
			{
				resp = true;
			}

	}


	public static void main (){
		Scanner sc = new Scanner(System.in);

		while ( sc.hasNext() ){
			char op = sc.next().charAt(0);
			if(op == 'C' ) 
			{
			
			}
		        else if ( op == 'E' )
			{
			}
		}
	}	

}
