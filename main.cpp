#include <allegro.h>
#include <ctype.h>
#include <stdio.h>
#include <ctime>

#define BUFFERSIZE 128


typedef struct libros{
    char nombre_libro[100];
    char nombre_autor[100];
    char categoria_libro[20];
    char id_libro[20];
    char id_usuario[20];
    int dias_maximo_prestado;
    bool estado;
    BITMAP *libro;
    libros *siguiente;
    libros *anterior;
}libros;

libros *cabeza = NULL;

typedef struct personaBiblioteca{
     char nombre[100];
     char cedula[20];
     int faltas;
     int libros_prestados;
     int libros_prestados_Actual;  
     personaBiblioteca *siguiente;                 
}personaBiblioteca;

personaBiblioteca *cabezaB = NULL;



/*Apuntadores que relacionan la parte grafica*/
BITMAP *buffer;
BITMAP *fondoHome, *fondoConsultar, *fondoAdmin, *fondoPoliticas;
BITMAP *fondoConsultarLibro, *fondoConsultarLibroDet;
BITMAP *fondoAdminDevolver, *fondoAdminRegistro;
BITMAP *raton, *ratonA, *fondoAux;
BITMAP *fondoReservar;
BITMAP *fondoError, *fondoExitoso, *fondoVacioL;

/* variables para almacenar las las cadenas introducidas por el teclado*/
char edittext[BUFFERSIZE];
char ID[BUFFERSIZE];
char NOMBRE[BUFFERSIZE];
char CEDULA[BUFFERSIZE];
int  caret = 0, caretID = 0, caretNOMBRE = 0, caretCEDULA  = 0;

/*
  Declaracion de funciones con la parte grafica 
*/

void init(void);
void load(void);

//paso de vistas 
void HomePasoCAP(void);
void pintaSegundoMenuEscogido(int);
void eventoClicPorSelecionVista(int);
void pintarFont(void);
void vistaConsultar(void);
void vistaAdmin(void); 
void vistaPoliticas(void);
void pintarFontSegunVista(int);
void opcionAdmin(int);
void mostrarLibro(int, char[]);
void devolverLibro(int);
void impresionLibros(int, char[]);
void pintarResultados(int);
void mostrarRegistro();

void vistaReserva();
void camposFormulario();
void pintarFontFormulario(int);

void pintarFontID();
void pintarFontNombre();
void pintarFontCedula();
//-----graficas

//logicas
void crearLibros(char[], char[], char[], char[], int, bool, BITMAP);
void crearPersonas(char[], char[]);
bool estadoLibro(char[], bool);
void cambiarEstadoLibro(char[] , bool, char[]);
void definicionFallas();
void buscarPersona(char[], char[], char[]);
void entregaLibro(char[]);
void entrega(char[], int);

// inicializar allegro y los componente de la app
void init()
{
    
    int depth, res;
    allegro_init();
    depth = desktop_color_depth();
    if (depth == 0)
        depth = 32;
    set_color_depth(depth);
    res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1300, 700, 0, 0);
    if (res != 0){
        allegro_message(allegro_error);
        exit(-1);      
    }
    install_keyboard();
    install_mouse();
    load();
}

/*
  carga de todas las imagenes y el buffer en su respectivo BITMAP
*/
void load(void)
{
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    fondoHome = load_bitmap("imagenes/fondoB1.bmp", NULL);
    fondoConsultar = load_bitmap("imagenes/fondoB4.bmp", NULL);
    fondoAdmin = load_bitmap("imagenes/fondoB2.bmp", NULL);
    fondoPoliticas = load_bitmap("imagenes/fondoB3.bmp", NULL);
    fondoConsultarLibro = load_bitmap("imagenes/fondoB6.bmp", NULL);
    fondoConsultarLibroDet = load_bitmap("imagenes/fondoB7.bmp", NULL);
    fondoAdminDevolver = load_bitmap("imagenes/fondoB5.bmp", NULL);
    fondoAdminRegistro = load_bitmap("imagenes/fondoB6.bmp", NULL);
    raton = load_bitmap("imagenes/raton.bmp", NULL);
    ratonA = load_bitmap("imagenes/ratonA.bmp", NULL);
    fondoReservar = load_bitmap("imagenes/fondoB8.bmp", NULL);
    fondoError = load_bitmap("imagenes/fondoError.bmp", NULL);
    fondoExitoso = load_bitmap("imagenes/fondoExitoso.bmp", NULL);
    fondoVacioL = load_bitmap("imagenes/fondoVacioL", NULL);
}

