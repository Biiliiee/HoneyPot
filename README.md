# WinProcHoneypot - Honeypot & Auditoria de Processo Pai (PPID) em C

Projeto desenvolvido em C nativo para Windows (Win32 API) focado em conceitos de Deception Technology (Tecnologia de Engano / Canary Executable) e análise de processos em baixo nível.

---

## Sobre o Projeto

A ideia desse projeto surgiu enquanto eu estudava como o Windows gerencia a memória dos processos e como atacantes executam comandos via linha de comando (powershell.exe, cmd.exe, etc).

Decidi criar um executável armadilha (Honeypot/Canary). Quando alguém ou algum script clica/executa esse binário:
1. Ele não executa a ação maliciosa esperada.
2. Em vez disso, faz um snapshot da memória da máquina em tempo de execução.
3. Descobre quem é o Processo Pai (PPID) que disparou o binário (ex: se veio do PowerShell, CMD ou de um acesso remoto).
4. Grava silenciosamente um log no disco com Data, Hora, PID e Nome do Executável Pai.

---

## Tecnologias e APIs Utilizadas

- Linguagem: C (C99/C11)
- Windows API (windows.h & tlhelp32.h):
  - GetCurrentProcessId(): Pega o PID da própria aplicação.
  - CreateToolhelp32Snapshot(): Cria um snapshot de todos os processos ativos na memória.
  - Process32First() & Process32Next(): Percorre a lista de processos para encontrar o PID e extrair o th32ParentProcessID.
  - CreateFileA() & WriteFile(): Manipulação nativa de I/O para gravação discreta do log no disco sem dependências externas.
  - GetLocalTime(): Captura a data e hora local do sistema para auditoria temporal.

---

## Como Compilar e Rodar

Você pode compilar diretamente usando o GCC (MinGW) no Windows:

```bash
# Compilar o código
gcc Honeypot.c -o Honeypot.exe

# Executar a armadilha
.\Honeypot.exe
```

---

## Exemplo de Saída do Log (meu_log.log)

Cada execução gera uma entrada formatada no arquivo meu_log.log:

```text
========================================
 Data/Hora: 25/08/2026 11:38:40
[+] Processo Atual: Honeypot.exe | PID: 13800
[+] Processo Pai:   powershell.exe | PPID: 8716
========================================
```

---

## Próximos Passos

- [ ] Adicionar disfarce com caixa de diálogo de erro falsa do Windows (MessageBoxA).
- [ ] Envio automático do alerta via Webhook do Discord/Telegram em vez de apenas log local.

---

*Projeto desenvolvido para fins educacionais e de estudo sobre arquitetura interna do Windows e Purple Team.*
