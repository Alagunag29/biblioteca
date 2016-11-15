#include <allegro.h>
#define BUFFERSIZE 128
/*
  estado : (prestado o no )
  nombre : nombre_libro
  autor : nombre_autor
  codigo: cuatro_digitos
  tiempo_entrega: dias;
  categoria: 
*/

/*
  Lista de registrados

*/

typedef struct libros{
    char nombre[20];
    char categoria[20];
    bool estado;
    libros *siguiente;
}libros;

BITMAP *buffer;
BITMAP *fondoHome, *fondoConsultar, *fondoAdmin, *fondoPoliticas;
BITMAP *fondoConsultarLibro, *fondoConsultarLibroDet;
BITMAP *fondoAdminDevolver, *fondoAdminRegistro;
BITMAP *raton, *ratonA, *fondoAux;


char edittext[BUFFERSIZE];
int  caret  = 0;

/*
  Declaracion de funciones Primero las graficas y mas despues las logicas
*/

//-----graficas
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
void mostrarLibro(int, char[]);
//-----graficas



//logicas



int main(void)
{
    init();
    while(!key[KEY_ESC]){
         draw_sprite(screen, buffer, 0, 0);
         clear(buffer);
         draw_sprite(buffer, fondoHome, 0, 0);
         HomePasoCAP(); 
    }
}
END_OF_MAIN();


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
    
    }
    if(mouse_x > 936 && mouse_x < 1110 && mouse_y > 527 && mouse_y < 575){
    
    }
}

/*Mostrar el libro segun su selecion al buscar*/
void mostrarLibro(int x, char mostrarLibro[])
{
     bool atras = true;
     while(atras){
         draw_sprite(screen, buffer, 0, 0);
         clear(buffer);
         draw_sprite(buffer, fondoConsultarLibroDet, 0, 0);
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

/*Pinta la linea por donde se esta escibiendo y la palabra al mismo tiempo */
void pintarFontSegunVista(int vista)
{
     if(vista == 1){
          pintarFont();
          textout(buffer, font, edittext, 590, 520, makecol(255, 255, 255));
          vline(buffer, (590 + (caret*8)), 515, 545, makecol(255, 255, 255));
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

