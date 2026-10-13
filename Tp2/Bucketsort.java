import java.util.Scanner;

//  lê ids da entrada e para cada id, busca o veículo e imprime
public class Bucketsort {

    public static void main(String[] args) {

        Veiculo[] veiculos = LeitorCsv.ler("/tmp/veiculos.csv");

        // array separado só com os veículos que forem encontrados a partir dos ids
        Veiculo[] selecionados = new Veiculo[veiculos.length];
        int totalSelecionados = 0;

        Scanner scanner = new Scanner(System.in);
        while (scanner.hasNextInt()) {
            int id = scanner.nextInt();

            if (id == -1) {
                break;
            }

            Veiculo encontrado = buscaSequencial(veiculos, id);

            if (encontrado != null) {
                selecionados[totalSelecionados] = encontrado;
                totalSelecionados++;
            }
        }

        // ordena o array de selecionados por cilindrada, usando Bucketsort
        Veiculo[] ordenados = bucketSort(selecionados, totalSelecionados);

        for (int i = 0; i < totalSelecionados; i++) {
            System.out.println(ordenados[i].format());
        }

        scanner.close();
    }

    // busca sequencial: percorre o array do início ao fim
    private static Veiculo buscaSequencial(Veiculo[] veiculos, int id) {
        for (Veiculo v : veiculos) {
            if (v.getId() == id) {
                return v;
            }
        }
        return null;
    }

    // Bucket Sort, usando "cilindrada" como chave

    private static Veiculo[] bucketSort(Veiculo[] veiculos, int n) {
        int numBaldes = 10;

        // cria os 10 baldes, cada um como uma lista dinâmica
        java.util.List<java.util.List<Veiculo>> baldes = new java.util.ArrayList<>();
        for (int i = 0; i < numBaldes; i++) {
            baldes.add(new java.util.ArrayList<>());
        }

        // joga cada veículo no balde certo, normalizando a cilindrada por 8.1 (IA)
        for (int i = 0; i < n; i++) {
            double normalizado = veiculos[i].getCilindrada() / 8.1;
            int indiceBalde = (int) (normalizado * numBaldes);

            // proteção pro caso do índice passar do último balde
            if (indiceBalde >= numBaldes) {
                indiceBalde = numBaldes - 1;
            }
            if (indiceBalde < 0) {
                indiceBalde = 0;
            }

            baldes.get(indiceBalde).add(veiculos[i]);
        }

        // ordena cada balde individualmente, usando Inserção
        for (java.util.List<Veiculo> balde : baldes) {
            insercaoNoBalde(balde);
        }

        // concatena todos os baldes de volta num único array
        Veiculo[] resultado = new Veiculo[n];
        int posicao = 0;
        for (java.util.List<Veiculo> balde : baldes) {
            for (Veiculo v : balde) {
                resultado[posicao] = v;
                posicao++;
            }
        }

        return resultado;
    }

    // Ordenação por Inserção

    private static void insercaoNoBalde(java.util.List<Veiculo> balde) {
        for (int i = 1; i < balde.size(); i++) {
            Veiculo atual = balde.get(i);
            int j = i - 1;

            while (j >= 0 && balde.get(j).getCilindrada() > atual.getCilindrada()) {
                balde.set(j + 1, balde.get(j));
                j--;
            }

            balde.set(j + 1, atual);
        }
    }
}

// ler o arquivo CSV e transforma cada linha em um objeto Veiculo
class LeitorCsv {

    public static Veiculo[] ler(String caminhoArquivo) {
        // arrayList porque eu ainda não sei quantas linhas o arquivo tem
        java.util.List<Veiculo> lista = new java.util.ArrayList<>();

        // try-with-resources: garante que o arquivo seja fechado - IA
        try (java.io.BufferedReader br = new java.io.BufferedReader(new java.io.FileReader(caminhoArquivo))) {

            String linha = br.readLine(); // cabeçalho, ignora
            linha = br.readLine();

            while (linha != null) {
                if (!linha.isBlank()) {
                    lista.add(Veiculo.parseVeiculo(linha));
                }
                linha = br.readLine();
            }

        } catch (java.io.IOException e) {

            System.err.println("Erro ao ler o arquivo: " + e.getMessage());
        }

        // converte a lista dinâmica para um array de Veiculo
        return lista.toArray(new Veiculo[0]);
    }
}

class Veiculo {

    // Todos os atributos são privados
    private int id;
    private String marca;
    private String modelo;
    private int ano;
    private String categoria;

    // Coloquei o array porque combustivel pode ter mais de um valor
    private String[] combustivel;

    private int cilindros;
    private double cilindrada;
    private String transmissao;
    private String tracao;
    private double consumoCidade;
    private double consumoEstrada;
    private double co2;
    private boolean turbo;
    private Data dataRegistro;

