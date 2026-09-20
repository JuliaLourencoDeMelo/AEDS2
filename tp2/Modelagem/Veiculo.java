// Exe1- MODELAGEM EM JAVA
// alguns comentarios eu coloco pois estou estudando junto com os codigos

public class Veiculo {
//atributos

    private int id;
    private String marca;
    private String modelo;
    private int ano;
    private String categoria;
    private String[] combustivel;   // vetor, pois pode haver mais de um
    private int cilindros;
    private double cilindrada;
    private String transmissao;
    private String tracao;
    private double consumoCidade;
    private double consumoEstrada;
    private double co2;
    private boolean turbo;
    private Data dataRegistro;      

    
    public Veiculo(int id, String marca, String modelo, int ano, String categoria,
                   String[] combustivel, int cilindros, double cilindrada,
                   String transmissao, String tracao, double consumoCidade,
                   double consumoEstrada, double co2, boolean turbo,
                   Data dataRegistro) {
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

    // converte uma linha do CSV em Veiculo 

    public static Veiculo parseVeiculo(String s) {
        // Corta a linha nas vírgulas


        String[] c = s.split(",");

        // Converte cada campo para o tipo certo e cria o objeto.
        return new Veiculo(
            Integer.parseInt(c[0]),           
            c[1],                             
            c[2],                             
            Integer.parseInt(c[3]),           
            c[4],                             
            c[5].split(";"),                  // combustivel: "Gasoline;Electricity" -> vetor
            Integer.parseInt(c[6]),           
            Double.parseDouble(c[7]),         
            c[8],                             
            c[9],                             
            Double.parseDouble(c[10]),        
            Double.parseDouble(c[11]),        
            Double.parseDouble(c[12]),        
            Boolean.parseBoolean(c[13]),      
            Data.parseData(c[14])
        );           
    }

    // Retorna o veículo no formato certo

    public String format() {
        // Junta os combustíveis separados por vírgula, sem espaço.
        
        String combustiveis = "";
        for (int i = 0; i < combustivel.length; i++) {
            combustiveis += (i > 0 ? "," : "") + combustivel[i];
        }

        return String.format(
            "[%d ## %s ## %s ## %d ## %s ## [%s] ## %d ## %.1f ## %s ## %s ## %.2f ## %.2f ## %.1f ## %s ## %s]",
            id, marca, modelo, ano, categoria, combustiveis, cilindros,
            cilindrada, transmissao, tracao, consumoCidade, consumoEstrada,
            co2, turbo, dataRegistro.format());
    }

    public int getId() { return id; }
    public String getMarca() { return marca; }
    public String getModelo() { return modelo; }
    public int getAno() { return ano; }
    public String getCategoria() { return categoria; }

    // clone() devolve uma CÓPIA do vetor. Assim, o método
    // não consegue alterar o vetor do objeto (protege o encapsulamento).
    public String[] getCombustivel() { return combustivel.clone(); }

    public int getCilindros() { return cilindros; }
    public double getCilindrada() { return cilindrada; }
    public String getTransmissao() { return transmissao; }
    public String getTracao() { return tracao; }
    public double getConsumoCidade() { return consumoCidade; }
    public double getConsumoEstrada() { return consumoEstrada; }
    public double getCo2() { return co2; }

    // atributos boolean, o padrão do Java é usar "is" no lugar de "get"
    public boolean isTurbo() { return turbo; }

    public Data getDataRegistro() { return dataRegistro; }
}
