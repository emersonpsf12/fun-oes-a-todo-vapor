
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#define LARGURA 800
#define ALTURA 600

int main()
{
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();

    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA, ALTURA);
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT *fonte = al_create_builtin_font();

    al_register_event_source(fila, al_get_display_event_source(janela));
    al_register_event_source(fila, al_get_timer_event_source(timer));

    al_start_timer(timer);

    int rodando = 1;

    while (rodando)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            rodando = 0;

        if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            al_clear_to_color(al_map_rgb(20, 25, 45));

            // Quadro da funcao
            al_draw_filled_rectangle(250, 35, 550, 105,
                al_map_rgb(255, 210, 40));

            al_draw_text(fonte, al_map_rgb(15, 35, 85),
                400, 63, ALLEGRO_ALIGN_CENTER, "f(x) = 2x + 1");

            // Area do grafico
            al_draw_filled_rectangle(60, 145, 740, 555,
                al_map_rgb(15, 35, 85));

            al_draw_rectangle(60, 145, 740, 555,
                al_map_rgb(255, 210, 40), 4);

            // Botao confirmar
            al_draw_filled_rectangle(620, 565, 740, 595,
                al_map_rgb(255, 210, 40));

            al_draw_text(fonte, al_map_rgb(15, 35, 85),
                680, 575, ALLEGRO_ALIGN_CENTER, "CONFIRMAR");

            al_flip_display();
        }
    }

    al_destroy_font(fonte);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(janela);

    return 0;
}
