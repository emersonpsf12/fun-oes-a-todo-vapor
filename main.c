
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

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

// Folha de caderno: papel, linhas azuis, margem vermelha e furos
void desenhaCaderno()
{
    int y;

    al_clear_to_color(al_map_rgb(250, 247, 232));

    for (y = 40; y < ALTURA; y += 28)
    {
        al_draw_line(0, y, LARGURA, y, al_map_rgb(175, 205, 235), 1);
    }

    al_draw_line(90, 0, 90, ALTURA, al_map_rgb(230, 120, 120), 2);

    for (y = 50; y < ALTURA; y += 100)
    {
        al_draw_filled_circle(40, y, 10, al_map_rgb(70, 70, 80));
    }
}

// Rabiscos de funcoes no fundo, como se fossem feitos a lapis
void desenhaRabiscos(ALLEGRO_FONT *fonte)
{
    ALLEGRO_COLOR lapis = al_map_rgb(185, 190, 205);
    float x;

    desenhaTexto(fonte, lapis, 230, 50, 2, "f(x) = 2x + 1");
    desenhaTexto(fonte, lapis, 620, 75, 2, "y = x² - 4");
    desenhaTexto(fonte, lapis, 210, 345, 2, "f(x) = -x + 3");
    desenhaTexto(fonte, lapis, 650, 330, 2, "y = ax + b");
    desenhaTexto(fonte, lapis, 400, 500, 2, "f(0) = 1");

    // grafico de uma reta (y = x), no canto de baixo da esquerda
    al_draw_line(130, 500, 290, 500, lapis, 2);
    al_draw_line(210, 420, 210, 580, lapis, 2);
    al_draw_line(150, 560, 270, 440, lapis, 2);

    // grafico de uma parabola (y = x * x), no canto de baixo da direita
    al_draw_line(560, 560, 740, 560, lapis, 2);
    al_draw_line(650, 430, 650, 580, lapis, 2);

    for (x = -3; x < 3; x += 0.25)
    {
        float x2 = x + 0.25;

        // cada unidade vale 25 pixels na largura e 12 na altura
        al_draw_line(650 + x * 25, 560 - x * x * 12,
            650 + x2 * 25, 560 - x2 * x2 * 12, lapis, 2);
    }
}

void desenhaTelaInicial(ALLEGRO_FONT *fonte)
{
    ALLEGRO_COLOR azul = al_map_rgb(15, 35, 85);
    ALLEGRO_COLOR amarelo = al_map_rgb(255, 210, 40);

    desenhaCaderno();
    desenhaRabiscos(fonte);

    // titulo com uma faixa amarela, igual marca texto
    al_draw_filled_rectangle(170, 125, 630, 195, amarelo);
    desenhaTexto(fonte, azul, 400, 132, 7, "FUNÇÕES");
    desenhaTexto(fonte, azul, 400, 215, 5, "A TODO VAPOR");

    desenhaTexto(fonte, azul, 400, 285, 2, "Monte o gráfico da função!");

    // botao jogar
    al_draw_filled_rounded_rectangle(290, 385, 510, 445, 10, 10, azul);
    desenhaTexto(fonte, amarelo, 400, 403, 3, "JOGAR");

    desenhaTexto(fonte, azul, 400, 462, 1.5, "Aperte ENTER para começar");

    desenhaTexto(fonte, azul, 400, 585, 1, "Projeto Integrador - BCC");
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
    al_install_keyboard();

    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA, ALTURA);
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT *fonte = al_create_builtin_font();

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
                desenhaTelaInicial(fonte);
            else
                desenhaJogo(fonte);

            al_flip_display();
        }
    }

    al_destroy_font(fonte);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(janela);

    return 0;
}
