import java.util.Scanner;

//  lê ids da entrada e para cada id, busca o veículo e imprime
public class Pilha {

    public static void main(String[] args) {

        Veiculo[] veiculos = LeitorCsv.ler("/tmp/veiculos.csv");

        PilhaEncadeada pilha = new PilhaEncadeada();

        Scanner scanner = new Scanner(System.in);

        while (scanner.hasNextInt()) {
            int id = scanner.nextInt();

            if (id == -1) {
                break;
            }

            Veiculo encontrado = buscaSequencial(veiculos, id);
            if (encontrado != null) {
                pilha.empilhar(encontrado);
            }
        }

        int m = scanner.nextInt();

        for (int i = 0; i < m; i++) {
            String comando = scanner.next();

            if (comando.equals("I")) {
                int idComando = scanner.nextInt();
                Veiculo encontrado = buscaSequencial(veiculos, idComando);
                if (encontrado != null) {
                    pilha.empilhar(encontrado);
                }

            } else if (comando.equals("R")) {
                Veiculo removido = pilha.desempilhar();
                System.out.println("(R)" + removido.getMarca() + " " + removido.getModelo());
            }
        }

        // mostra os elementos da pilha a partir do topo
        pilha.imprimir();

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
}

// nó da pilha - guarda um veículo e o ponteiro pro nó de baixo
class No {
    Veiculo dado;
    No proximo;

    No(Veiculo dado, No proximo) {
        this.dado = dado;
        this.proximo = proximo;
    }
}

// só mexe no topo
class PilhaEncadeada {

    private No topo;

    // cria um nó novo já apontando pro que era o topo, e faz esse
    // nó novo virar o topo
    public void empilhar(Veiculo v) {
        No novo = new No(v, topo);
        topo = novo;
    }

    // tira o nó do topo e devolve o veiculo dele
    public Veiculo desempilhar() {
        No removido = topo;
        topo = removido.proximo;
        return removido.dado;
    }

    public void imprimir() {
        No atual = topo;
        while (atual != null) {
            System.out.println(atual.dado.format());
            atual = atual.proximo;
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

// representa uma data, ta serpara porque é um tipo proprio

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

    // É static porque ainda não existe um objeto Data quando ele é chamado — é
    // justamente ele que vai criar o objeto
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