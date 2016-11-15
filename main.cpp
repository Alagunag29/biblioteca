#include <allegro.h>
#include <ctype.h>
#include <stdio.h>

#define BUFFERSIZE 128


typedef struct libros{
    char nombre_libro[100];
    char nombre_autor[100];
    char categoria_libro[20];
    int dias_maximo_prestado;
    bool estado;
    BITMAP *libro;
    libros *siguiente;
    libros *anterior;
}libros;

libros *cabeza = NULL;

typedef struct personaBiblioteca{
     char nombre[100];
     char codigo[20];
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
BITMAP *pru;

/* variables para almacenar las las cadenas introducidas por el teclado*/
char edittext[BUFFERSIZE];
int  caret  = 0;

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
//-----graficas



//logicas
void crearLibros(char, char, char, int, bool, BITMAP);
void crearPersonas(char, char, int, int, int);

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
    fondoConsultarLibroDet = load_bitmap("imagenes/fondoB6.bmp", NULL);
    fondoAdminDevolver = load_bitmap("imagenes/fondoB5.bmp", NULL);
    fondoAdminRegistro = load_bitmap("imagenes/fondoB6.bmp", NULL);
    raton = load_bitmap("imagenes/raton.bmp", NULL);
    ratonA = load_bitmap("imagenes/ratonA.bmp", NULL);
    pru = load_bitmap("imagenes/vacio.bmp", NULL);
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
                   pintaSegundoMenuEscogido(1);
               }  
          }
          if(mouse_y > 472 && mouse_y < 541)
          {
               draw_sprite(buffer, ratonA, mouse_x, mouse_y); /*curso cuando se encuentra en un boton*/
               if(mouse_b & 1 ){
                   pintaSegundoMenuEscogido(2);
               }
          }
          if(mouse_y > 596 && mouse_y < 667)
          {          
               draw_sprite(buffer, ratonA, mouse_x, mouse_y); /*curso cuando se encuentra en un boton*/
               if(mouse_b & 1 ){   
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
            opcionAdmin(1);
        }
    }
    if(mouse_x > 936 && mouse_x < 1110 && mouse_y > 527 && mouse_y < 575){
        draw_sprite(buffer, ratonA, mouse_x, mouse_y);
        if(mouse_b & 1 ){
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
               }
         }else{
                draw_sprite(buffer, raton, mouse_x, mouse_y);   
         }
         // condicion que devuelve atras en este caso 
         //eventoClicPorSelecionVista(vista);
      }  
    
}

/*Impresion de los libros */
void impresionLibros(int opcion, char categoria[100])
{
     int x = 40, y = 160;
     libros *p;
     draw_sprite(screen, buffer, 0, 0);
     clear(buffer);
     draw_sprite(buffer, fondoConsultarLibroDet, 0, 0);

     for(p = cabeza; p != NULL; p = p->siguiente){   
           if( opcion == 1){
               if( (strcmp(p->categoria_libro, categoria) == 0) ){
                   draw_sprite(buffer, p->libro, x, y); 
                   textprintf(buffer, font, x, 450, makecol(255, 255, 255), p->categoria_libro);
                   textprintf(buffer, font, x, 465, makecol(255, 255, 255), p->nombre_libro);
                   textprintf(buffer, font, x, 475, makecol(255, 255, 255), p->nombre_autor);
                   if(p->estado)
                       textprintf(buffer, font, x, 485, makecol(255, 255, 255),"Disponible");
                   else
                       textprintf(buffer, font, x, 485, makecol(255, 255, 255),"Ocupado");
                    
                   x = x + 220;
               }
           }else{
              if( (strcmp(p->nombre_libro, categoria) == 0)  ){
                   draw_sprite(buffer, p->libro, x, y);   
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
          pintarFontSegunVista(5);
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

/*Creacion de la lista que contendra todos los libros*/
void crearLibros(char nombre_libro[100], char nombre_autor[100], char categoria[20], int dias , bool estado, BITMAP *lib)
{
     libros *NODO = (libros*)malloc(sizeof(libros));
     strcpy(NODO->nombre_libro, nombre_libro);
     strcpy(NODO->nombre_autor, nombre_autor);
     strcpy(NODO->categoria_libro, categoria);
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
void crearPersonas(char nombre[20], char cod[20], int faltas, int lib_prestados, int lib_pres_actual){
     personaBiblioteca *NODO = (personaBiblioteca*)malloc(sizeof(personaBiblioteca));
    strcpy(NODO->nombre, nombre);
    strcpy(NODO->codigo, cod);
    NODO->faltas = faltas;
    NODO->libros_prestados = lib_prestados;
    NODO->libros_prestados_Actual = lib_pres_actual;
    if (cabezaB == NULL){
         NODO->siguiente = NULL;
         cabezaB = NODO;
     }else{
         NODO->siguiente = cabezaB;
         cabezaB = NODO;
     }
}


int main(void)
{
    init();
    crearLibros("calculo integral", "Burro", "matematica", 3 , true, load_bitmap("imagenes/libroC3.bmp", NULL));
    crearLibros("algoritmo", "Cole", "programacion", 3 , true, load_bitmap("imagenes/libroP1.bmp", NULL));
    crearLibros("calculo integral", "Otro cole", "matematica", 3 , true, load_bitmap("imagenes/libroC2.bmp", NULL));
    crearLibros("calculo diferencial", "Hanssel", "matematica", 3 , true, load_bitmap("imagenes/libroC3.bmp", NULL));
    crearLibros("medicina", "Andres", "matematica", 3 , true, load_bitmap("imagenes/libroC4.bmp", NULL));
    crearLibros("calculo integral", "Andres", "medicina", 3 , true, load_bitmap("imagenes/libroC1.bmp", NULL));
    crearLibros("estadistica", "Hanssel", "matematica", 3 , true, load_bitmap("imagenes/libroC1.bmp", NULL));
    crearLibros("fisica", "you lenon", "fisica", 6, true, load_bitmap("imagenes/libroFQ1.bmp", NULL));
     crearLibros("vacio", "vacio", "defaul", 0 , true, load_bitmap("imagenes/vacio.bmp", NULL));
        

    while(!key[KEY_ESC]){
         draw_sprite(screen, buffer, 0, 0);
         clear(buffer);
         draw_sprite(buffer, fondoHome, 0, 0);
         HomePasoCAP(); 
    }
}
END_OF_MAIN();
