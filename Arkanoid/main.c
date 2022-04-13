#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

int szer = 800, wys = 600;  //rozmiary okna
int deska_x = 100, deska_y = 20; //rozmiary deski
int p_x = 400, p_y = 320; //rozmiary pilki
enum Direction {
   LEWO,
   GORA,
   DOL,
   PRAWO
}Dir = DOL; // odbijanie od krawedzi

int main()
{

    al_init();
    al_install_keyboard(); //instalacja klawiatury
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    al_install_mouse(); //instalacja myszy
    al_init_font_addon(); //instalacja czcionki
    al_init_image_addon(); //dodawanie bitmap
    al_init_primitives_addon();
    ALLEGRO_KEYBOARD_STATE klawiatura;
    ALLEGRO_DISPLAY* okno = al_create_display(szer, wys); //tworzenie okna
    al_set_window_title(okno, "ARKANOID PROJEKT"); //tytul projektu
    ALLEGRO_BITMAP* deska = al_create_bitmap(deska_x, deska_y); //stworzenie platformy do odbijania
    ALLEGRO_BITMAP* pilka = al_load_bitmap("pilka.png"); //wczytanie pilki
    ALLEGRO_FONT* font8 = al_create_builtin_font(); //czcionka
    al_set_target_bitmap(deska);
    al_clear_to_color(al_map_rgb(200, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(okno));

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(okno));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    int x = 320, y = 550; // umiejscowienie deski
    int szerokosc_pilka = al_get_bitmap_width(pilka);
    int wysokosc_pilka = al_get_bitmap_height(pilka);
    al_get_keyboard_state(&klawiatura); //odczyt z klawiatury
    ALLEGRO_EVENT event;
    //double czas = al_get_time();
    al_start_timer(timer);
    while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE))
    {
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&klawiatura);
  
        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && x < szer - deska_x) x = x + 8;
            if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && x > 0) x = x - 8;

  
            //sciany
            //PRAWO
            if (p_x >= szer)
            {
                Dir = LEWO;

            }
    
         
       
            //LEWO
         
            else if (p_x <= 0)
            {
                Dir = PRAWO;

            }

            //GORA
            if (p_y == 0)
            {
                Dir = DOL;
            }

            //DOL
            else if (p_y == wys )
            {
                Dir = GORA;
             }

        
            //Pilka ruch
            if (Dir == GORA) 
            {
                    p_x -= 1;
                    p_y -= 4;
        
            }
            else  if (Dir == DOL) {
                p_y += 4;
                p_x -= 2;
               
            }
             else if (Dir == LEWO) {
                p_x -= 2;
                p_y -= 2;
            }
             else  if (Dir == PRAWO) {
                p_x += 4;
                p_y += 2;
            }


            al_clear_to_color(al_map_rgb_f(100, 0.5, 0.5)); //tlo planszy
            al_draw_bitmap(deska, x, y, 0); //wywolanie deski
            //al_draw_bitmap(pilka, 400, 300, 0);
            al_draw_scaled_bitmap(pilka, 15, 10, szerokosc_pilka, wysokosc_pilka, p_x, p_y, 25, 25, 0); //wywolanie pilki
            al_draw_textf(font8, al_map_rgb(255, 255, 0), 10, 10, 0, "x=%3d , y=%3d", x, y); //tekst sluzacy do okreslania gdzie znajudje sie deska - tymczasowy
       
            al_flip_display();
            al_rest(0.001);
        }
    }
    al_destroy_bitmap(deska);// czyszczenie pamieci
    al_destroy_display(okno);//
    al_destroy_bitmap(pilka); //
    return 0;
}