/*
  Funcionamiento de los botones de la pagina principal
  al tener el cursor en esa area seleccionada, y el evento de clic 
  permite pasar a la siguiente vista
*/
void HomePasoCAP(void)
{
     draw_sprite(buffer, raton, mouse_x, mouse_y); /* cursor por defecto*/  
     if(mouse_x > 88 && mouse_x < 272 )
     {
          if(mouse_y > 345 && mouse_y < 418)
          {
               draw_sprite(buffer, ratonA, mouse_x, mouse_y); /*curso cuando se encuentra en un boton*/
               if(mouse_b & 1 ){
                   strcpy(edittext,"");
                   caret = 0;
                   pintaSegundoMenuEscogido(1);
               }  
          }
          if(mouse_y > 472 && mouse_y < 541)
          {
               draw_sprite(buffer, ratonA, mouse_x, mouse_y); /*curso cuando se encuentra en un boton*/
               if(mouse_b & 1 ){
                    strcpy(edittext,"");
                    caret = 0;
                   pintaSegundoMenuEscogido(2);
               }
          }
          if(mouse_y > 596 && mouse_y < 667)
          {          
               draw_sprite(buffer, ratonA, mouse_x, mouse_y); /*curso cuando se encuentra en un boton*/
               if(mouse_b & 1 ){   
                   strcpy(edittext,"");
                   caret = 0;
                   pintaSegundoMenuEscogido(3);
               }
          }
     }   
}

/*
  Pinta el segundo menu
*/
void pintaSegundoMenuEscogido(int vista)
{
     bool atras = true;
     
      if(vista == 1){
         fondoAux =  fondoConsultar;
      }else if( vista == 2){
         fondoAux =  fondoAdmin;
         vista = 2;
      }else if(vista == 3){
         fondoAux = fondoPoliticas;
      } 
      
      while(atras){
         draw_sprite(screen, buffer, 0, 0);
         clear(buffer);
         draw_sprite(buffer, fondoAux, 0, 0);
         pintarFontSegunVista(vista);
         ///ojo -> entregaLibro(id;
         if(mouse_x > 50 && mouse_x < 84 && mouse_y > 639 && mouse_y < 669)
         {
              draw_sprite(buffer, ratonA, mouse_x, mouse_y);
              if(mouse_b & 1 ){   
                   atras = false;
                   strcpy(edittext,"");
                   caret = 0;
               }
         }else{
              draw_sprite(buffer, raton, mouse_x, mouse_y);   
         }
         // condicion que devuelve atras en este caso 
         eventoClicPorSelecionVista(vista);
      }  
}

/*Permite saber a que menu paso, para pasar a otra vista */
void eventoClicPorSelecionVista(int vista)
{
    if(vista == 1){
         vistaConsultar();
    }else if( vista == 2){
         vistaAdmin();    
    }
}

/*ddddd*/
void vistaConsultar()
{    
     char opcionLibro[100];
     if (mouse_x >560 && mouse_x < 740 && mouse_y > 605 && mouse_y < 655){
         draw_sprite(buffer, ratonA, mouse_x, mouse_y);
         if(mouse_b & 1 ){   
             strcpy(opcionLibro, edittext);
             strcpy(edittext,"");
             caret = 0;
             mostrarLibro(1, opcionLibro);
         }
     }
     if(mouse_x >785 && mouse_x < 960 && mouse_y > 605 && mouse_y < 655){
         draw_sprite(buffer, ratonA, mouse_x, mouse_y);
         if(mouse_b & 1 ){   
             strcpy(opcionLibro, edittext);
             strcpy(edittext,"");
             caret = 0;
             mostrarLibro(2, opcionLibro);
         }
     }
}

