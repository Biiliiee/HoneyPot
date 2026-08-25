// clang-format off
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
// clang-format on

void GravarLog(const char *mensagem) {

  HANDLE hArquivo =
      CreateFileA("meu_log.log", FILE_APPEND_DATA, FILE_SHARE_READ, NULL,
                  OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hArquivo == INVALID_HANDLE_VALUE) {
    printf("Erro ao abrir/criar o arquivo de log!\n");
    return;
  }

  DWORD bytesEscritos;
  DWORD tamanhoTexto = (DWORD)strlen(mensagem);

  if (!WriteFile(hArquivo, mensagem, tamanhoTexto, &bytesEscritos, NULL)) {
    printf("Erro ao escrever no arquivo!\n");
  }

  CloseHandle(hArquivo);
}

bool buscar_processo_por_pid(DWORD pidAlvo, PROCESSENTRY32 *pe32Out) {
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  if (hSnapshot == INVALID_HANDLE_VALUE) {
    return false;
  }

  pe32Out->dwSize = sizeof(PROCESSENTRY32);
  bool encontrado = false;

  if (Process32First(hSnapshot, pe32Out)) {
    do {
      if (pe32Out->th32ProcessID == pidAlvo) {
        encontrado = true;
        break;
      }
    } while (Process32Next(hSnapshot, pe32Out));
  }

  CloseHandle(hSnapshot);
  return encontrado;
}

int main() {

  DWORD meuPid = GetCurrentProcessId();
  PROCESSENTRY32 dadosMeuProcesso;

  if (buscar_processo_por_pid(meuPid, &dadosMeuProcesso)) {
    printf("[+] Processo Atual: %s | PID: %lu\n", dadosMeuProcesso.szExeFile,
           meuPid);

    DWORD ppid = dadosMeuProcesso.th32ParentProcessID;
    PROCESSENTRY32 dadosProcessoPai;

    if (buscar_processo_por_pid(ppid, &dadosProcessoPai)) {
      printf("[+] Processo Pai:   %s | PPID: %lu\n", dadosProcessoPai.szExeFile,
             ppid);

      SYSTEMTIME st;
      GetLocalTime(&st);

      char buffer[512];
      snprintf(buffer, sizeof(buffer),
               "========================================\n"
               " Data/Hora: %02d/%02d/%04d %02d:%02d:%02d\n"
               "[+] Processo Atual: %s | PID: %lu\n"
               "[+] Processo Pai:   %s | PPID: %lu\n"
               "========================================\n\n",
               st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond,
               dadosMeuProcesso.szExeFile, meuPid, dadosProcessoPai.szExeFile,
               ppid);

      GravarLog(buffer);

    } else {
      printf("[!] Processo pai com o PID %lu nao foi encontrado.\n", ppid);
    }

  } else {
    printf("[!] Processo com o PID %lu nao foi encontrado.\n", meuPid);
  }

  return 0;
}