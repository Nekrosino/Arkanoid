#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

int szer = 800, wys = 600, deska_x = 100, deska_y = 20;




int main()
{
    al_init();
    al_install_keyboard(); //instalacja klawiatury
 
    al_install_mouse(); //instalacja myszy
    al_init_font_addon(); //instalacja czcionki
    al_init_image_addon(); //dodawanie bitmap
    ALLEGRO_KEYBOARD_STATE klawiatura; 
    ALLEGRO_DISPLAY* okno = al_create_display(szer, wys); //tworzenie okna
    al_set_window_title(okno, "ARKANOID PROJEKT"); //tytul projektu
    ALLEGRO_BITMAP* deska = al_create_bitmap(deska_x, deska_y); //stworzenie platformy do odbijania
    ALLEGRO_BITMAP* pilka = al_load_bitmap("pilka.png"); //wczytanie pilki
    ALLEGRO_FONT* font8 = al_create_builtin_font();
    al_set_target_bitmap(deska);
    al_clear_to_color(al_map_rgb(200, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(okno));
    int x = 320, y = 550;
    int szerokosc_pilka = al_get_bitmap_width(pilka);
    int wysokosc_pilka = al_get_bitmap_height(pilka);
    al_get_keyboard_state(&klawiatura);
    double czas = al_get_time();
    while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE))
    {
        al_get_keyboard_state(&klawiatura);
        if (al_get_time() > czas + 0.01)
        {
            if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && x < szer - deska_x) x = x + 5;
            if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && x > 0) x = x - 5;
  
            czas = al_get_time(); //zegar do eventow
        }
        al_clear_to_color(al_map_rgb_f(100, 0.5, 0.5)); //tlo planszy
        al_draw_bitmap(deska, x, y, 0); //wywolanie deski
        //al_draw_bitmap(pilka, 400, 300, 0);
        al_draw_scaled_bitmap(pilka, 15, 10, szerokosc_pilka, wysokosc_pilka, 400, 300, 25, 25, 0); //wywolanie pilki
        al_draw_textf(font8, al_map_rgb(255, 255, 0), 10, 10, 0, "x=%3d , y=%3d", x, y); //tekst sluzacy do okreslania gdzie znajudje sie deska - tymczasowy
        al_flip_display();
        al_rest(0.001);
    }
    al_destroy_bitmap(deska);// czyszczenie pamieci
    al_destroy_display(okno);//
    al_destroy_bitmap(pilka); //
    return 0;
}