void vistaAdmin()
{    
    if(mouse_x > 370 && mouse_x < 545 && mouse_y > 527 && mouse_y < 575){
        draw_sprite(buffer, ratonA, mouse_x, mouse_y);
        if(mouse_b & 1){
            strcpy(edittext,"");
            caret = 0; 
            opcionAdmin(1);//////////////////////////////////////////////////////////////////////////////////////
        }
    }
    if(mouse_x > 936 && mouse_x < 1110 && mouse_y > 527 && mouse_y < 575){
        draw_sprite(buffer, ratonA, mouse_x, mouse_y);
        if(mouse_b & 1 ){
            strcpy(edittext,"");
            caret = 0;
            opcionAdmin(2);
        }
    }
}

/*Mostrar el libro segun su selecion al buscar*/
void mostrarLibro(int x, char mostrarLibro[100])
{
     bool atras = true;
     while(atras){
         impresionLibros(x, mostrarLibro);
         
         if(mouse_x > 50 && mouse_x < 84 && mouse_y > 639 && mouse_y < 669)
         {
              draw_sprite(buffer, ratonA, mouse_x, mouse_y);
              if(mouse_b & 1 ){   
                   atras = false;
                   strcpy(edittext,"");
                   caret = 0;
               }
         }else{
              if(mouse_x > 1105 && mouse_x < 1270 && mouse_y > 642 && mouse_y < 695){
                   draw_sprite(buffer, ratonA, mouse_x, mouse_y);
                   if(mouse_b & 1){
                       strcpy(edittext,"");
                       caret = 0;
                       vistaReserva();
                   }
              }else{
                  draw_sprite(buffer, raton, mouse_x, mouse_y);  
              }
         }
         // condicion que devuelve atras en este caso 
         //eventoClicPorSelecionVista(vista);
      }  
    
}

/*Impresion de los libros */
void impresionLibros(int opcion, char categoria[100])
{
     int x = 40, y = 160; // cordenadas x, y para pintar libro
     libros *p;
     draw_sprite(screen, buffer, 0, 0);
     clear(buffer);
     draw_sprite(buffer, fondoConsultarLibroDet, 0, 0);

     for(p = cabeza; p != NULL; p = p->siguiente){   
           if( opcion == 1){
               if( (strcmp(p->categoria_libro, categoria) == 0) ){
                   draw_sprite(buffer, p->libro, x, y); 
                   textprintf(buffer, font, x, 450, makecol(255, 255, 255), "Id: %s", p->id_libro);
                   textprintf(buffer, font, x, 465, makecol(255, 255, 255), "Categoria: %s", p->categoria_libro);
                   textprintf(buffer, font, x, 480, makecol(255, 255, 255), "Titulo: %s", p->nombre_libro);
                   textprintf(buffer, font, x, 495, makecol(255, 255, 255), "Autor: %s", p->nombre_autor);
                   if(p->estado)
                       textprintf(buffer, font, x, 510, makecol(255, 255, 255),"Estado: Disponible");
                   else
                       textprintf(buffer, font, x, 510, makecol(255, 255, 255),"Estado: Ocupado");
                   x = x + 250;
               }
           }else{
              if( (strcmp(p->nombre_libro, categoria) == 0)  ){
                   draw_sprite(buffer, p->libro, x, y);
                   textprintf(buffer, font, x, 450, makecol(255, 255, 255),"Id: %s", p->id_libro);
                   textprintf(buffer, font, x, 465, makecol(255, 255, 255), "Categoria: %s", p->categoria_libro);
                   textprintf(buffer, font, x, 480, makecol(255, 255, 255), "Titulo: %s", p->nombre_libro);
                   textprintf(buffer, font, x, 495, makecol(255, 255, 255), "Autor: %s", p->nombre_autor);
                   if(p->estado)
                       textprintf(buffer, font, x, 510, makecol(255, 255, 255),"Estado: Disponible");
                   else
                       textprintf(buffer, font, x, 510, makecol(255, 255, 255),"Estado: Ocupado");   
                   x = x + 220;
               }
           }
           
     }
     if(x == 40 && y == 160){
          draw_sprite(buffer, cabeza->libro, x, y);   
     } 
     
}


