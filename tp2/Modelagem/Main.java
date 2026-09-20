import java.util.Locale;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {

        // Força o uso de PONTO nos números decimais
        Locale.setDefault(Locale.US);

        // Lê o CSV uma única vez e guarda todos os veículos em um vetor.
        Veiculo[] veiculos = LeitorCsv.ler("veiculos.csv");

        Scanner sc = new Scanner(System.in);

        // Lê o primeiro id.
        int id = sc.nextInt();

        // Repete até encontrar o -1 (fim da entrada)
        while (id != -1) {

            // percorre o vetor do início ao fim,
            // comparando o id de cada veículo com o id procurado
            for (int i = 0; i < veiculos.length; i++) {
                if (veiculos[i].getId() == id) {
                    // imprime o veículo
                    System.out.println(veiculos[i].format());

                    // Faz o "i" passar do limite para encerrar o for
                    i = veiculos.length;
                }
            }

            id = sc.nextInt();
        }

        sc.close();
    }
}
