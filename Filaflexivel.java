import java.util.Scanner;

public class Filaflexivel {

    public static void main(String[] args) {
        Fila fila = new Fila();

        Scanner scanner = new Scanner(System.in);

        while (scanner.hasNext()) {
            String comando = scanner.next();

            if (comando.equals("E")) {
                int x = scanner.nextInt();
                fila.enfileirar(x);

            } else if (comando.equals("D")) {
                int valor = fila.desenfileirar();
                System.out.println(valor);

            } else if (comando.equals("M")) {
                fila.imprimir();

            } else if (comando.equals("P")) {
                int y = scanner.nextInt();
                System.out.println(fila.pesquisar(y) ? "S" : "N");
            }
        }

        scanner.close();
    }
}

// nó da fila guarda um valor e o ponteiro pro próximo
class No {
    int valor;
    No prox;

    No(int valor) {
        this.valor = valor;
    }
}

// nó fantasma, sem valor útil)
class Fila {

    private No cabeca;
    private No fim;

    // fim aponta pra cabeça, porque a fila está vazia (não tem nenhum nó real)
    public Fila() {
        cabeca = new No(0);
        fim = cabeca;
    }

    public void enfileirar(int x) {
        No novo = new No(x);

        // encaixa o novo nó depois do que era o fim, e atualiza o fim
        fim.prox = novo;
        fim = novo;
    }

    public int desenfileirar() {
        // fila vazia = célula cabeça não aponta pra nenhum nó real
        if (cabeca.prox == null) {
            return -1;
        }

        No removido = cabeca.prox;
        int valor = removido.valor;

        // a cabeça passa a apontar pro segundo nó (que vira o novo primeiro)
        cabeca.prox = removido.prox;

        if (fim == removido) {
            fim = cabeca;
        }

        return valor;
    }

    public void imprimir() {
        if (cabeca.prox == null) {
            System.out.println("V");
            return;
        }

        StringBuilder sb = new StringBuilder();
        No atual = cabeca.prox;
        boolean primeiro = true;

        while (atual != null) {
            if (!primeiro) {
                sb.append(" ");
            }
            sb.append(atual.valor);
            primeiro = false;
            atual = atual.prox;
        }

        System.out.println(sb.toString());
    }

    // pesquisa se Y está na fila
    public boolean pesquisar(int y) {
        No atual = cabeca.prox;

        while (atual != null) {
            if (atual.valor == y) {
                return true;
            }
            atual = atual.prox;
        }

        return false;
    }
}