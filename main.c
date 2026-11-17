#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

#define LARGURA 800
#define ALTURA 600

// telas do jogo
#define TELA_INICIAL 0
#define TELA_JOGO 1

// A fonte padrao do Allegro e bem pequena, entao essa funcao
// desenha o texto aumentado. O x e o centro do texto.
void desenhaTexto(ALLEGRO_FONT *fonte, ALLEGRO_COLOR cor,
    float x, float y, float escala, const char *texto)
{
    ALLEGRO_TRANSFORM t;

    al_identity_transform(&t);
    al_scale_transform(&t, escala, escala);
    al_translate_transform(&t, x, y);
    al_use_transform(&t);

    al_draw_text(fonte, cor, 0, 0, ALLEGRO_ALIGN_CENTER, texto);

    // volta o desenho para o tamanho normal
    al_identity_transform(&t);
    al_use_transform(&t);
}

// Tela inicial: a capa e uma imagem. O jogo comeca com ENTER.
void desenhaTelaInicial(ALLEGRO_BITMAP *capa)
{
    // desenha a imagem esticada para ocupar a janela inteira
    al_draw_scaled_bitmap(capa, 0, 0,
        al_get_bitmap_width(capa), al_get_bitmap_height(capa),
        0, 0, LARGURA, ALTURA, 0);
}

void desenhaJogo(ALLEGRO_FONT *fonte)
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
}

int main()
{
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA, ALTURA);
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT *fonte = al_create_builtin_font();

    // imagem da capa do jogo
    ALLEGRO_BITMAP *capa = al_load_bitmap("imagens/capa.png");

    if (capa == NULL)
    {
        printf("Erro: nao achei a imagem imagens/capa.png\n");
        return 1;
    }

    al_set_window_title(janela, "Funções a Todo Vapor");

    al_register_event_source(fila, al_get_display_event_source(janela));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());

    al_start_timer(timer);

    int rodando = 1;
    int tela = TELA_INICIAL;

    while (rodando)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            rodando = 0;

        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            // ESC fecha o jogo
            if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                rodando = 0;

            // ENTER na tela inicial comeca o jogo
            if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER && tela == TELA_INICIAL)
                tela = TELA_JOGO;
        }

        if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            if (tela == TELA_INICIAL)
                desenhaTelaInicial(capa);
            else
                desenhaJogo(fonte);

            al_flip_display();
        }
    }

    al_destroy_bitmap(capa);
    al_destroy_font(fonte);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(janela);

    return 0;
}