/*
  devolver libro en el administrador
*/
void opcionAdmin(int vista){
     bool atras = true;
     BITMAP *fond;
     
     if(vista == 1){
         fond = fondoAdminDevolver;
     }else if(vista == 2){
         fond = fondoAdminRegistro;
     }
     while(atras){
          draw_sprite(screen, buffer, 0, 0);
          clear(buffer);
          draw_sprite(buffer, fond, 0, 0);
          if(vista == 1){
              pintarFontSegunVista(5);
          }
          if(mouse_x > 50 && mouse_x < 84 && mouse_y > 639 && mouse_y < 669)
          {
              draw_sprite(buffer, ratonA, mouse_x, mouse_y);
              if(mouse_b & 1 ){   
                   atras = false;
                   strcpy(edittext,"");
                   caret = 0;
               }
          }else if(mouse_x > 630 && mouse_x < 824 && mouse_y > 555 && mouse_y < 600){
              draw_sprite(buffer, ratonA, mouse_x, mouse_y); 
              if(mouse_b & 1){
                  entregaLibro(edittext);
              }
              
          }else{
              draw_sprite(buffer, raton, mouse_x, mouse_y);
          }             
     }
}

/*Pinta la linea por donde se esta escibiendo y la palabra al mismo tiempo */
void pintarFontSegunVista(int vista)
{
     if(vista == 1){
          pintarFont();
          textout(buffer, font, edittext, 590, 520, makecol(255, 255, 255));
          vline(buffer,(590 + (caret*8)), 515, 545, makecol(255, 255, 255));
     }else if(vista == 5){
          pintarFont();
          textout(buffer, font, edittext, 590, 460, makecol(255, 255, 255));
          vline(buffer, (590 + (caret*8)), 450, 480, makecol(255, 255, 255)); 
     }
       
}

void vistaReserva()
{
    int posicionForm = 0;
    bool atras = true;
    while(atras){
        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
        draw_sprite(buffer, fondoReservar, 0, 0);
        textout(buffer, font, ID, 582, 222, makecol(255, 255, 255));
        textout(buffer, font, NOMBRE, 582, 348, makecol(255, 255, 255));
        textout(buffer, font, CEDULA, 582, 479, makecol(255, 255, 255));
        draw_sprite(buffer, raton, mouse_x, mouse_y);
        if(mouse_x > 582 && mouse_x < 940){
            if(mouse_y > 222 && mouse_y < 270){
                draw_sprite(buffer, ratonA, mouse_x, mouse_y);
                if(mouse_b & 1){
                    posicionForm = 1;
                    caretID = caretNOMBRE = caretCEDULA = 0;
                    strcpy(ID, "");
                    strcpy(NOMBRE, "");
                    strcpy(CEDULA, "");
                }
            }else if(mouse_y > 348 && mouse_y < 395){
                draw_sprite(buffer, ratonA, mouse_x, mouse_y);
                if(mouse_b & 1){
                    posicionForm = 2;
                }
            }else if(mouse_y > 470 && mouse_y < 518){
                draw_sprite(buffer, ratonA, mouse_x, mouse_y);
                if(mouse_b & 1){
                    posicionForm = 3;
                }
            }else
                draw_sprite(buffer, raton, mouse_x, mouse_y); 
        }
        if(mouse_x > 50 && mouse_x < 84 && mouse_y > 639 && mouse_y < 669){
            draw_sprite(buffer, ratonA, mouse_x, mouse_y);
            if(mouse_b & 1 ){
                strcpy(edittext,"");
                caret = 0;
                atras = false;
            }
        }
        
        if(mouse_x > 650 && mouse_x < 820 && mouse_y > 600 && mouse_y < 650){
            draw_sprite(buffer, ratonA, mouse_x, mouse_y);
            if(mouse_b & 1){
                   if( !( ( strcmp(ID,"") == 0) && (strcmp(NOMBRE, "") == 0) && (strcmp(CEDULA, "") == 0) ) ){
                        buscarPersona(ID, NOMBRE, CEDULA);//*********************************************************************
                        caretID = caretNOMBRE = caretCEDULA = 0;
                        strcpy(ID, "");
                        strcpy(NOMBRE, "");
                        strcpy(CEDULA, "");
                   }
            }
        }
         
        
        pintarFontFormulario(posicionForm); 
       
        /*para saber en cual se tiene que ir escribiendo*/
    }
}

