import java.io.BufferedReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

/**
 * Classe LeitorCsv: lê o arquivo CSV e cria os objetos Veiculo.
 */
public class LeitorCsv {

    // Método estátic
    // Recebe o caminho do arquivo e devolve um vetor com todos os veículos
    public static Veiculo[] ler(String caminhoArquivo) {

        // sem quantiadade de linha : usa uma lista e depois transforma em vetor
        List<Veiculo> lista = new ArrayList<>();

        try (BufferedReader br = Files.newBufferedReader(
                Paths.get(caminhoArquivo), StandardCharsets.UTF_8)) {

            // A primeira linha do CSV só tem os nomes das colunas ( le e depois descarta)
            br.readLine();

            // Lê linha por linha. Quando o arquivo acaba, readLine devolve null.
            String linha;
            while ((linha = br.readLine()) != null) {

                // Ignora linhas vazias.
                if (linha.equals("")) {
                    continue;
                }

                // Converte a linha em um Veiculo e coloca na lista.
                lista.add(Veiculo.parseVeiculo(linha));
            }
        } catch (IOException e) {
            System.err.println("Erro ao ler o arquivo: " + e.getMessage());
        }

        // Converte a lista em vetor de Veiculo e devolve.
        return lista.toArray(new Veiculo[0]);
    }
}