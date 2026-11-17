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

    // deixa as imagens lisas quando o jogo e esticado na tela cheia
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA, ALTURA);
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT *fonte = al_create_builtin_font();

    // o jogo e desenhado nessa folha de 800x600 e depois
    // a folha e esticada para o tamanho da janela
    ALLEGRO_BITMAP *folha = al_create_bitmap(LARGURA, ALTURA);

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
    int telaCheia = 0;

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

            // F11 liga e desliga a tela cheia
            if (evento.keyboard.keycode == ALLEGRO_KEY_F11)
            {
                telaCheia = !telaCheia;
                al_set_display_flag(janela, ALLEGRO_FULLSCREEN_WINDOW, telaCheia);
            }

            // ENTER na tela inicial comeca o jogo
            if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER && tela == TELA_INICIAL)
                tela = TELA_JOGO;
        }

        if (evento.type == ALLEGRO_EVENT_TIMER)
        {
            // 1) desenha o jogo na folha de 800x600
            al_set_target_bitmap(folha);

            if (tela == TELA_INICIAL)
                desenhaTelaInicial(capa);
            else
                desenhaJogo(fonte);

            // 2) estica a folha para caber na janela sem deformar
            al_set_target_backbuffer(janela);
            al_clear_to_color(al_map_rgb(0, 0, 0));

            float larguraJanela = al_get_display_width(janela);
            float alturaJanela = al_get_display_height(janela);

            float escala = larguraJanela / LARGURA;

            if (alturaJanela / ALTURA < escala)
                escala = alturaJanela / ALTURA;

            // o que sobra vira faixa preta dos dois lados
            float sobraX = (larguraJanela - LARGURA * escala) / 2;
            float sobraY = (alturaJanela - ALTURA * escala) / 2;

            al_draw_scaled_bitmap(folha, 0, 0, LARGURA, ALTURA,
                sobraX, sobraY, LARGURA * escala, ALTURA * escala, 0);

            al_flip_display();
        }
    }

    al_destroy_bitmap(folha);
    al_destroy_bitmap(capa);
    al_destroy_font(fonte);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(janela);

    return 0;
}
