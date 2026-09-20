
public class Data {

    private int dia;
    private int mes;
    private int ano;

    // Cria uma Data já com os valores informados.
    public Data(int dia, int mes, int ano) {
        this.dia = dia;
        this.mes = mes;
        this.ano = ano;
    }

    // converte texto em Data
    public static Data parseData(String s) {

        // Corta os hífens
        String[] p = s.split("-");

        // Integer.parseInt converte texto em número - IA
        return new Data(
                Integer.parseInt(p[2]), // dia
                Integer.parseInt(p[1]), // mês
                Integer.parseInt(p[0]) // ano
        );
    }

    public String format() {
        return String.format("%02d/%02d/%04d", dia, mes, ano);
    }

    // devolvem o valor de cada atributo
    public int getDia() {
        return dia;
    }

    public int getMes() {
        return mes;
    }

    public int getAno() {
        return ano;
    }
}