/*clic en cada campo del registro de datos para poder prestar el libro*/
void pintarFontFormulario(int pos)
{
     if(pos == 1){
         pintarFontID();
         textout(buffer, font, ID, 582, 222, makecol(255, 255, 255));
         vline(buffer, (582 + (caretID*8)), 222, 260, makecol(255, 255, 255)); 
     }else if(pos == 2){
        pintarFontNombre();
        textout(buffer, font, NOMBRE, 582, 348, makecol(255, 255, 255));
        vline(buffer, (582 + (caretNOMBRE*8)), 350, 390, makecol(255, 255, 255)); 
     }else if(pos == 3){
        pintarFontCedula();
        textout(buffer, font, CEDULA, 582, 479, makecol(255, 255, 255));
        vline(buffer, (582 + (caretCEDULA*8)), 470, 518, makecol(255, 255, 255)); 
     }
}


void pintarResultados(int n)
{
    bool atras = true;
    BITMAP *opcionFondo;
   if(n == 1){
        opcionFondo = fondoExitoso;
   if(n == 2){
        opcionFondo = fondoError;
   }
   if(n == 3){
        opcionFondo = fondoVacioL;
   }
    while(atras){
        draw_sprite(screen, buffer, 0, 0);
        clear(buffer);
        draw_sprite(buffer, opcionFondo, 0, 0);
        if(mouse_x > 50 && mouse_x < 84 && mouse_y > 639 && mouse_y < 669)
        {
              draw_sprite(buffer, ratonA, mouse_x, mouse_y);
              if(mouse_b & 1 ){   
                   atras = false;
                   strcpy(edittext,"");
                   caret = 0;
               }
          }else{
                draw_sprite(buffer, raton, mouse_x, mouse_y);
          }
    }     
}
}
/*pintada y creacion de fuente*/
void pintarFont(void)
{
      if(keypressed())
      {
         int  newkey   = readkey();
         char ASCII    = newkey & 0xff;
         char scancode = newkey >> 8;
 
         /* Al presionar una caracter por el teclado se lo añade a la cadena */
         if(ASCII >= 32 && ASCII <= 126)
         {
            if(caret < BUFFERSIZE - 1)
            {
               edittext[caret] = ASCII;
               caret++;
               edittext[caret] = '\0';
            }
         }
         else if(scancode == KEY_BACKSPACE)
         {
            if (caret > 0) caret--;
            edittext[caret] = '\0';
         }
      }
}

/*Creacion de font del registro*/
void pintarFontID(void)
{
      if(keypressed())
      {
         int  newkey   = readkey();
         char ASCII    = newkey & 0xff;
         char scancode = newkey >> 8;
 
         /* Al presionar una caracter por el teclado se lo añade a la cadena */
         if(ASCII >= 32 && ASCII <= 126)
         {
            if(caretID < BUFFERSIZE - 1)
            {
               ID[caretID] = ASCII;
               caretID++;
               ID[caretID] = '\0';
            }
         }
         else if(scancode == KEY_BACKSPACE)
         {
            if (caretID > 0) caretID--;
            ID[caretID] = '\0';
         }
      }
}

void pintarFontNombre(void)
{
      if(keypressed())
      {
         int  newkey   = readkey();
         char ASCII    = newkey & 0xff;
         char scancode = newkey >> 8;
 
         /* Al presionar una caracter por el teclado se lo añade a la cadena */
         if(ASCII >= 32 && ASCII <= 126)
         {
            if(caretNOMBRE < BUFFERSIZE - 1)
            {
               NOMBRE[caretNOMBRE] = ASCII;
               caretNOMBRE++;
               NOMBRE[caretNOMBRE] = '\0';
            }
         }
         else if(scancode == KEY_BACKSPACE)
         {
            if (caretNOMBRE > 0) caretNOMBRE--;
            NOMBRE[caretNOMBRE] = '\0';
         }
      }
}

