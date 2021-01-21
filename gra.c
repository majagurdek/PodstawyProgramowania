#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>

// Globalne kolory.
//----------------------------------------------------------------------------------------------------------------------
ALLEGRO_COLOR ball_color;
ALLEGRO_COLOR wall_color;
ALLEGRO_COLOR path_color;
ALLEGRO_COLOR start_color;
ALLEGRO_COLOR finish_color;

//----------------------------------------------------------------------------------------------------------------------
struct GameMap
{
    int cols;
    int rows;
    int *cells;
};

typedef struct
{
    int x;
    int y;
} Point;

//----------------------------------------------------------------------------------------------------------------------
struct GameMap load_map(const char *filename)
{
    FILE *in = fopen(filename, "r");

    if(in == NULL) printf("BLAD"); //exit(1);

    struct GameMap map;
    int status = fscanf(in, "%d%d", &map.rows, &map.cols);
    if (status != 2) exit (1);

    int n = map.rows * map.cols;
    map.cells = malloc(n * sizeof(int));

    for(int i=0; i<n; i++)
    {
        int status = fscanf(in, "%d", &map.cells[i]);
        if (status != 1) exit(1);
    }

    fclose(in);

    return map;
}

void destroy_map(struct GameMap *map)
{
    free(map->cells);
    map->rows = 0;
    map->cols = 0;
    map->cells = NULL;
}

