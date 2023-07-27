#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_ACCOUNTS 100

struct Login {
    int account_code;
    bool logged_in;
};

struct Account {
    char name[100], last_name[100], password[100];
    int age;
    float balance;
};

struct Login log_status;
struct Account accounts[MAX_ACCOUNTS];
bool run = true;
int num_accounts = 0;

void clear_terminal() {
    printf("\033[H\033[2J");
}


int print_account(int account_code) {
    printf("\n\n*************************************\n\n");
    printf("Usuário: %s %s\n", accounts[account_code].name, accounts[account_code].last_name);
    printf("Idade: %i\n", accounts[account_code].age);
    printf("Seu saldo atual é de R$ %.2f\n", accounts[account_code].balance);
    printf("Código da conta: %i", account_code + 1);
    printf("\n\n*************************************\n\n");
}


void print_menu() {
    printf ("[1] Criar nova conta;\n");
    printf ("[2] Atualizar informações da sua conta;\n");
    printf ("[3] Acessar transações;\n");
    printf ("[4] Verificar detalhes da sua conta;\n");
    printf ("[5] Remover uma conta existente;\n");
    printf ("[6] Saques e depósitos;\n");
    printf ("[7] Sair\n\n");
}


int exiting(void) {
    printf("Deseja voltar ao menu principal?\n");
    printf("Digite [1] para voltar ao menu e [2] para fechar o programa:\n");
    
    int exit_confirmation;
    unsigned int sleepDuration = 2;
    
    do {
        scanf("%i", &exit_confirmation);
        switch (exit_confirmation) {
            case 1:
                clear_terminal();
                printf("Voltando ao menu principal...\n\n");
                sleep(sleepDuration);
                return 1;
                break;
            case 2:
                clear_terminal();
                printf("Fechando o programa...");
                run = false;
                log_status.account_code = -1;
                log_status.logged_in = false;
                return 0;
                break;
            default:
                printf("Opção inválida, por favor, tente novamente:\n");
                break;
        }
    } while (exit_confirmation != 1 && exit_confirmation != 2);
}


int money_move(void) {
    int option;
    float value;
    
    clear_terminal();
    
    printf("Selecione um tipo de movimentação.\n");
    do {
        printf("Digite [1] para saques e [2] para depósitos.\n");
        scanf("%i", &option);
        switch (option) {
            case 1:
                printf("\n");
                printf("Qual valor você deseja retirar?\n");
                scanf("%f", &value);
                if (value <= accounts[log_status.account_code].balance && value > 0) {
                    accounts[log_status.account_code].balance -= value;
                    
                    printf("\n");
                    printf("%.2f retirado da sua conta, saldo atual: R$ %.2f\n", value, accounts[log_status.account_code].balance);
                    exiting();
                    return 1;
                    
                } else if (value > 0) {
                    printf("\n");
                    printf("Saldo insuficiente para realizar o saque.\n\n");
                    exiting();
                    return 1;
                    
                } else {
                    printf("\n");
                    printf("Opção inválida, por favor, tente novamente.\n");
                } 
                break;
            case 2:
                printf("\n");
                printf("Qual valor você deseja depositar?\nR$ ");
                scanf("%f", &value);
                
                accounts[log_status.account_code].balance += value;
                
                printf("\n");
                printf("Valor de %.2f depositado com sucesso!\n", value);
                printf("Saldo atual: R$ %.2f\n\n", accounts[log_status.account_code].balance);
                exiting();
                return 1;
                
                break;
            default:
                printf("\n");
                printf("Opção inválida, por favor, tente novamente.\n");
                break;
        }
    } while(true);
}


int create_account(void) {
    float balance = 0.0;
    
    clear_terminal();

    printf ("Digite seu nome: ");
    scanf ("%s", accounts[num_accounts].name);
    printf ("Digite seu sobrenome: ");
    scanf ("%s", accounts[num_accounts].last_name);
    printf ("Digite sua Idade: ");
    scanf ("%i", &accounts[num_accounts].age);
    
    if (accounts[num_accounts].age >= 18) {
        printf ("Crie uma senha: ");
        scanf ("%s", accounts[num_accounts].password);
        
        char confirm[100];
        printf ("Confirme sua senha: ");
        scanf ("%s", confirm);
        
        while (strcmp(accounts[num_accounts].password, confirm) != 0) {
            printf ("\nSenha não confirmada, por favor, tente novamente.\n");
            printf ("Crie uma senha: ");
            scanf ("%s", accounts[num_accounts].password);
            printf ("Confirme sua senha: ");
            scanf ("%s", confirm);
        }
        
        clear_terminal();
        printf("Conta criada com sucesso!");
        print_account(num_accounts);
        
        log_status.account_code = num_accounts;
        log_status.logged_in = true;
        num_accounts += 1;
        
        exiting();
        return 1;
    } else {
        clear_terminal();
        printf ("Você precisa ser maior de idade para criar uma conta.\n");
        printf ("Voltando para o menu principal...\n\n");
        return 0;
    }
}