void pintarFontCedula(void)
{
      if(keypressed())
      {
         int  newkey   = readkey();
         char ASCII    = newkey & 0xff;
         char scancode = newkey >> 8;
 
         /* Al presionar una caracter por el teclado se lo añade a la cadena */
         if(ASCII >= 32 && ASCII <= 126)
         {
            if(caretCEDULA < BUFFERSIZE - 1)
            {
               CEDULA[caretCEDULA] = ASCII;
               caretCEDULA++;
               CEDULA[caretCEDULA] = '\0';
            }
         }
         else if(scancode == KEY_BACKSPACE)
         {
            if (caretCEDULA > 0) caretCEDULA--;
            CEDULA[caretCEDULA] = '\0';
         }
      }
}

/*Creacion de la lista que contendra todos los libros*/
void crearLibros(char nombre_libro[100], char nombre_autor[100], char categoria[20], char id[20], int dias , bool estado, BITMAP *lib)
{
     libros *NODO = (libros*)malloc(sizeof(libros));
     strcpy(NODO->nombre_libro, nombre_libro);
     strcpy(NODO->nombre_autor, nombre_autor);
     strcpy(NODO->categoria_libro, categoria);
     strcpy(NODO->id_libro, id);
     strcpy(NODO->id_usuario, "");
     NODO->dias_maximo_prestado = dias;
     NODO->estado = estado;
     NODO->libro = lib;
     NODO->siguiente = NULL;
     NODO->anterior = NULL;

     if (cabeza == NULL){
         cabeza = NODO;
     }else{
         cabeza->anterior = NODO;
         NODO->siguiente = cabeza;
         cabeza = NODO;
     } 
}

//void crearPersonas(char, char, int, int, int);
void crearPersonas(char nombre[], char cc[] ){
    personaBiblioteca *NODO = (personaBiblioteca*)malloc(sizeof(personaBiblioteca));
    strcpy(NODO->nombre, nombre);
    strcpy(NODO->cedula, cc);
    NODO->faltas = 0;
    NODO->libros_prestados = 1;
    NODO->libros_prestados_Actual = 1;
    if (cabezaB == NULL){
         NODO->siguiente = NULL;
         cabezaB = NODO;
     }else{
         NODO->siguiente = cabezaB;
         cabezaB = NODO;
     }
}

/*
resive como parametro el estado que se necesite 
recordando que true = estado disponible, false es que ya se encuentra ocupado
y por lo tanto retornara false la funcion, indicando que no se puede hacer
el registro.
*/
bool estadoLibro(char id[], bool est){
     libros *p;
     int sw = 0;
     for(p=cabeza; p!=NULL && sw == 0; p = p->siguiente){
         if(strcmp(p->id_libro, id) == 0  && p->estado == est){
             sw = 1;
         }
     }
     if (sw == 1){
        return true;   
     }else{
        return false;
     }
}

/*
 busco personas que este en la biblioteca
 primero miramos si el id conside con un libro verdadero y si se puede 
 prestar el libro 
*/
void buscarPersona(char id[], char nombre[], char cc[] ){
    personaBiblioteca *p;
    int sw = 0;
    bool estado = estadoLibro( id, true);
    if (estado == true){
        for(p = cabezaB; p != NULL && sw == 0; p = p->siguiente){
            if( strcmp(p->cedula, cc)== 0 ){
                if(p->faltas  <= 3){
                    p->libros_prestados +=1;
                    p->libros_prestados_Actual +=1;
                    sw = 1;
                }else{
                   pintarResultados(2); ///*****************************************************************************
                }
            }
        }
        if(sw == 0){
           crearPersonas(nombre, cc);
        }
        cambiarEstadoLibro(id, false, cc);
        pintarResultados(1); //*******************************************************************************************
        //hacer una funcion que mande una vista diciendo como quedo el estado 
    }else{
       pintarResultados(2);
       //******************************************************************************************************************
    }
}

/*Buscar al libro y cambiar su estado respectivamente
  y le asigna el id al usuario en la lista de libros 
  para referenciar al libro con un usuario
*/
void cambiarEstadoLibro(char id[], bool estd, char cc[]){
     libros *p;
     int sw = 0;
     for(p=cabeza; p != NULL && sw == 0; p = p->siguiente){
         if( strcmp(p->id_libro, id) == 0 ){
             p->estado = estd; 
             strcpy(p->id_usuario, cc); 
             sw = 1;   
         }
     }
}