//----------------------------------------------------------------------------------------------------------------------
void show_map(struct GameMap *map, double cell_width, double cell_height)
{
    ALLEGRO_COLOR chosen_color = wall_color;

    for(int i=0; i<map->rows;i++)
    {
        for(int j=0; j<map->cols; j++)
        {
            int index = i * map->cols + j;

            switch(map->cells[index])
            {
                case 0:	// Sciezka
                    chosen_color = path_color;
                    break;
                case 1: // Sciana
                    chosen_color = wall_color;
                    break;
                case 2: // Start
                    chosen_color = start_color;
                    break;
                case 3: // Finisz
                    chosen_color = finish_color;
                    break;
            }
            al_draw_filled_rectangle(j * cell_width, i * cell_height, (j+1) * cell_width, (i+1) * cell_height, chosen_color);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
Point find_point(struct GameMap *map, int z)
{
    Point find = {0, 0};
    for(int i = 0; i<map->rows;i++)
    {
        for (int j = 0; j < map->cols; j++)
        {
            int index = i * map->cols + j;
            if (map->cells[index] == z)
            {
                find.x = j;
                find.y = i;
                goto loop_end;
            }
        }
    }
    loop_end:
    
    return find;
}


int main(int argc, char **argv) {
    //------------------------------------------------------------------------------------------------------------------
    // INICJALIZACJA I USTAWIENIA.
    //------------------------------------------------------------------------------------------------------------------

    al_init();               		
    al_install_keyboard();			
    al_install_mouse();       		
	al_init_primitives_addon();

	// Przypisywanie kolorów.
	ball_color = al_map_rgb(255, 0, 0);
	wall_color = al_map_rgb(100, 100, 100);
	path_color = al_map_rgb(200, 200, 200);
	start_color = al_map_rgb(255, 200, 200);
	finish_color = al_map_rgb(200, 255, 200);


    //=======================================================
    // Wczytywanie mapy i tworzenie dostatecznie dużego okna.
    //=======================================================
    struct GameMap map = load_map("mapa.txt");
 
    double cell_width = 40;
    double cell_height = cell_width;
    
    ALLEGRO_DISPLAY *display = al_create_display(cell_width * map.cols, cell_height * map.rows);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Gra");                           
    
    // Znajdujemy pozycję początku i końca.
    Point start = find_point(&map, 2);
    Point finish = find_point(&map, 3);
    
	show_map(&map, cell_width, cell_height);
	
	// Ustawianie pozycji kulki.
	double x = start.x * cell_width + cell_height / 2;
	double y = start.y * cell_height + cell_height / 2;
    
    al_draw_filled_circle(x, y, cell_width / 4, ball_color);
    
    al_flip_display();
    
    //=======================================================
    // Kolejka komunikatów i rejestracja źródeł komunikatów.
    //=======================================================

    ALLEGRO_TIMER *timer = al_create_timer(20e-3);                   

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();                   
    ALLEGRO_EVENT_SOURCE *disp_esource = al_get_display_event_source(display);    
    ALLEGRO_EVENT_SOURCE *kbd_esource = al_get_keyboard_event_source();            
    ALLEGRO_EVENT_SOURCE *timer_esource = al_get_timer_event_source(timer);        
	

    // Rejestrowanie źródeł zdarzeń.
    al_register_event_source(event_queue, disp_esource);
    al_register_event_source(event_queue, kbd_esource);
    al_register_event_source(event_queue, timer_esource);

    // Informacje które klawisze są naciśnięte.
    int w_pressed = 0;
    int s_pressed = 0;
    int a_pressed = 0;
    int d_pressed = 0;
    
    //=======================================================
    // Główna część programu + główna pętla komunikatów.
    //=======================================================

    // Włączamy timer
    al_start_timer(timer);

    // Pętla zdarzeń.
    int run = 1;
    do {
        ALLEGRO_EVENT event;                        
        al_wait_for_event(event_queue, &event);       

        // Przetwórz odebrane zdarzenie.
        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:        
                run = 0;
                break;
            case ALLEGRO_EVENT_TIMER:                

                al_clear_to_color(al_map_rgb(255, 255, 255)); 
                show_map(&map, cell_width, cell_height);

                // Reagujemy na naciśnięte klawisze.
                if (w_pressed == 1) y -= 2;
                if (s_pressed == 1) y += 2;
                if (a_pressed == 1) x -= 2;
                if (d_pressed == 1) x += 2;

                al_draw_filled_circle(x, y, 10, ball_color);
                al_flip_display();

                //wchodzenie na ścianę, wygrana
                for(int i=0; i<map.rows; i++)
                {
                    for (int j = 0; j < map.cols; j++)
                    {
                        int index = i * map.cols + j;
                        if (map.cells[index] == 1)
                        {
                            if (x > j * cell_width && x < (j+1) * cell_width && y > i * cell_height && y < (i+1) * cell_height)
                            {
                                printf("NIESTETY PRZEGRAŁEŚ :(\n");
                                run = 0;
                            }
                        }
                        if (map.cells[index] == 3)
                        {
                            if (x > j * cell_width && x < (j+1) * cell_width && y > i * cell_height && y < (i+1) * cell_height)
                            {
                                printf("GRATULACJE, WYGRAŁEŚ! :)\n");
                                run = 0;
                            }
                        }
                    }
                }
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_W:
                        w_pressed = 1;
                        break;
                    case ALLEGRO_KEY_S:
                        s_pressed = 1;
                        break;
                    case ALLEGRO_KEY_A:
                        a_pressed = 1;
                        break;
                    case ALLEGRO_KEY_D:
                        d_pressed = 1;
                        break;
                }

                break;
            case ALLEGRO_EVENT_KEY_UP:
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_W:
                        w_pressed = 0;
                        break;
                    case ALLEGRO_KEY_S:
                        s_pressed = 0;
                        break;
                    case ALLEGRO_KEY_A:
                        a_pressed = 0;
                        break;
                    case ALLEGRO_KEY_D:
                        d_pressed = 0;
                        break;
                }
                break;
        }

    } while (run);

    //------------------------------------------------------------------------------------------------------------------
    // SPRZĄTANIE PO PROGRAMIE
    //------------------------------------------------------------------------------------------------------------------

    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);                          
    al_destroy_display(display);                                   
    destroy_map(&map);
    return 0;
}
