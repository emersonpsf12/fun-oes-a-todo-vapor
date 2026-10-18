#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <stdio.h>
#include <math.h>

#define LARGURA 800
#define ALTURA 600
#define MARGEM 40

// limites do grafico
#define XMIN -10
#define XMAX 10
#define YMIN -10
#define YMAX 10

typedef struct {
    float x, y;
} Ponto;

float telax(float x) {
    return MARGEM + (x - XMIN) * (LARGURA - 2 * MARGEM) / (XMAX - XMIN);
}

float telay(float y) {
    return ALTURA - MARGEM - (y - YMIN) * (ALTURA - 2 * MARGEM) / (YMAX - YMIN);
}

float planox(float px) {
    return XMIN + (px - MARGEM) * (XMAX - XMIN) / (LARGURA - 2 * MARGEM);
}

float planoy(float py) {
    return YMIN + (ALTURA - MARGEM - py) * (YMAX - YMIN) / (ALTURA - 2 * MARGEM);
}

void desenha_grade() {
    int i;
    for (i = XMIN; i <= XMAX; i++) {
        al_draw_line(telax(i), telay(YMIN), telax(i), telay(YMAX), al_map_rgb(220, 220, 220), 1);
    }
    for (i = YMIN; i <= YMAX; i++) {
        al_draw_line(telax(XMIN), telay(i), telax(XMAX), telay(i), al_map_rgb(220, 220, 220), 1);
    }
}

void desenha_eixos(ALLEGRO_FONT *fonte) {
    int i;
    char txt[10];
    ALLEGRO_COLOR preto = al_map_rgb(0, 0, 0);

    // eixo x
    al_draw_line(telax(XMIN), telay(0), telax(XMAX) + 10, telay(0), preto, 2);
    al_draw_filled_triangle(telax(XMAX) + 15, telay(0), telax(XMAX) + 5, telay(0) - 5, telax(XMAX) + 5, telay(0) + 5, preto);
    al_draw_text(fonte, preto, telax(XMAX) + 15, telay(0) + 8, ALLEGRO_ALIGN_CENTER, "x");

    // eixo y
    al_draw_line(telax(0), telay(YMIN), telax(0), telay(YMAX) - 10, preto, 2);
    al_draw_filled_triangle(telax(0), telay(YMAX) - 15, telax(0) - 5, telay(YMAX) - 5, telax(0) + 5, telay(YMAX) - 5, preto);
    al_draw_text(fonte, preto, telax(0) - 12, telay(YMAX) - 18, ALLEGRO_ALIGN_CENTER, "y");

    for (i = XMIN; i <= XMAX; i++) {
        if (i == 0) continue;
        al_draw_line(telax(i), telay(0) - 4, telax(i), telay(0) + 4, preto, 1);
        sprintf(txt, "%d", i);
        al_draw_text(fonte, preto, telax(i), telay(0) + 8, ALLEGRO_ALIGN_CENTER, txt);
    }
    for (i = YMIN; i <= YMAX; i++) {
        if (i == 0) continue;
        al_draw_line(telax(0) - 4, telay(i), telax(0) + 4, telay(i), preto, 1);
        sprintf(txt, "%d", i);
        al_draw_text(fonte, preto, telax(0) - 8, telay(i) - 4, ALLEGRO_ALIGN_RIGHT, txt);
    }
    al_draw_text(fonte, preto, telax(0) - 8, telay(0) + 8, ALLEGRO_ALIGN_RIGHT, "0");
}

int main() {
    al_init();
    al_init_primitives_addon();
    al_init_font_addon();
    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_DISPLAY *janela = al_create_display(LARGURA, ALTURA);
    ALLEGRO_EVENT_QUEUE *fila = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT *fonte = al_create_builtin_font();

    al_register_event_source(fila, al_get_display_event_source(janela));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_mouse_event_source());

    Ponto pontos[100];
    int qtd = 0;
    int i;
    float mx = 0, my = 0;
    int rodando = 1;
    char txt[50];

    al_start_timer(timer);

    while (rodando) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(fila, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = 0;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                rodando = 0;
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            mx = ev.mouse.x;
            my = ev.mouse.y;
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (ev.mouse.button == 1 && qtd < 100) {
                pontos[qtd].x = round(planox(ev.mouse.x));
                pontos[qtd].y = round(planoy(ev.mouse.y));
                qtd++;
            }
            if (ev.mouse.button == 2 && qtd > 0) {
                qtd--;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(255, 255, 255));
            desenha_grade();
            desenha_eixos(fonte);

            for (i = 0; i < qtd; i++) {
                al_draw_filled_circle(telax(pontos[i].x), telay(pontos[i].y), 5, al_map_rgb(30, 90, 200));
                sprintf(txt, "(%.0f, %.0f)", pontos[i].x, pontos[i].y);
                al_draw_text(fonte, al_map_rgb(30, 90, 200), telax(pontos[i].x) + 8, telay(pontos[i].y) - 12, 0, txt);
            }

            sprintf(txt, "mouse: %.1f, %.1f", planox(mx), planoy(my));
            al_draw_text(fonte, al_map_rgb(100, 100, 100), 10, 10, 0, txt);

            al_flip_display();
        }
    }

    al_destroy_font(fonte);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila);
    al_destroy_display(janela);
    return 0;
}