    // Construtor: recebe todos os valores já prontos e monta o objeto.
    public Veiculo(int id, String marca, String modelo, int ano, String categoria,
            String[] combustivel, int cilindros, double cilindrada,
            String transmissao, String tracao, double consumoCidade,
            double consumoEstrada, double co2, boolean turbo, Data dataRegistro) {
        this.id = id;
        this.marca = marca;
        this.modelo = modelo;
        this.ano = ano;
        this.categoria = categoria;
        this.combustivel = combustivel;
        this.cilindros = cilindros;
        this.cilindrada = cilindrada;
        this.transmissao = transmissao;
        this.tracao = tracao;
        this.consumoCidade = consumoCidade;
        this.consumoEstrada = consumoEstrada;
        this.co2 = co2;
        this.turbo = turbo;
        this.dataRegistro = dataRegistro;
    }

    // get: cada atributo, exigidos pelo enunciado.

    public int getId() {
        return id;
    }

    public String getMarca() {
        return marca;
    }

    public String getModelo() {
        return modelo;
    }

    public int getAno() {
        return ano;
    }

    public String getCategoria() {
        return categoria;
    }

    public String[] getCombustivel() {
        return combustivel;
    }

    public int getCilindros() {
        return cilindros;
    }

    public double getCilindrada() {
        return cilindrada;
    }

    public String getTransmissao() {
        return transmissao;
    }

    public String getTracao() {
        return tracao;
    }

    public double getConsumoCidade() {
        return consumoCidade;
    }

    public double getConsumoEstrada() {
        return consumoEstrada;
    }

    public double getCo2() {
        return co2;
    }

    // atributos boolean, usa is
    public boolean isTurbo() {
        return turbo;
    }

    public Data getDataRegistro() {
        return dataRegistro;
    }

    // método static: recebe uma linha crua e devolve um objeto Veiculo já pronto

    public static Veiculo parseVeiculo(String s) {
        // split quebra a linha inteira
        String[] campos = s.split(",");

        int id = Integer.parseInt(campos[0]);
        String marca = campos[1];
        String modelo = campos[2];
        int ano = Integer.parseInt(campos[3]);
        String categoria = campos[4];

        // o campo de combustível pode ter vários valores separados por ; entao faz um
        // segundo split
        String[] combustivel = campos[5].split(";");

        int cilindros = Integer.parseInt(campos[6]);
        double cilindrada = Double.parseDouble(campos[7]);
        String transmissao = campos[8];
        String tracao = campos[9];
        double consumoCidade = Double.parseDouble(campos[10]);
        double consumoEstrada = Double.parseDouble(campos[11]);
        double co2 = Double.parseDouble(campos[12]);

        boolean turbo = Boolean.parseBoolean(campos[13]);

        Data dataRegistro = Data.parseData(campos[14]);
        return new Veiculo(id, marca, modelo, ano, categoria, combustivel, cilindros,
                cilindrada, transmissao, tracao, consumoCidade, consumoEstrada, co2,
                turbo, dataRegistro);
    }

    public String format() {
        // junta o array de combustíveis de volta em uma única String
        String combustivelStr = "[" + String.join(",", combustivel) + "]";

        return String.format(
                "[%d ## %s ## %s ## %d ## %s ## %s ## %d ## %.1f ## %s ## %s ## %.2f ## %.2f ## %.1f ## %b ## %s]",
                id, marca, modelo, ano, categoria, combustivelStr, cilindros, cilindrada,
                transmissao, tracao, consumoCidade, consumoEstrada, co2, turbo,
                dataRegistro.format());
    }
}

// representa uma data (ano, mês, dia), ta serpara porque é um tipo proprio

class Data {

    private int ano;
    private int mes;
    private int dia;

    public Data(int ano, int mes, int dia) {
        this.ano = ano;
        this.mes = mes;
        this.dia = dia;
    }

    // ler os valores (get: nao muda )

    public int getAno() {
        return ano;
    }

    public int getMes() {
        return mes;
    }

    public int getDia() {
        return dia;
    }

    // É static porque ainda não existe um objeto Data quando ele é chamado - ele
    // que vai criar o objeto
    public static Data parseData(String s) {
        // split = quebra linha
        String[] partes = s.split("-");

        // Integer.parseInt converte cada pedaço de String para int
        int ano = Integer.parseInt(partes[0]);
        int mes = Integer.parseInt(partes[1]);
        int dia = Integer.parseInt(partes[2]);

        return new Data(ano, mes, dia);
    }

    // Converte a data para o formato de saída
    public String format() {
        return String.format("%02d/%02d/%04d", dia, mes, ano);
    }
}