/*define fallas al usuario 
generando un numero aleaorios que seran los dias que tenia el libro
si el numero es mayor que el plazo de Dias dispuesto por la biblioteca a 
dicho libro de le sumar una multa 
tambien cambiara el estado del libro automaticamente */
void entregaLibro(char id[])
{
    int numeroAleat;
    bool estado = estadoLibro(id, false);
    if (estado){
       srand(time(0));
       numeroAleat =  1 + rand()% 10;
       entrega(id, numeroAleat);
       cambiarEstadoLibro(id, true, "");
    }else{
          pintarResultados(0);
    //error al colocar id  ***********************************************************************************************************     
    }
}

void entrega(char id[], int diaEntregado)
{
     char cc_usuario;
     int sw = 0;
     int diasMax;
     libros *p;
     personaBiblioteca *j;
     for( p = cabeza; p != NULL && sw == 0; p = p->siguiente){
          if( strcmp(p->id_libro, id) == 0){
              diasMax = p->dias_maximo_prestado;
              sw = 1;
              break;
          }
     }
     for(j = cabezaB; j != NULL; j = j->siguiente){
         if(j->cedula, p->id_usuario){
             j->libros_prestados_Actual-=1;
             if(diasMax > diaEntregado){
                j->faltas+=1;
             }
         }
     }
     
     
}
/*
void mostrarRegistro()
{
     personaBiblioteca *p;
     int x = 69;
     int y = 163;
     
     for(p = cabezaB; p != NULL; p = p->siguiente ){
           textprintf(buffer, font, 60, y+15, makecol(255, 255, 255), "Nombre: %s", p->nombre);
           textprintf(buffer, font, 60, y+15, makecol(255, 255, 255), "Cedula: %s", p->cedula);
           textprintf(buffer, font, 60, y+15, makecol(255, 255, 255), "Faltas: %d", p->faltas);
           textprintf(buffer, font, 60, y+15, makecol(255, 255, 255), "Libros Actuales %d", p->libros_prestados_Actual);
           textprintf(buffer, font, 60, y+15, makecol(255, 255, 255), "Libros Total Prestados: %d", p->libros_prestados);
           y=y+75;
     }
     if(cabezaB == NULL){
         pintarResultados(3);
     }
}*/
/*
typedef struct personaBiblioteca{
     char nombre[100];
     char cedula[20];
     int faltas;
     int libros_prestados;
     int libros_prestados_Actual;  
     personaBiblioteca *siguiente;                 
}personaBiblioteca;

*/



int main(void)
{
    init();
    crearLibros("calculo integral", "Burro", "matematica", "123", 3 , true, load_bitmap("imagenes/libroC3.bmp", NULL));
    crearLibros("algoritmo", "Cole", "programacion","1234" ,  3 , true, load_bitmap("imagenes/libroP1.bmp", NULL));
    crearLibros("calculo integral", "Otro cole", "matematica", "12345", 3 , true, load_bitmap("imagenes/libroC2.bmp", NULL));
    crearLibros("calculo diferencial", "Hanssel", "matematica", "123456", 3 , true, load_bitmap("imagenes/libroC3.bmp", NULL));
    crearLibros("medicina", "Andres", "matematica", "1234567", 3 , true, load_bitmap("imagenes/libroC4.bmp", NULL));
    crearLibros("calculo integral", "Andres", "medicina", "321", 3 , true, load_bitmap("imagenes/libroC1.bmp", NULL));
    crearLibros("estadistica", "Hanssel", "matematica", "1212", 3 , true, load_bitmap("imagenes/libroC1.bmp", NULL));
    crearLibros("fisica", "you lenon", "fisica", "ew11", 6, true, load_bitmap("imagenes/libroFQ1.bmp", NULL));
    crearLibros("vacio", "vacio", "defaul", "dsd2", 0 , true, load_bitmap("imagenes/vacio.bmp", NULL));
    

    while(!key[KEY_ESC]){
         draw_sprite(screen, buffer, 0, 0);
         clear(buffer);
         draw_sprite(buffer, fondoHome, 0, 0);
         HomePasoCAP(); 
    }
}
END_OF_MAIN();
