// Importando bibliotecas

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Declaração das variáveis globais

int profundidade_container;
int linhas;
int colunas;
int numero_ilhas = 0;
int* profundidade_ilha;
int* matriz_alturas; 
int* matriz_ids;     
int* casas_visitadas;
int solucao_encontrada = 0; 
int* melhor_caminho;
int tamanho_melhor_caminho = 0;

// Declaração das funções

void parametros(int argc, char *argv[]);
void matriz(char* nome_matriz);
void buscar_ilhas(int index, int ilha_atual);
void encontrar_ilhas();
int arredondar_profundidade(double profundidade_bruta);
void profundidade_ilhas();
void buscar_combinacao_recursiva(int index, int soma, int *combinacao_parcial, int tamanho_parcial);
void combinacao_containers();
void liberar_memoria(); 

// Função principal

int main(int argc, char *argv[]) {
    parametros(argc, argv);
    encontrar_ilhas();
    printf("%d\n", numero_ilhas);
    if (numero_ilhas > 0) {
        profundidade_ilhas();
        for(int i = 0; i < numero_ilhas; i++) {
            printf("%d%s", profundidade_ilha[i], (i == numero_ilhas - 1) ? "" : " ");
        }
        printf("\n");
        combinacao_containers();
    }
    else {
        printf("\n");
        printf("Nao ha resposta valida!\n");
    }
    liberar_memoria();
    return 0;
}

// Funções auxiliares

void parametros(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <Profundidade Total> <Linhas> <Colunas> <Arquivo Matriz>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    profundidade_container = atoi(argv[1]);
    linhas = atoi(argv[2]);
    colunas = atoi(argv[3]);
    matriz(argv[4]);
}
void matriz(char* nome_matriz) {
    int tamanho_total = linhas * colunas;
    matriz_alturas = (int*)malloc(tamanho_total * sizeof(int));
    matriz_ids = (int*)malloc(tamanho_total * sizeof(int));
    casas_visitadas = (int*)malloc(tamanho_total * sizeof(int)); 
    if (matriz_alturas == NULL || matriz_ids == NULL || casas_visitadas == NULL) {
        fprintf(stderr, "Erro\n");
        exit(EXIT_FAILURE);
    }
    FILE *pont_arq = fopen(nome_matriz, "r");
    if (pont_arq == NULL) {
        fprintf(stderr, "Erro %s\n", nome_matriz);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < tamanho_total; i++) {
        if (fscanf(pont_arq, "%d", &matriz_alturas[i]) != 1) {
            fprintf(stderr, "Erro ao ler dados no arquivo.\n");
            fclose(pont_arq);
            liberar_memoria();
            exit(EXIT_FAILURE);
        }
        matriz_ids[i] = 0; 
        casas_visitadas[i] = 0; 
    }
    fclose(pont_arq);
}
void buscar_ilhas(int index, int ilha_atual) {
    if (index < 0 || index >= linhas * colunas) return;
    if (matriz_alturas[index] == 0) return; 
    if (casas_visitadas[index] == 1) return; 
    casas_visitadas[index] = 1;
    matriz_ids[index] = ilha_atual;
    int linha = index / colunas;
    int coluna = index % colunas;
    if (linha > 0) buscar_ilhas(index - colunas, ilha_atual); 
    if (linha < linhas - 1) buscar_ilhas(index + colunas, ilha_atual); 
    if (coluna > 0) buscar_ilhas(index - 1, ilha_atual); 
    if (coluna < colunas - 1) buscar_ilhas(index + 1, ilha_atual); 
}
void encontrar_ilhas() {
    int tamanho_total = linhas * colunas;    
    for (int i = 0; i < tamanho_total; i++) {
        if (matriz_alturas[i] > 0 && casas_visitadas[i] == 0) {
            numero_ilhas++;
            buscar_ilhas(i, numero_ilhas);
        }
    }
}
int arredondar_profundidade(double profundidade_bruta) {
    if (profundidade_bruta <= 0.0) return 0;    
    if (profundidade_bruta > 0.0 && profundidade_bruta < 1.0) {
        return 1;
    }    
    return (int)profundidade_bruta;
}
void profundidade_ilhas() {
    if (numero_ilhas == 0) return;
    profundidade_ilha = (int*)malloc(numero_ilhas * sizeof(int));
    for (int i = 0; i < numero_ilhas; i++) {
        profundidade_ilha[i] = 0;
    }
    for (int i = 0; i < linhas * colunas; i++) {
        int id = matriz_ids[i];
        if (id != 0) {
            profundidade_ilha[id - 1] += matriz_alturas[i];
        }
    }
    for (int i = 0; i < numero_ilhas; i++) {
        double profundidade_bruta = (double)profundidade_ilha[i] / 6.0;
        profundidade_ilha[i] = arredondar_profundidade(profundidade_bruta);
    }
}
void buscar_combinacao_recursiva(int index, int soma, int *combinacao_parcial, int tamanho_parcial) {
    if (solucao_encontrada) return;
    if (soma == profundidade_container) {
        solucao_encontrada = 1;
        tamanho_melhor_caminho = tamanho_parcial;
        for (int i = 0; i < tamanho_parcial; i++) {
            melhor_caminho[i] = combinacao_parcial[i];
        }
        return;
    }    
    if (soma > profundidade_container || index >= numero_ilhas) {
        return;
    }
    combinacao_parcial[tamanho_parcial] = profundidade_ilha[index];    
    buscar_combinacao_recursiva(
        index + 1, 
        soma + profundidade_ilha[index], 
        combinacao_parcial, 
        tamanho_parcial + 1
    );
    if (!solucao_encontrada) {
        buscar_combinacao_recursiva(
            index + 1, 
            soma, 
            combinacao_parcial, 
            tamanho_parcial 
        );
    }
}
void combinacao_containers() {
    int *combinacao_parcial = malloc(numero_ilhas * sizeof(int));
    melhor_caminho = malloc(numero_ilhas * sizeof(int));    
    buscar_combinacao_recursiva(0, 0, combinacao_parcial, 0);
    if (solucao_encontrada) {
        for (int i = 0; i < tamanho_melhor_caminho; i++) {
            printf("%d%s", melhor_caminho[i], (i == tamanho_melhor_caminho - 1) ? "" : " ");
        }
        printf("\n");
    } else {
        printf("Nao ha resposta valida!\n");
    }

    free(combinacao_parcial);
}
void liberar_memoria() {
    if (matriz_alturas != NULL) free(matriz_alturas);
    if (matriz_ids != NULL) free(matriz_ids);
    if (casas_visitadas != NULL) free(casas_visitadas);
    if (profundidade_ilha != NULL) free(profundidade_ilha);
    if (melhor_caminho != NULL) free(melhor_caminho);
}
