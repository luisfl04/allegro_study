#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>

// Configurações básicas de inicialização da tela:
static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_FONT *fonte_geral = NULL;
static ALLEGRO_FONT *fonte_mensagem_boas_vindas = NULL;
static ALLEGRO_SAMPLE *menu_de_musica = NULL;
static ALLEGRO_BITMAP *imagem_fundo = NULL;
static ALLEGRO_BITMAP *imagem_nave = NULL;
static bool musica_tocando = true;
static int largura_janela = 1366;
static int altura_janela = 768;

static void desenharBotao(float x, float y, float largura, float altura, const char *texto, ALLEGRO_COLOR cor_fundo, ALLEGRO_COLOR cor_texto) {
    /* Função responsável por renderizar os botões usados no menu inicial. Ela pode ser usada em outras implementações, visto que desenha os botões
    de acordo com o parâmetros passados. */
    
    // Desenha os botões:
    al_draw_filled_rectangle(x, y, x + largura, y + altura, cor_fundo);
    float texto_x = x + (largura - al_get_text_width(fonte_geral, texto)) / 2;
    float texto_y = y + (altura - al_get_font_line_height(fonte_geral)) / 2;
    al_draw_text(fonte_geral, cor_texto, texto_x, texto_y, 0, texto);
}


static void carregarMenuInicial() {

    /* Essa função é responsável por carregar o menu inicial do jogo que representa também o menu de pause. O usuário irá usá-la sempre que 
    o game for pausado ou quando o game for iniciado pela primeira vez */

    // Inicializando allegro:
    if (!al_init()) {
        al_show_native_message_box(NULL, "Erro", "Não foi possível inicializar o Allegro", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }

    // Cria a janela com o tamanho especificado:
    display = al_create_display(largura_janela, altura_janela);
    if (!display) {
        al_show_native_message_box(NULL, "Erro", "Não foi possível criar a janela", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return;
    }
    al_set_window_title(display, "Menu inicial"); // Definindo titulo da janela

    // Inicializa addons necessários:
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_init_image_addon();

    // Configuração de eventos:
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    al_install_mouse();
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());

    // Carregando fonte geral:
    fonte_geral = al_load_ttf_font("./fonts/roboto/Roboto-Regular.ttf", 48, 0);
    if (!fonte_geral) {
        al_show_native_message_box(NULL, "Erro", "Não foi possível carregar a fonte geral", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        return;
    }

    // Carregando música:
    al_reserve_samples(1);
    menu_de_musica = al_load_sample("./audio/menu_music.wav");
    if (!menu_de_musica) {
        al_show_native_message_box(NULL, "Erro", "Não foi possível carregar a música do menu", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    } else {
        // Toca a música em loop:
        al_play_sample(menu_de_musica, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
        musica_tocando = true;
    }

    // Prepara o fundo e carrega a imagem com estrelas sobre o fundo:
    al_clear_to_color(al_map_rgb(0, 0, 0));
    imagem_fundo = al_load_bitmap("./imagens/menu_inicial/fundo_estrelas_menu_inicial.png");
    if (!imagem_fundo) {
        al_show_native_message_box(NULL, "Erro", "Não foi possível carregar a imagem de fundo", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    } else {
        // Desenha a imagem na tela
        al_draw_bitmap(imagem_fundo, 0, 0, 0);
    }

    // Configurando os botões e os desenhando na tela:
    float largura_botao = 380;
    float altura_botao = 90;
    float x_botoes = largura_janela / 2 - largura_botao / 2;
    float y_botao_iniciar = altura_janela / 2 - altura_botao - 10;
    float y_botao_fechar = altura_janela / 2 + 10;
    desenharBotao(x_botoes, y_botao_iniciar, largura_botao, altura_botao, "Iniciar jogo", al_map_rgb(50, 150, 50), al_map_rgb(255, 255, 255));
    desenharBotao(x_botoes, y_botao_fechar, largura_botao, altura_botao, "Fechar", al_map_rgb(150, 50, 50), al_map_rgb(255, 255, 255));

    // Desenhando mensagem de boas vindas:
        
        // Carregando a imagem da nave que é renderizada ao lado da mensagem de boas vindas:
        imagem_nave = al_load_bitmap("./imagens/menu_inicial/destroyer.png");
        if (!imagem_nave) {
            al_show_native_message_box(NULL, "Erro", "Não foi possível carregar a imagem da nave", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }

        // Carregando a fonte:
        fonte_mensagem_boas_vindas = al_load_ttf_font("./fonts/roboto/Roboto-Bold.ttf", 55, 0);
        if (!fonte_mensagem_boas_vindas) {
            al_show_native_message_box(NULL, "Erro", "Não foi possível carregar a fonte de boas vindas", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_display(display);
            return;
        }

        // Configurações da mensagem de boas vindas:
        const char *mensagem = "Bem-vindo ao Space Game!";
        float largura_mensagem = al_get_text_width(fonte_mensagem_boas_vindas, mensagem);
        float altura_mensagem = al_get_font_line_height(fonte_mensagem_boas_vindas);
        float x_mensagem = largura_janela / 4;
        float y_mensagem = altura_janela / 4;
        
        // Desenhando a mensagem:
        al_draw_text(fonte_mensagem_boas_vindas, al_map_rgb(255, 255, 255), x_mensagem , y_mensagem - 70, 0, mensagem);

        // Desenha a imagem da nave ao lado do texto:
        if (imagem_nave) {
            float largura_nave = al_get_bitmap_width(imagem_nave);
            float altura_nave = al_get_bitmap_height(imagem_nave);
            al_draw_bitmap(imagem_nave, x_mensagem + largura_mensagem + 20, y_mensagem - altura_nave / 4, 0);
        }

    // Atualiza a tela:
    al_flip_display();

    // Loop principal do menu que espera por uma ação do usuário, seja ela iniciar o game, fechá-lo ou desativar e ativar música:
    bool sair = false;
    while (!sair) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            sair = true;
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (evento.mouse.x >= x_botoes && evento.mouse.x <= x_botoes + largura_botao) {
                if (evento.mouse.y >= y_botao_fechar && evento.mouse.y <= y_botao_fechar + altura_botao) {
                    sair = true; // Fechar programa
                }
            }
        }
    }

    // Ao fechar loop, libero os recursos usados:
    if (menu_de_musica) {
        al_destroy_sample(menu_de_musica);
    }
    if (imagem_fundo) {
        al_destroy_bitmap(imagem_fundo);
    }
    if (fonte_geral) {
        al_destroy_font(fonte_geral);
    }
    if (display) {
        al_destroy_display(display);
    }
    al_destroy_event_queue(fila_eventos);
}
