#include <stdio.h>
#include <stdlib.h>
#define MAX 128
typedef struct Usuario{
    int ID;
    char nombre[MAX];
    char telefono[MAX];
}Usuario;
typedef struct Automovil{
    char modelo[MAX];
    char marca[MAX];
    char placa[MAX];
    int tiempo;
    int ID;
    int ID_usuario;
    int estacionado;
}Automovil;
typedef struct NodoP{
    struct NodoP *sgte;
    Automovil dato;
}NodoP;
typedef struct Pila{
    NodoP *inicio;
    int tam;
}Pila;
typedef struct NodoC{
    struct NodoC *sgte;
    Pila *autos;
}NodoC;
typedef struct Cola{
    NodoC *inicio;
    int tam;
    int flag;
}Cola;
typedef struct NodoL{
    struct NodoL *sgte;
    Cola *lotes;
}NodoL;
typedef struct Lista{
    NodoL *inicio;
    int tam;
}Lista;
void limpiarSTDIN(void);
Pila * crearPila(void);
Cola * crearCola(void);
Lista * crearLista(void);
void agregarAPila(Pila **, Automovil);
void capturarAuto(int);
void agregarLote(Cola **, Pila **);
int verificarIDUsuario(int, FILE *);
int verificarIDAuto(int, FILE *);
void capturarUsuario(FILE *);
void mostrarUsuario(int, FILE *);
void agregarPiso(Lista *);
void sacarAuto(Pila *p);
void modificarAuto(int);
void loteAXPiso(Lista **);
void mostrarEstado(Lista *);
void mostrarLotes(Cola *);
void mostrarAutos(Pila *);
Automovil obtenerAuto(int, int);
void estacionarAuto(Lista *);
void actualizar(void);
int main()
{
    int op;
    FILE *fp = NULL;
    Lista *estacionamiento = crearLista();
    do{
        printf("1 -> Agregar usuario\n2 -> Estacionar auto\n3 -> Agregar piso\n4 -> Agregar lote a X piso\n5 -> Mostrar estado del estacionamiento (-1 para salir): ");
        scanf("%d", &op);
        limpiarSTDIN();
        switch(op){
        case -1:
            break;
        case 1:
            capturarUsuario(fp);
            break;
        case 2:
            if(estacionamiento->tam > 0){
                estacionarAuto(estacionamiento);
            }else printf("\x1b[31mNo ha agregado pisos\x1b[0m\n");
            break;
        case 3:
            agregarPiso(estacionamiento);
            printf(" ----------------------------\n");
            printf("|Se ha agregado un nuevo piso|\n");
            printf(" ----------------------------\n");
            break;
        case 4:
            loteAXPiso(&estacionamiento);
            break;
        case 5:
            mostrarEstado(estacionamiento);
            break;
        default:
            printf("\x1b[31mOpcion invalida\x1b[0m\n");
            break;
        }

    }while(op != -1);

    return 0;
}
void limpiarSTDIN(void){
    int c;
    do{
        c = getchar();
    }while(c != '\n' && c != EOF);
}
Pila * crearPila(void){
    Pila *nueva = malloc(sizeof(Pila));
    nueva->tam = 0;
    nueva->inicio = NULL;
    return nueva;
}
Cola * crearCola(void){
    Cola *nueva = malloc(sizeof(Cola));
    nueva->tam = 0;
    nueva->inicio = NULL;
    return nueva;
}
Lista * crearLista(void){
    Lista *nueva = malloc(sizeof(Lista));
    nueva->tam = 0;
    nueva->inicio = NULL;
    return nueva;
}
void actualizar(void){
    FILE *fp = fopen("automoviles.dat", "r+b");
    if(fp != NULL){
        Automovil a;
        fread(&a, sizeof(Automovil), 1, fp);
        while(!feof(fp)){
            a.estacionado = 0;
            long pos = ftell(fp) - (long)sizeof(Automovil);
            fseek(fp, pos, SEEK_SET);
            fwrite(&a, sizeof(Automovil), 1, fp);
            fread(&a, sizeof(Automovil), 1, fp);
        }
    }
    fclose(fp);
}
void mostrarUsuario(int ID, FILE *fp){
    fp = fopen("usuarios.dat", "r+b");
    if(fp != NULL){
        if(verificarIDUsuario(ID, fp)){
            Usuario aux;
            fread(&aux, sizeof(Usuario), 1, fp);
            while(!feof(fp)){
                if(aux.ID == ID){
                    printf("Nombre del usuario -> %s", aux.nombre);
                    printf("Telefono del usuario -> %s", aux.telefono);
                    printf("ID del usuario -> %d\n", aux.ID);
                    break;
                }
                fread(&aux, sizeof(Usuario), 1, fp);
            }
        }else fprintf(stderr, "\x1b[31mUsuario no encontrado\x1b[0m\n");
    }else fprintf(stderr, "\x1b[31mError al abrir el archivo\x1b[0m\n");
}
void agregarAPila(Pila **p, Automovil a){
    NodoP *nuevo = malloc(sizeof(NodoP));
    nuevo->dato = a;
    nuevo->sgte = NULL;
    if((*p)->tam < 3){
        if((*p)->inicio == NULL){
            (*p)->inicio = nuevo;
        }else{
            NodoP *ptr = (*p)->inicio;
            while(ptr->sgte != NULL){
                ptr = ptr->sgte;
            }
            ptr->sgte = nuevo;
        }
        (*p)->tam++;
    }else printf("\x1b[31mNo hay cupo en el lote\x1b[0m\n");
}
int verificarIDUsuario(int ID, FILE *fp){
    fp = fopen("usuarios.dat", "r+b");
    Usuario aux;
    fread(&aux, sizeof(Usuario), 1, fp);
    while(!feof(fp)){
        if(aux.ID == ID) return 1;
        fread(&aux, sizeof(Usuario), 1, fp);
    }
    fclose(fp);
    return 0;
}
int verificarIDAuto(int ID, FILE *fp){
    fp = fopen("automoviles.dat", "r+b");
    Automovil aux;
    fread(&aux, sizeof(Automovil), 1, fp);
    while(!feof(fp)){
        if(aux.ID == ID) return 1;
        fread(&aux, sizeof(Automovil), 1, fp);
    }
    fclose(fp);
    return 0;
}
void capturarUsuario(FILE *fp){
    fp = fopen("usuarios.dat", "a+b");
    Usuario nuevo;
    printf("Nombre del usuario: ");
    fgets(nuevo.nombre, MAX, stdin);
    printf("Telefono del usuario: ");
    fgets(nuevo.telefono, MAX, stdin);
    printf("ID del usuario: ");
    scanf("%d", &nuevo.ID);
    limpiarSTDIN();
    while(verificarIDUsuario(nuevo.ID, fp)){
        fprintf(stderr, "\x1b[31mYa existe un usuario con ese ID, ingrese otro: \x1b[0m\n");
        scanf("%d", &nuevo.ID);
        limpiarSTDIN();
    }
    fwrite(&nuevo, sizeof(Usuario), 1, fp);
    fclose(fp);
    int op;
    printf("\x1b[36m---Capturando automoviles del cliente con ID %d---\x1b[0m\n", nuevo.ID);
    do{
        capturarAuto(nuevo.ID);
        printf("\x1b[36m* -1 para salir *\x1b[0m\n");
        scanf("%d", &op);
        limpiarSTDIN();
    }while(op != -1);
}
void sacarAuto(Pila *p){
    if(p->tam == 0) printf("\x1b[31mNo hay autos estacionados\x1b[0m\n");
    else{
        Automovil aux;
        FILE *fp = NULL;
        if(p->tam == 1){
            aux = p->inicio->dato;
            free(p->inicio);
            p->inicio = NULL;

        }else{
            NodoP *ptr = p->inicio;
            while(ptr->sgte->sgte != NULL){
                ptr = ptr->sgte;
            }
            NodoP *e = ptr->sgte;
            aux = e->dato;
            ptr->sgte = NULL;
            free(e);
        }
        printf("\x1b[36m---Automovil a sacar---\x1b[0m\n");
        printf("Marca -> %s", aux.marca);
        printf("Modelo -> %s", aux.modelo);
        printf("Placa -> %s", aux.placa);
        printf("Tiempo de guardado -> %d\n", aux.tiempo);
        printf("\x1b[36m---Informacion sobre el dueño---\x1b[0m\n");
        mostrarUsuario(aux.ID_usuario, fp);
        p->tam--;
    }
}
void agregarLote(Cola **c, Pila **p){
    NodoC *nuevo = malloc(sizeof(NodoC));
    nuevo->sgte = NULL;
    nuevo->autos = *p;
    (*c)->flag = 1;
    if((*c)->inicio == NULL){
        (*c)->inicio = nuevo;
    }else{
        NodoC *ptr = (*c)->inicio;
        while(ptr->sgte != NULL){
            ptr = ptr->sgte;
        }
        ptr->sgte = nuevo;
    }
    (*c)->tam++;
}
void agregarPiso(Lista *l){
    NodoL *nuevo = malloc(sizeof(NodoL));
    nuevo->sgte = NULL;
    nuevo->lotes = crearCola();
    if(l->inicio == NULL) l->inicio = nuevo;
    else{
        NodoL *ptr = l->inicio;
        while(ptr->sgte != NULL) ptr = ptr->sgte;
        ptr->sgte = nuevo;
    }
    l->tam++;
}
void capturarAuto(int ID_usuario){
    Automovil nuevo;
    FILE *fp;
    fp = fopen("automoviles.dat", "a+b");
    nuevo.ID_usuario = ID_usuario;
    nuevo.estacionado = 0;
    nuevo.tiempo = -1;
    printf("Marca del auto: ");
    fgets(nuevo.marca, MAX, stdin);
    printf("Modelo del auto: ");
    fgets(nuevo.modelo, MAX, stdin);
    printf("Placa del auto: ");
    fgets(nuevo.placa, MAX, stdin);
    printf("ID del auto: ");
    scanf("%d", &nuevo.ID);
    limpiarSTDIN();
    while(verificarIDAuto(nuevo.ID, fp)){
        printf("\x1b[31mYa existe un automovil con tal ID, ingrese uno valido: \x1b[0m\n");
        scanf("%d", &nuevo.ID);
        limpiarSTDIN();
    }
    fwrite(&nuevo, 1, sizeof(Automovil), fp);
    fclose(fp);
}
Automovil obtenerAuto(int ID_usuario, int ID_auto){
    FILE *usuarios = fopen("usuarios.dat", "r+b");
    FILE *autos = fopen("automoviles.dat", "r+b");
    if(usuarios != NULL && autos != NULL){
        Usuario aux;
        int existe = 0, valido = 0;
        fread(&aux, sizeof(Usuario), 1, usuarios);
        while(!feof(usuarios)){
            if(aux.ID == ID_usuario){
                existe = 1;
                break;
            }
            fread(&aux, sizeof(Usuario), 1, usuarios);
        }
        if(existe){
            Automovil a;
            existe = 0;
            fread(&a, sizeof(Automovil), 1, autos);
            while(!feof(autos)){
                if(a.ID == ID_auto){
                    existe = 1;
                    if(ID_usuario == a.ID_usuario) valido = 1;
                    break;
                }
                fread(&a, sizeof(Automovil), 1, autos);
            }
            if(existe){
                if(valido){
                    if(a.estacionado == 0){
                        return a;
                    }else printf("\x1b[31mAutomovil ya estacionado\x1b[0m\n");
                }
                else printf("\x1b[31mTal automovil no pertenece al usuario\x1b[0m\n");
            }else printf("\x1b[31mNo existe el automovil\x1b[0m\n");
        }else printf("\x1b[31mNo existe el usuario\x1b[0m\n");
    }
    fclose(usuarios);
    fclose(autos);
    Automovil at;
    at.ID = -128;
    return at;
}
void modificarAuto(int ID){
    FILE *fp = fopen("automoviles.dat", "r+b");
    if(fp != NULL){
       Automovil aux;
       fread(&aux, sizeof(Automovil), 1, fp);
       while(!feof(fp)){
           if(aux.ID == ID){
               long pos = ftell(fp) - (long)sizeof(Automovil);
               fseek(fp, pos, SEEK_SET);
               aux.estacionado = 1;
               fwrite(&aux, sizeof(Automovil), 1, fp);
               break;
           }
           fread(&aux, sizeof(Automovil), 1, fp);
       }
    }
    fclose(fp);
}
void loteAXPiso(Lista **l){
    int index;
    printf("Ingrese el numero del piso: ");
    scanf("%d", &index);
    limpiarSTDIN();
    if((*l)->tam > 0){
        if(index >= 0 && index < (*l)->tam){
            int cont = 0;
            NodoL *ptr = (*l)->inicio;
            while(ptr != NULL){
                if(cont == index){
                    Pila *p = crearPila();
                    agregarLote(&ptr->lotes, &p);
                    break;
                }
                ptr = ptr->sgte;
                cont++;
            }
        }else printf("\x1b[31mFuera de rango\x1b[0m\n");
    }else printf("\x1b[31mNo hay pisos\x1b[0m\n");
}
void estacionarAuto(Lista *l){
    int ID_usuario, ID_auto;
    printf("ID del usuario: ");
    scanf("%d", &ID_usuario);
    limpiarSTDIN();
    printf("ID del auto: ");
    scanf("%d", &ID_auto);
    limpiarSTDIN();
    Automovil a = obtenerAuto(ID_usuario, ID_auto);
    if(a.ID != -128){
        int tiempo;
        printf("Ingrese numero de dias de estacionado: ");
        scanf("%d", &tiempo);
        limpiarSTDIN();
        int dias[50], ind = -1;
        dias[0] = 1;
        for(int i = 1; i < 50; i++) dias[i] = dias[i-1] + 2;
        if(tiempo >= 0 && tiempo < 1) ind = 0;
        else if(tiempo < 0) printf("\x1b[31mTiempo invalido\x1b[0m\n");
        if(ind != 0 && tiempo > 0){
            for(int i = 1; i < 50; i++){
                if(tiempo >= dias[i-1] && tiempo < dias[i]){
                    ind = i;
                    break;
                }
            }
        }
        if(ind != -1){
            if(ind < l->tam){
                int cont = 0;
                NodoL *ptr = l->inicio;
                while(ptr != NULL){
                    if(cont == ind) break;
                    ptr = ptr->sgte;
                    cont++;
                }
                Cola **c = &ptr->lotes;
                if((*c)->tam > 0){
                    NodoC *ptrC = (*c)->inicio;
                    while(ptrC != NULL){
                        if(ptrC->autos->tam < 3){
                            agregarAPila(&ptrC->autos, a);
                            modificarAuto(a.ID);
                            break;
                        }
                        ptrC = ptrC->sgte;
                    }
                }else printf("\x1b[31mHay pisos disponibles, pero no hay lotes\x1b[0m\n");
            }else printf("\x1b[31mNo hay pisos para ese automovil\x1b[0m\n");
        }
    }
}
void mostrarLotes(Cola *c){
    NodoC *ptr = c->inicio;
    int cont = 0;
    while(ptr != NULL){
        printf("---Lote %d---\n", cont);
        mostrarAutos(ptr->autos);
        ptr = ptr->sgte;
        cont++;
    }
}
void mostrarAutos(Pila *p){
    NodoP *ptr = p->inicio;
    while(ptr != NULL){
        printf("ID del auto -> %d\n", ptr->dato.ID);
        printf("Placa del auto -> %s", ptr->dato.placa);
        ptr = ptr->sgte;
    }
}
void mostrarEstado(Lista *l){
    NodoL *ptr = l->inicio;
    int cont = 0;
    while(ptr != NULL){
        printf("---Piso %d---\n", cont);
        mostrarLotes(ptr->lotes);
        ptr = ptr->sgte;
        cont++;
    }
}
