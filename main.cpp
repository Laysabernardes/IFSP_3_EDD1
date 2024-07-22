#include <iostream>
#include <locale.h>
#include <string>
#include <vector>

using namespace std;

// Estrutura de um nó da fila
struct No {
    int dado;
    No *prox;
};

// Estrutura de uma fila
struct Fila {
    No *ini;
    No *fim;
};

// Função para inicializar uma fila
Fila* init() {
    Fila *f = new Fila;
    f->ini = NULL;
    f->fim = NULL;
    return f;
}

// Função para verificar se a fila está vazia
int isEmpty(Fila *f) {
    return (f->ini == NULL);
}

// Função para adicionar um elemento à fila (enfileirar)
void enqueue(Fila *f, int v) {
    No *no = new No;
    no->dado = v;
    no->prox = NULL;
    if (isEmpty(f)) {
        f->ini = no;
    }
    else {
        f->fim->prox = no;
    }
    f->fim = no;
}

// Função para remover um elemento da fila (desenfileirar)
int dequeue(Fila *f) {
    int ret;
    if (isEmpty(f)) {
        ret = -1;
    }
    else {
        No *no = f->ini;
        ret = no->dado;
        f->ini = no->prox;
        if (f->ini == NULL) {
            f->fim = NULL;
        }
        delete no;
    }
    return ret;
}

// Função para contar a quantidade de elementos na fila
int count(Fila *f) {
    int qtde = 0;
    No *no;
    no = f->ini;
    while (no != NULL) {
        qtde++;
        no = no->prox;
    }
    return qtde;
}

// Função para imprimir os elementos da fila
void print(Fila *f) {
    No *no;
    no = f->ini;
    cout << "-- Qtde de elementos: " << count(f) << endl;
    while (no != NULL) {
        cout << no->dado << endl;
        no = no->prox;
    }
}

// Função para liberar a memória da fila
void freeFila(Fila *f) {
    No *no = f->ini;
    while (no != NULL) {
        No *temp = no->prox;
        delete no;
        no = temp;
    }
    delete f;
}

// Estrutura de um guichê
struct Guiche {
    int id;
    Fila *senhasAtendidas;
};

vector<Guiche> guiches;

// Função para encontrar um guichê pelo ID
Guiche* findGuiche(int id) {
    for (size_t i = 0; i < guiches.size(); i++) {
        if (guiches[i].id == id) {
            return &guiches[i];
        }
    }
    return nullptr;
}

void abrirGuiche(int id) {
    Guiche guiche;
    guiche.id = id;
    guiche.senhasAtendidas = init();
    guiches.push_back(guiche);
    cout << "Guiche " << id << " aberto." << endl;
}

void listarSenhasAtendidas(int id) {
    Guiche *guiche = findGuiche(id);
    if (guiche != nullptr) {
        cout << "Senhas atendidas pelo guiche " << id << ":" << endl;
        print(guiche->senhasAtendidas);
    } else {
        cout << "Guiche " << id << " nao encontrado." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "");
    Fila *senhasGeradas = init();
    int opcao;
    int senha = 0;
    int totalAtendidas = 0;

    do {
        string line(40, '-');
        cout << line << endl;
        cout << "0. Sair" << endl;
        cout << "1. Gerar senha" << endl;
        cout << "2. Abrir guiche" << endl;
        cout << "3. Realizar atendimento" << endl;
        cout << "4. Listar senhas atendidas" << endl;
        cout << "Opcao: ";
        cin >> opcao;

        if (opcao == 0 && !isEmpty(senhasGeradas)) {
            cout << "Ainda ha senhas aguardando atendimento. Não e possível sair." << endl;
            opcao = -1;
        }

        switch (opcao) {
            case 0:
                cout << line << endl;
                cout << "Encerrando programa...\n";
                totalAtendidas = 0;
                for (const auto& guiche : guiches) {
                    totalAtendidas += count(guiche.senhasAtendidas);
                }
                cout << "Total de senhas atendidas: " << totalAtendidas << endl;
                break;
            case 1:
                cout << line << endl;
                senha++;
                enqueue(senhasGeradas, senha);
                cout << "Senha gerada: " << senha << endl;
                cout << "Senhas aguardando atendimento: " << count(senhasGeradas) << endl;
                break;
            case 2: {
                cout << line << endl;
                int idGuiche;
                cout << "Digite o ID do guiche a ser aberto: ";
                cin >> idGuiche;
                abrirGuiche(idGuiche);
                cout << "Senhas aguardando atendimento: " << count(senhasGeradas) << endl;
                cout << "Numero de guiches abertos: " << guiches.size() << endl;
                break;
            }
            case 3: {
                cout << line << endl;
                if (isEmpty(senhasGeradas)) {
                    cout << "Nenhuma senha para atender." << endl;
                    break;
                }
                int idGuiche;
                cout << "Digite o ID do guiche: ";
                cin >> idGuiche;
                Guiche *guiche = findGuiche(idGuiche);
                if (guiche != nullptr) {
                    int senhaAtendida = dequeue(senhasGeradas);
                    enqueue(guiche->senhasAtendidas, senhaAtendida);
                    cout << "Senha atendida: " << senhaAtendida << endl;
                } else {
                    cout << "Guiche " << idGuiche << " não encontrado." << endl;
                }
                cout << "Senhas aguardando atendimento: " << count(senhasGeradas) << endl;
                break;
            }
            case 4: {
                cout << line << endl;
                int idGuiche;
                cout << "Digite o ID do guiche: ";
                cin >> idGuiche;
                listarSenhasAtendidas(idGuiche);
                break;
            }
            default:
                cout << "Opção invalida. Tente novamente." << endl;
        }

    } while (opcao != 0);

    freeFila(senhasGeradas);
    for (auto& guiche : guiches) {
        freeFila(guiche.senhasAtendidas);
    }

    return 0;
}