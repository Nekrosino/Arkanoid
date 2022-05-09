#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

int szer = 800, wys = 600;  //rozmiary okna
int deska_x = 100, deska_y = 20; //rozmiary deski
int p_x = 400, p_y = 320; //rozmiary pilki
enum Direction {
    LEWO,
    GORA,
    DOL,
    PRAWO
}Dir = GORA; // odbijanie od krawedzi

bool kolizja(float paddle_x, float paddle_y, int szer, int wys, int p_y, int p_x, int paddle_width, int paddle_height)
{
    if (p_y == paddle_y)
    {
        if ((p_x >= paddle_x && p_x < paddle_x + paddle_width))
            return true;
        else return false;
    }
    else return false;
}

bool brick_collision(float brick_x, float brick_y, int szer, int wys, int p_y, int p_x, int brick_width, int brick_height)
{
    if (p_y == brick_y)
    {
        if ((p_x >= brick_x && p_x < brick_x + brick_width))

            return true;
        else return false;
    }
    else return false;
}
bool angle_normal, angle_large;
int check_bar_collision(int ball_x, int ball_y, int bar_x, int bar_y, int bar_width, int bar_height)
{
    if (ball_y >= bar_y)
    {
        //left and right side parts of bar bounces off with larger angle
        if ((ball_x >= bar_x && ball_x < bar_x + bar_width / 5) || (ball_x > bar_x + (bar_width - bar_width / 5)) && (ball_x <= bar_x + bar_width))
            return angle_large;

        else if (ball_x >= bar_x + bar_width / 5 && ball_x <= bar_x + (bar_width - bar_width / 5))
            return angle_normal;
    }
    return 0;
}
int main()
{

    al_init();
    al_install_keyboard(); //instalacja klawiatury
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    ALLEGRO_SAMPLE* sample = NULL;

    al_install_mouse(); //instalacja myszy
    al_init_font_addon(); //instalacja czcionki
    al_init_image_addon(); //dodawanie bitmap
    al_init_primitives_addon();
    al_install_audio();
    al_init_acodec_addon();

    // al_reserve_sampler(1);

    sample = al_load_sample("hit.mp3");
    ALLEGRO_KEYBOARD_STATE klawiatura;
    ALLEGRO_DISPLAY* okno = al_create_display(szer, wys); //tworzenie okna
    al_set_window_title(okno, "ARKANOID PROJEKT"); //tytul projektu
    //ALLEGRO_BITMAP* deska = al_create_bitmap(deska_x, deska_y); //stworzenie platformy do odbijania
    ALLEGRO_BITMAP* paddle = al_load_bitmap("deska.png");
    ALLEGRO_BITMAP* red_brick = al_load_bitmap("red.png");
    int paddle_width = al_get_bitmap_width(paddle);
    int paddle_height = al_get_bitmap_height(paddle);
    int brick_width = al_get_bitmap_width(red_brick);
    int brick_height = al_get_bitmap_height(red_brick);
    int brick_x = 300;
    int brick_y = 100;
    ALLEGRO_BITMAP* pilka = al_load_bitmap("pilka.png"); //wczytanie pilki
    ALLEGRO_FONT* font8 = al_create_builtin_font(); //czcionka
   // al_set_target_bitmap(deska);
    al_clear_to_color(al_map_rgb(200, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(okno));

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(okno));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    int paddle_x = 320, paddle_y = 520; // umiejscowienie deski
    int szerokosc_pilka = al_get_bitmap_width(pilka);
    int wysokosc_pilka = al_get_bitmap_height(pilka);
    al_get_keyboard_state(&klawiatura); //odczyt z klawiatury
    ALLEGRO_EVENT event;
    //double czas = al_get_time();
    al_start_timer(timer);
    int exist = 1;
    al_reserve_samples(1);
    while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE))
    {
        al_wait_for_event(queue, &event);
        al_get_keyboard_state(&klawiatura);

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && paddle_x < szer - paddle_width) paddle_x += 8;
            if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && paddle_x > 0) paddle_x -= 8;


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

            // GORA
            else if (p_y <= 0)
            {
                Dir = DOL;
            }

            //DOL

            else if (p_y >= wys)
            {
                Dir = GORA;
            }
            int kolizja_paddle = kolizja(paddle_x, paddle_y, szer, wys, p_y, p_x, paddle_width, paddle_height);
            if (kolizja_paddle == 1)
            {
                Dir = GORA;
            }

            //Cegla
            int brickcollision = brick_collision(brick_x, brick_y, szer, wys, p_y, p_x, brick_width, brick_height);

            if (brickcollision == 1)
            {
                al_play_sample(sample, 1, 0, 5, ALLEGRO_PLAYMODE_ONCE, NULL);
                Dir = DOL;
                exist = 0;
            }
            // 
            //deska
           // int bar_collision = check_bar_collision(p_x, p_y, x, y, deska_x, deska_y);
          //  if (bar_collision == angle_normal)
           // {
            //    Dir = GORA;

          //  }
          //  else if (bar_collision == angle_large)
          //  {


            //    Dir = GORA;



          //  }

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



            //font_caption = al_load_font("fonts/FFF_Tusj.ttf", 60, 0);


            al_clear_to_color(al_map_rgb_f(100, 0.5, 0.5)); //tlo planszy
            //al_draw_bitmap(deska, x, y, 0); //wywolanie deski
            al_draw_bitmap(paddle, paddle_x, paddle_y, 1);
            // al_draw_bitmap(pilka, 400, 300, 1);

            if (exist == 1)
            {
                al_draw_bitmap(red_brick, brick_x, brick_y, 1);


            }
            else if (exist == 0)
            {
                al_draw_text(font8, al_map_rgb(0, 255, 0), 350, 300, 0, "Wygrales");
            }

            al_draw_scaled_bitmap(pilka, 15, 10, szerokosc_pilka, wysokosc_pilka, p_x, p_y, 25, 25, 0); //wywolanie pilki
           // al_draw_textf(font8, al_map_rgb(255, 255, 0), 10, 10, 0, "x=%3d , y=%3d", x, y); //tekst sluzacy do okreslania gdzie znajudje sie deska - tymczasowy

            al_flip_display();
            al_rest(0.001);
        }
    }
    // al_destroy_bitmap(deska);// czyszczenie pamieci
    al_destroy_display(okno);//
    al_destroy_bitmap(pilka);
    al_destroy_sample(sample);//
    return 0;
}
