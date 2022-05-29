#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
//szer_paddle 150px
//wys_paddle 57px
//lewy 35
//prawy 35
//srodek 
#define true 1 
#define false 0
bool leftside = true; //mialobyc do kolizji paddla 
bool rightside = true; //to samo
bool center = true; //to dziala

#define kolumny 7 /*!< ilosc kolumn cegiel  */
#define wiersze 3 /*!< ilosc wieszy cegiel */
int szer = 800, wys = 600;  /*!< rozmiary okna  */
int deska_x = 100, deska_y = 20; /*!< rozmiary deski */
int p_x = 400, p_y = 320; /*!< umiejscowienie pilki   */

enum STAN { MENU, LEVEL1,LEVEL2 ,GAMEOVER,RESET }; //stany mozna kombinowac


struct ball //pilka
{
    float x;
    float y;
    float dx;
    float dy;
    int lives;
}ball;

struct brick //cegly
{
    int x;
    int y;
    int zycia;
    int szer;
    int wys;
    bool istnieje;
    int kolor;

}brick[kolumny][wiersze];

struct paddle
{
   int paddle_width;
   int paddle_height;
   int paddle_x;
   int paddle_y;
   int paddle_size;
}paddle;


/** \brief Kolizja deski 
@param[in] <paddle_x> <pozycja x deski >
@param[in] <paddle_y> <pozycja y deski>
@param[in] <szer> <szerokosc okna>
@param[in] <wys> <wysokosc okna>
@param[in] <p_y> <pozycja y pilki>
@param[in] <p_x> <pozycja x pilki>
@param[in] <paddle_widt> <szeokosc deski>
@param[in] <paddle_heigh> <wysokosc deski>

*/
bool paddle_collsion(float paddle_x, float paddle_y, int szer, int wys, int p_y, int p_x, int paddle_width, int paddle_height)
{
    if (p_y >= paddle_y-20)
    {
        if (p_x >= paddle_x && p_x <= paddle_x + paddle_width)
        {
            return center;
        }
        //else if (p_x >= paddle_x + 60 && p_x < paddle_x + 228) //nie dziala
      //  {
       //     return tcenter;
      //  }
       // else if (p_x >= paddle_x + 228 && p_x <= paddle_width)
       // {
      //      return rightside;
       // }
        else return 0;
    }
    else return 0;
}
/** \brief Kolizja cegiel
@param[in] <brick_x> <pozycja x cegly >
@param[in] <brick_y> <pozycja y cegly>
@param[in] <brick_szer> <szerokosc cegly>
@param[in] <brick_wys> <wysokosc cegly>
@param[in] <szer> <szerokosc okna>
@param[in] <wys> <wysokosc okna>
@param[in] <p_y> <pozycja y pilki>
@param[in] <p_x> <pozycja x pilki>

*/
bool brick_collision(int brick_x,int brick_y,int brick_szer,int brick_wys, int szer, int wys, int p_y, int p_x)
{   
 
    if (p_y <= brick_y+40 &&  p_y>= (brick_y + brick_wys)-50)
    {
        if (p_x >= brick_x && p_x <= brick_x + brick_szer) return true;
        else return false;
    }
    else return false;
}
/** \brief Funkcja rederujaca bloki
@param[in] <brick brick[kolumny][wiersze]> <struktura cegly >

*/
void renderuj_bloki(struct brick brick[kolumny][wiersze])
{
    int i, j;
   for(i=0;i<kolumny;i++)
   {
       for (j = 0;j<wiersze;j++)
       {
           brick[i][j].x = 10 +(i*98);
           brick[i][j].y = 50+(j*30);
           brick[i][j].istnieje = true;
          
       }
   }
}
/// Postaw Bloki
/**
* Funckja tworzy bloki cegie³, które zostaly stworzone za pomoc¹ programu gimp, a nastêpnie zainklementowane za pomoc¹ biblioteki allegro.

*/
void postaw_bloki(struct brick brick[kolumny][wiersze], ALLEGRO_BITMAP* red_brick)
{   
    //ALLEGRO_BITMAP* brick_1 = al_load_bitmap("red.png");
    
    int i, j;
    for (i = 0; i < kolumny; i++)
    {
        for (j = 0; j < wiersze; j++)
        {
            if (brick[i][j].istnieje == true)
            {
                al_draw_bitmap(red_brick, brick[kolumny][wiersze].x, brick[kolumny][wiersze].y, 0);
            }
        }
    }
}/// Main 
/**
*G³ówna funckja programu
*  
*/
    int main()
    {
        bool done = false;
        al_init();
        
   
        al_install_keyboard(); //instalacja klawiatury
        ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

        ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
        ALLEGRO_SAMPLE* sample = NULL;
        ALLEGRO_SAMPLE* soundtrack = NULL;
        ALLEGRO_SAMPLE* hit = NULL;
       
        al_install_mouse(); //instalacja myszy
        al_init_font_addon(); //instalacja czcionki
        al_init_image_addon(); //dodawanie bitmap
        al_init_primitives_addon();
        al_install_audio();
        al_init_acodec_addon();
        al_init_ttf_addon();

        // al_reserve_sampler(1);
        soundtrack = al_load_sample("soundtrack.mp3");
        sample = al_load_sample("punkt.mp3");
        hit = al_load_sample("hit.mp3");
        ALLEGRO_KEYBOARD_STATE klawiatura;
        ALLEGRO_DISPLAY* okno = al_create_display(szer, wys); //tworzenie okna
    
      
        al_set_window_title(okno, "ARKANOID PROJEKT"); //tytul projektu
        //ALLEGRO_BITMAP* deska = al_create_bitmap(deska_x, deska_y); //stworzenie platformy do odbijania
        ALLEGRO_BITMAP* paddle = al_load_bitmap("deska3.png");
        ALLEGRO_BITMAP* red_brick = al_load_bitmap("red.png");
        ALLEGRO_BITMAP* green_brick = al_load_bitmap("zielonyklocek.png");
        ALLEGRO_BITMAP* yellow_brick = al_load_bitmap("zoltyklocek.png");
        
        
        int paddle_width = al_get_bitmap_width(paddle);
        int paddle_height = al_get_bitmap_height(paddle);
        int brick_width = al_get_bitmap_width(red_brick);
        int brick_height = al_get_bitmap_height(red_brick);
        int brick_x = 300; //szerokosc 
        int brick_y = 100; //wysokosc
        int points = 0;
        int destroyed_blocks = 0;
        int i, j;
        //==============
        //STAN POCZATKOWY
        //===============
        int STAN = MENU;
        
        //DZIALAJACE RENDEROWANIE MIEJSCA NA BLOKI
      //  for (i = 0; i < kolumny; i++)
      //  {
       //     for (j = 0; j < wiersze; j++)
         //   {
         //       brick[i][j].x = 30 + (i * 105);
         //       brick[i][j].y = 50 + (j * 55);
         //       brick[i][j].istnieje = true;
         //       brick[i][j].szer = al_get_bitmap_width(red_brick);
          //      brick[i][j].wys = al_get_bitmap_height(red_brick);
         //   }
      //  } //uzyte w innym miejscu
        ALLEGRO_BITMAP* pilka1 = al_load_bitmap("pilka_kosmiczna.png"); //wczytanie pilki
        ALLEGRO_BITMAP* pilka2 = al_load_bitmap("pilka1.png");
        ALLEGRO_FONT* font8 = al_create_builtin_font(); //czcionka
        ALLEGRO_FONT* font = al_load_font("Arka_solid.ttf", 90, 0);
        
       
       // al_set_target_bitmap(deska);
        al_clear_to_color(al_map_rgb(200, 0, 0));
        al_set_target_bitmap(al_get_backbuffer(okno));

        al_register_event_source(queue, al_get_keyboard_event_source());
        al_register_event_source(queue, al_get_display_event_source(okno));
        al_register_event_source(queue, al_get_timer_event_source(timer));
        int paddle_x = 320, paddle_y = 520; // umiejscowienie deski
        al_get_keyboard_state(&klawiatura); //odczyt z klawiatury
        ALLEGRO_EVENT event;
        //double czas = al_get_time();
        al_start_timer(timer);
        int exist = 1; //do niszczenia cegiel
       // ball.x = szer / 2;
       // ball.y = 300;
       // ball.dx = 4;
       // ball.dy = 4;
       // ball.lives = 3; //uzyte nizej
        int POZIOM1=0;
       

        while (!al_key_down(&klawiatura, ALLEGRO_KEY_ESCAPE))
        {   
            al_wait_for_event(queue, &event);
            al_get_keyboard_state(&klawiatura);
          

            if (STAN == MENU)
            {   
                int paddle_x = 320, paddle_y = 520; //komentarz wyzej
                int points = 0;
                int destroyed_blocks = 0;
                ball.x = szer / 2;
                ball.y = 300;
                ball.dx = 4;
                ball.dy = 4;
                ball.lives = 3;
                if (al_key_down(&klawiatura, ALLEGRO_KEY_ENTER))
                {
                    for (i = 0; i < kolumny; i++) //render miejsca dla cegiel
                    {
                        for (j = 0; j < wiersze; j++)
                        {
                            brick[i][j].x = 30 + (i * 105);
                            brick[i][j].y = 50 + (j * 55);
                            brick[i][j].istnieje = true;
                            brick[i][j].szer = al_get_bitmap_width(red_brick);
                            brick[i][j].wys = al_get_bitmap_height(red_brick);
                        }
                    }
                    STAN = LEVEL1;
                   
                }
            }
            if (STAN == RESET)
            {
                paddle_x = 320, paddle_y = 520;
                points = 0;
                destroyed_blocks = 0;
                ball.x = szer / 2;
                ball.y = 300;
                ball.dx = 4;
                ball.dy = 4;
                ball.lives = 3;
                    for (i = 0; i < kolumny; i++)
                    {
                        for (j = 0; j < wiersze; j++)
                        {
                            brick[i][j].x = 30 + (i * 105);
                            brick[i][j].y = 50 + (j * 55);
                            brick[i][j].istnieje = true;
                            brick[i][j].szer = al_get_bitmap_width(red_brick);
                            brick[i][j].wys = al_get_bitmap_height(red_brick);
                        }
                    }
                    STAN = LEVEL1;

            }

            else if (STAN == LEVEL1)
            {   
              
                if (al_key_down(&klawiatura, ALLEGRO_KEY_TAB))
                {
                    STAN = MENU;
                }

                else if (al_key_down(&klawiatura, ALLEGRO_KEY_R))
                {
                    STAN = RESET;
                }
                else if (al_key_down(&klawiatura, ALLEGRO_KEY_J))
                {
                    STAN = LEVEL2;
                }

            }
            else if (STAN == LEVEL2)
            {

                if (al_key_down(&klawiatura, ALLEGRO_KEY_TAB))
                {
                    STAN = MENU;
                }

                if (al_key_down(&klawiatura, ALLEGRO_KEY_R))
                {
                    STAN = RESET;
                }

            }

            if (event.type == ALLEGRO_EVENT_TIMER)
            {

                //=====
                //MENU
                //====
                if (STAN == MENU)
                {
                    
                    al_reserve_samples(1);
                    al_play_sample(soundtrack, 0.3, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
            
                    al_clear_to_color(al_map_rgb_f(0, 0, 205)); //t³o
                    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "ARKANOID");
                    al_draw_text(font8, al_map_rgb(0, 255, 0), (szer / 2) - 100, 325, 0, "WCISNIJ ENTER ABY ZAGRAC!");  
                    al_draw_text(font8, al_map_rgb(255, 255, 255), 400, 390, ALLEGRO_ALIGN_CENTRE, "==========");
                    al_draw_text(font8, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTRE, "STEROWANIE");
                    al_draw_text(font8, al_map_rgb(255, 255, 255), 400, 410, ALLEGRO_ALIGN_CENTRE, "==========");
                    al_draw_text(font8, al_map_rgb(0, 255, 0), 400, 425, ALLEGRO_ALIGN_CENTRE, "WCISNIJ [LEWA STRZALKA] ABY PORUSZYC PLATFORMA W LEWO");
                    al_draw_text(font8, al_map_rgb(0, 255, 0), 400, 450, ALLEGRO_ALIGN_CENTRE, "WCISNIJ [PRAWA STRZALKA] ABY PORUSZYC PLATFORMA W PRAWO");
                    al_draw_text(font8, al_map_rgb(0, 255, 0), 400, 475, ALLEGRO_ALIGN_CENTRE, "WCISNIJ ESC ABY WYJSC Z PROGRAMU");
                    al_draw_text(font8, al_map_rgb(0, 255, 0), 400, 525, ALLEGRO_ALIGN_CENTRE, "WCISNIJ R ABY ZRESTARTOWAC PROGRAM");
                    al_draw_text(font8, al_map_rgb(0, 255, 0), 400, 500, ALLEGRO_ALIGN_CENTRE, "WCISNIJ TAB ABY WROCIC DO MENU GLOWNEGO");
                    
                    al_flip_display();
                    al_rest(0.001);
                    

                    
                    
                }

                else if (STAN == GAMEOVER)
                {
                    al_draw_text(font8, al_map_rgb(0, 255, 0), 400, 400, 0, "PRZEGRALES!");
                    al_draw_text(font8, al_map_rgb(0, 255, 0), 500, 400, 0, "WCISNIJ TAB ABY WROCIC DO MENU!");
                }

                //========
                //LEVEL 1
                //========
                else if (STAN == LEVEL1)
                {   
                    al_reserve_samples(16);
                   
                    if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && paddle_x < szer - paddle_width) paddle_x += 8;
                    if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && paddle_x > 0) paddle_x -= 8;

                    if (ball.lives == 0) //przegrana
                    {
                         al_draw_text(font8, al_map_rgb(0, 255, 0), 400, 400, 0, "Przegrales!");
                        ball.x = 250;
                        ball.y = 300;


                    }
                    else if (destroyed_blocks == 21)
                    {
                        ball.x = 250;
                        ball.y = 300;
                        POZIOM1 == 1;
                    }
                    else
                        ball.x += ball.dx;
                    ball.y += ball.dy;


                    int paddlecollision = paddle_collsion(paddle_x, paddle_y, szer, wys, ball.y, ball.x, paddle_width, paddle_height);
                    if (paddlecollision == center)
                    {
                        al_play_sample(hit, 1, 0, 5, ALLEGRO_PLAYMODE_ONCE, NULL);
                        
                        ball.dy *= -1;
                    }
                        


                    if (ball.x <= 0) //lewa strona planszy
                    {
                        ball.dx *= -1;
                    }
                    else if (ball.x >= szer-20) //prawa strona planszy
                    {
                        ball.dx *= -1;
                    }
                    else if (ball.y <= 10) // gorna strona planszy 
                    {
                        ball.dy *= -1;
                    }
                    else if (ball.y >= wys)
                    {
                        ball.lives--;
                        ball.x = szer / 2;
                        ball.dx *= -1;
                        ball.y = 300; //dolna strona planszy
                    }

                    //Cegla
                    /// Renderowanie Bloków
                    /*
                    */

                    //=============
                    //RENDER BLOKOW
                    //=============
                    for (i = 0; i <= kolumny; i++)
                    {
                        for (j = 0; j < wiersze; j++)
                        {
                            if (brick[i][j].istnieje == 1)
                            {
                                int brickcollision = brick_collision(brick[i][j].x, brick[i][j].y, brick[i][j].szer, brick[i][j].wys, szer, wys, ball.y, ball.x);
                                if (brickcollision == true)
                                {
                                    al_play_sample(sample, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                                    
                                    ball.dy *= -1;
                                    brick[i][j].istnieje = false;
                                    points += 10;
                                    destroyed_blocks++;

                                }

                            }
                        }
                    }




                    al_clear_to_color(al_map_rgb_f(200, 0.5, 0.5)); //tlo planszy


                    al_draw_bitmap(paddle, paddle_x, paddle_y, 1);

                    {

                        for (i = 0; i <= kolumny; i++)
                        {
                            for (j = 0; j <= wiersze; j++)
                                if (brick[i][j].istnieje == 1)
                                {
                                    al_draw_bitmap(red_brick, brick[i][j].x, brick[i][j].y, 1);
                                }
                        }
                    }

                    
                    al_draw_bitmap(pilka1, ball.x, ball.y, 0);
                    al_draw_textf(font8, al_map_rgb(255, 255, 0), 10, 10, 0, "LEVEL 1 Zycia=%3d ,Punkty=%3d ", ball.lives, points); //tekst sluzacy do okreslania gdzie znajudje sie deska - tymczasowy
                    if (ball.lives == 0)
                    {
                        al_draw_textf(font8, al_map_rgb(200, 0, 0), 350, 300, 0, "PRZEGRALES!");
                        al_draw_textf(font8, al_map_rgb(0, 200, 0), 300, 320, 0, "Twoj wynik to: %3d", points);

                    }
                    else if (destroyed_blocks == 21)
                    {   
                        
                        al_draw_textf(font8, al_map_rgb(0, 200, 0), 350, 300, 0, "LEVEL 1 - WYGRALES!");
                        al_draw_textf(font8, al_map_rgb(0, 200, 0), 300, 320, 0, "Twoj wynik to: %3d", points);
                        al_draw_textf(font8, al_map_rgb(0, 200, 0), 300, 340, 0, "Wcisnij Enter aby przejsc do nastepnego poziomu");

                        POZIOM1 == 1;

                    }
                }

                //=======
                // LEVEL2
                //=======


                else if (STAN == LEVEL2)
                {
                al_reserve_samples(16);

                if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && paddle_x < szer - paddle_width) paddle_x += 8;
                if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && paddle_x > 0) paddle_x -= 8;

                if (ball.lives == 0) //przegrana
                {
                    // al_draw_text(font8, al_map_rgb(0, 255, 0), 400, 400, 0, "Przegrales!");
                    ball.x = 250;
                    ball.y = 300;


                }
                else if (destroyed_blocks == 21)
                {
                    ball.x = 250;
                    ball.y = 300;
                }
                else
                    ball.x += ball.dx;
                ball.y += ball.dy;


                int paddlecollision = paddle_collsion(paddle_x, paddle_y, szer, wys, ball.y, ball.x, paddle_width, paddle_height);
                if (paddlecollision == center)
                {
                    al_play_sample(hit, 1, 0, 5, ALLEGRO_PLAYMODE_ONCE, NULL);

                    ball.dy *= -1;
                }



                if (ball.x <= 0) //lewa strona planszy
                {
                    ball.dx *= -1;
                }
                else if (ball.x >= szer - 20) //prawa strona planszy
                {
                    ball.dx *= -1;
                }
                else if (ball.y <= 10) // gorna strona planszy 
                {
                    ball.dy *= -1;
                }
                else if (ball.y >= wys)
                {
                    ball.lives--;
                    ball.x = szer / 2;
                    ball.dx *= -1;
                    ball.y = 300; //dolna strona planszy
                }

                //Cegla


                //=============
                //RENDER BLOKOW
                //=============
                for (i = 0; i <= kolumny; i++)
                {
                    for (j = 0; j < wiersze; j++)
                    {
                        if (brick[i][j].istnieje == 1)
                        {
                            int brickcollision = brick_collision(brick[i][j].x, brick[i][j].y, brick[i][j].szer, brick[i][j].wys, szer, wys, ball.y, ball.x);
                            if (brickcollision == true)
                            {
                                al_play_sample(sample, 0.3, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);                                
                                ball.dy *= -1;
                                brick[i][j].istnieje = false;
                                points += 10;
                                destroyed_blocks++;

                            }

                        }
                    }
                }



                
                al_clear_to_color(al_map_rgb_f(100, 0.5, 0.5)); //tlo planszy


                al_draw_bitmap(paddle, paddle_x, paddle_y, 1);

                {

                    for (i = 0; i <= kolumny; i++)
                    {
                        for (j = 0; j <= wiersze; j++)
                            if (brick[i][j].istnieje == 1)
                            {
                                if (j % 2 == 0)
                                {
                                    al_draw_bitmap(yellow_brick, brick[i][j].x, brick[i][j].y, 1);
                                }
                                else
                                al_draw_bitmap(green_brick, brick[i][j].x, brick[i][j].y, 1);
                            }
                    }
                }


                al_draw_bitmap(pilka2, ball.x, ball.y, 0);
                al_draw_textf(font8, al_map_rgb(255, 255, 0), 10, 10, 0, "Level 2 ,Zycia=%3d ,Punkty=%3d ", ball.lives, points);//tekst sluzacy do okreslania gdzie znajudje sie deska - tymczasowy
                
                if (ball.lives == 0)
                {
                    al_draw_textf(font8, al_map_rgb(200, 0, 0), 350, 300, 0, "PRZEGRALES!");
                    al_draw_textf(font8, al_map_rgb(0, 200, 0), 300, 320, 0, "Twoj wynik to: %3d", points);

                }
                else if (destroyed_blocks == 21)
                {
                    al_draw_textf(font8, al_map_rgb(0, 200, 0), 350, 300, 0, "Poziom 2 - WYGRALES!");
                    al_draw_textf(font8, al_map_rgb(0, 200, 0), 300, 320, 0, "Twoj wynik to: %3d", points);

                }
                }

             
                al_flip_display();
                al_rest(0.001);
            }
          
        }

      

        // al_destroy_bitmap(deska);// czyszczenie pamieci
     
        //
        al_destroy_bitmap(pilka1);
        al_destroy_bitmap(paddle);
        al_destroy_bitmap(red_brick);
        al_destroy_sample(sample);//
        al_destroy_sample(hit);
        al_destroy_sample(soundtrack);
        al_destroy_bitmap(green_brick);

        return 0;

    }