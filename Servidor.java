import java.net.*;
import java.io.*;
import java.util.*;
import java.nio.file.*;
import org.json.*; 

class Conexao extends Thread {

    Socket socketCliente;
    static List<Pessoa> pessoas = Collections.synchronizedList(new ArrayList<>());
    private static final String ARQUIVO = "pessoas.json";

    Conexao(Socket aSocketCliente) {
        this.socketCliente = aSocketCliente;
    }

    // Método para salvar a lista de pessoas no arquivo
    private static synchronized void salvarNoArquivo() {
        JSONArray jArray = new JSONArray();
        synchronized (pessoas) {
            for (Pessoa p : pessoas) {
                JSONObject obj = new JSONObject();
                obj.put("id", p.id);
                obj.put("cpf", p.cpf);
                obj.put("nome", p.nome);
                obj.put("idade", p.idade);
                obj.put("profissao", p.profissao);
                jArray.put(obj);
            }
        }
        try {
            Files.write(Paths.get(ARQUIVO), jArray.toString(2).getBytes());
        } catch (IOException e) {
            System.out.println("Erro ao salvar arquivo: " + e.getMessage());
        }
    }

    // Método para carregar pessoas do arquivo
    public static void carregarDoArquivo() {
        Path path = Paths.get(ARQUIVO);
        if (Files.exists(path)) {
            try {
                String conteudo = new String(Files.readAllBytes(path));
                JSONArray jArray = new JSONArray(conteudo);
                for (int i = 0; i < jArray.length(); i++) {
                    JSONObject jObj = jArray.getJSONObject(i);
                    Pessoa p = new Pessoa();
                    p.id = jObj.getInt("id");
                    p.cpf = jObj.getString("cpf");
                    p.nome = jObj.getString("nome");
                    p.idade = jObj.getInt("idade");
                    p.profissao = jObj.getString("profissao");
                    pessoas.add(p);
                }
            } catch (IOException | JSONException e) {
                System.out.println("Erro ao carregar arquivo: " + e.getMessage());
            }
        }
    }

    @Override
    public void run() {
        try (PrintWriter saida = new PrintWriter(socketCliente.getOutputStream(), true);
             BufferedReader entrada = new BufferedReader(new InputStreamReader(socketCliente.getInputStream()))) {

            InetAddress endCliente = socketCliente.getInetAddress();
            String mensagem;

            while ((mensagem = entrada.readLine()) != null) {
                mensagem = mensagem.trim();
                System.out.println("Mensagem recebida do cliente: [" + mensagem + "]");

                if (mensagem.isEmpty()) continue;

                if ("LISTAR".equals(mensagem)) {
                    JSONArray jArray = new JSONArray();
                    synchronized (pessoas) {
                        for (Pessoa p : pessoas) {
                            JSONObject obj = new JSONObject();
                            obj.put("id", p.id);
                            obj.put("cpf", p.cpf);
                            obj.put("nome", p.nome);
                            obj.put("idade", p.idade);
                            obj.put("profissao", p.profissao);
                            jArray.put(obj);
                        }
                    }
                    saida.println(jArray.toString());

                } else if (mensagem.startsWith("INSERIR ")) {
                    String jsonStr = mensagem.substring(8).trim();
                    System.out.println("JSON recebido para inserção: [" + jsonStr + "]");

                    try {
                        JSONObject jObj = new JSONObject(jsonStr);
                        Pessoa p = new Pessoa();
                        p.id = jObj.getInt("id");
                        p.cpf = jObj.getString("cpf");
                        p.nome = jObj.getString("nome");
                        p.idade = jObj.getInt("idade");
                        p.profissao = jObj.getString("profissao");

                        pessoas.add(p);
                        saida.println("Pessoa adicionada com sucesso!");

                        // Salva no arquivo após adicionar
                        salvarNoArquivo();
                    } catch (JSONException e) {
                        saida.println("Erro: JSON inválido");
                    }

                } else {
                    saida.println("Comando inválido!");
                }

                System.out.println("Cliente " + endCliente.getHostAddress() + " atendido.");
            }

            System.out.println("Cliente " + socketCliente.getInetAddress().getHostAddress() + " desconectado.");

        } catch (IOException e) {
            System.out.println("Erro E/S: " + e);
        } finally {
            try {
                socketCliente.close();
            } catch (IOException e) {
                System.out.println("Erro ao fechar socket: " + e);
            }
        }
    }

    static class Pessoa {
        int id;
        String cpf;
        String nome;
        int idade;
        String profissao;
    }
}

public class Servidor {
    public static void main(String[] args) throws IOException {

        // Carrega dados do arquivo antes de iniciar o servidor
        Conexao.carregarDoArquivo();

        final int portaDefault = 1500;
        int porta;
        int backlog = 5;
        ServerSocket socketServidor = null;

        if ((args.length == 1))
            porta = Integer.parseInt(args[0]);
        else
            porta = portaDefault;

        while (true) {
            try {
                socketServidor = new ServerSocket(porta, backlog);
                break;
            } catch (IOException e) {
                porta++;
            }
        }

        System.out.println("Servidor Simples ativado. Aguardando Cliente na porta " + porta + "...\n");

        while (true) {
            try {
                Socket socketCliente = socketServidor.accept();
                new Conexao(socketCliente).start();
            } catch (IOException e) {
                System.err.println("Erro de E/S: " + e);
            }
        }
    }
}