bool account_access(void) {
    int account_code, attempts = 0;
    char validation[100];
    
    clear_terminal();
    
    printf("Digite o código de sua conta: ");
    while (true) {
        scanf("%i", &account_code);
        
        account_code--;
        
        clear_terminal();
        
        if (account_code < num_accounts && accounts[account_code].name[0] != '\0') {
            printf("Bem vindo(a) %s %s\n", accounts[account_code].name, accounts[account_code].last_name);
            if (account_code != log_status.account_code || log_status.logged_in == false) {
                printf("Para acessar sua conta, por favor digite sua senha: ");
                do {
                    scanf("%s", validation);
                    
                    if (strcmp(validation, accounts[account_code].password) == 0) {
                        
                        print_account(account_code);
                        
                        exiting();
                        log_status.account_code = account_code;
                        log_status.logged_in = true;
                        return true;
                        
                    } else {
                        attempts++;
                        clear_terminal();
                        printf("Acesso negado. Senha incompátivel.\n");
                        printf("Tentativas feitas %i\n", attempts);
                        printf("Tente novamente: ");
        
                    }
                } while(attempts < 3);
                
                clear_terminal();
                printf("Você excedeu o número de tentativas.\n");
                printf("Voltanto ao menu principal...\n\n");
                return false;
            
            } else {
                print_account(account_code);
                  
                exiting();
                log_status.account_code = account_code;
                log_status.logged_in = true;
                return true;
            }
                
        } else {
            printf("Conta não encontrada, por favor, tente novamente.\n");
        }
    }
}


int main (void) {
    setlocale (LC_ALL, "Portuguese");
    int option, exit_confirmation;
    
    log_status.logged_in = false;
    unsigned int sleepDuration = 2;
    
    printf ("Bem vindo(a) ao nosso banco!\n");
    printf ("Para começar, por favor escolha um das opções abaixo\n");
    do {
        printf ("[1] Criar uma conta;\n[2] Realizar log-in;\n[3] Sair\n");
        scanf ("%i", &option);
        printf ("\n\n");
        switch (option) {
            case 1:
                if (num_accounts < 100) {
                        printf ("Criando uma nova conta...\n\n");
                        sleep(sleepDuration);
                        create_account();
                        
                    } else {
                        printf ("Infelizmente, nosso sistema de contas está cheio no momento.\n");
                        printf ("Por favor, tente novamente mais tarde...\n");
                    }
                break;
            case 2:
                printf ("Iniciando log-in\n");
                sleep(sleepDuration);
                account_access();
                break;
            case 3:
                printf ("Saindo...");
                return 0;
                run = false;
                break;
            default:
                clear_terminal();
                printf ("Opção inválida.\nPor favor, digite uma opção existente.\n\n");
                break;
        }
    } while(log_status.logged_in == false);
    
    clear_terminal();
    
    printf("Por favor, escolha uma das opções abaixo:\n\n");
    
    do {
        print_menu();
        
        scanf ("%i", &option);
        printf ("\n");
        
        switch (option) {
            case 1:
                printf ("Criando uma nova conta\n");
                sleep(sleepDuration);
                create_account();
                break;
            case 2:
                printf ("Atualizando informações\n");
                break;
            case 3:
                 printf ("Acessando transações\n");
                 break;
            case 4:
                printf ("Verificando detalhes de sua conta\n");
                sleep(sleepDuration);
                account_access();
                break;
            case 5:
                printf ("Removendo sua conta\n");
                break;
            case 6:
                if (log_status.account_code != -1 && log_status.logged_in) {
                    printf ("Acessando área de saques e depósitos...\n");
                    sleep(sleepDuration);
                    money_move();
                } else {
                    printf("Você precisa realizar o login antes de acessar a área de saques e depósitos.");
                    sleep(sleepDuration);
                    account_access();
                }
                break;
            case 7:
                printf ("Deseja mesmo sair?\n");
                printf ("Digite [1] para 'sim' e [2] para 'não'.\n");
                scanf ("%i", &exit_confirmation);
              
                if (exit_confirmation == 1) {
                    clear_terminal();
                    printf ("Saindo...");
                    log_status.account_code = -1;
                    log_status.logged_in = false;
                    run = false;
                }
              
                break;
            default:
                clear_terminal();
                printf ("Opção inválida.\nPor favor, digite uma opção existente.\n\n");
                break;
        }
    } while(run);

    return 0;
}