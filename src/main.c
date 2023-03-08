/*
Equipe 4 - 2020.2
- Edson José Araújo Pereira Júnior (ejapj)
- José Victor de Luna da Silva (jvls)
- Mário Sérgio Fernandes Mendonça (msfm)
- Nathalia Fernanda de Araújo Barbosa (nfab)
- Ricardo Ferreira da Cruz (rfc3)
*/

#include "raylib.h"

//Variáveis globais
char NOME_DO_JOGO[] = "Em Busca do Free";
const int screenWidth = 900, screenHeight = 700;
int gameState = 0, windowState = 0;

//Structs para fase dois
#define NUM_MAX_ENEMIES 2
#define NUM_SHOOTS 2

typedef struct Enemy
{
    Rectangle rec;
    Rectangle pos;
    Vector2 speed;
    bool active;
    bool type;
    Color color;
} Enemy;

typedef struct Shoot
{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Shoot;
//---------------------------------------------------------------------------------
int main(void)
{
    //Declarando as funções
    void menuHistoria(int *frameCounterHist);
    void menuPrincipal(int *frameCounterNome, int *frameCounterGrupo);
    void faseUm();
    void faseDois();
    //Declarando variáveis
    int frameCounterHist = 0, frameCounterNome = 0, frameCounterGrupo = 0;

    InitWindow(screenWidth, screenHeight, NOME_DO_JOGO);
    //Música da intro e do final
    InitAudioDevice();
    Music introSong = LoadMusicStream("../assets/musicas/Love_On_A_Real_Train.mp3");
    Music winSong = LoadMusicStream("../assets/musicas/vamo-jogar-free-fire.mp3");
    PlayMusicStream(introSong);
    SetMusicVolume(introSong, 0.1f);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (IsMusicPlaying(introSong))
            UpdateMusicStream(introSong);

        //inicia menu com introdução do jogo. ao fim do menu, o gameState vira 1
        if (gameState == 0)
            menuHistoria(&frameCounterHist);

        //inicia o menu principal. ao pressionar space, o gameState vira 2
        else if (gameState == 1)
            menuPrincipal(&frameCounterNome, &frameCounterGrupo);

        //inicia a intro da fase um. ao completar, o gameState vira 3
        else if (gameState == 2)
            menuHistoria(&frameCounterHist);

        //inicia a fase um do jogo. ao completar, o gameState vira 4
        else if (gameState == 3)
        {
            StopMusicStream(introSong);
            faseUm();
            windowState = 2;
        }

        //inicia a intro da fase dois. ao completar, o gameState vira 5
        else if (gameState == 4)
            menuHistoria(&frameCounterHist);

        //inicia a fase dois do jogo. ao completar, o gameState vira 6
        else if (gameState == 5)
        {
            faseDois();
            windowState = 4;
        }

        //final
        else if (gameState == 6)
        {
            if (!IsMusicPlaying(winSong))
            {
                PlayMusicStream(winSong);
                SetMusicVolume(winSong, 0.4f);
            }

            UpdateMusicStream(winSong);
            menuHistoria(&frameCounterHist);
        }

        else
            break;
    }

    UnloadMusicStream(introSong);
    UnloadMusicStream(winSong);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void menuHistoria(int *frameCounterHist)
{
    char historia[] = "É noite de domingo em Hellcife; no Sítio dos Pintos, o galo treina seu canto para as aulas de Matemática Discreta da quarta-feira.\nA vizinha da monitoria de I.P. compra ovos no carro do ovo, que não para de passar em sua rua, ao mesmo tempo em que um carro obstrui a passagem de uma ambulância, que ecoa sua sirene frenética nas proximidades do ACM Building.\nJá são 23:59 e o aluno de E.C. Mano José Nath Mário de EPS, mais conhecido como Mano Free Fire, ao tentar loucamente submeter seu código da lista nº 73 de IP no The Huxley, alinha a ordem de coincidências mais incrível da história do CIn, causando a abertura de um portal que subtamente o puxa para outra dimensão (o covil do mais perigoso da Terra, o dominador de Assembly, ACMáquina!).\nMas hoje não é o dia de sorte de Mano FF, pois, às 00:01, o The Huxley cai, fechando o portal, impedindo-o de voltar para casa...\n A única solução será encontrar ACMáquina e desalocá-lo, para que o nosso mundo de 0's e 1's volte ao equilíbrio.";
    char instrucaoFase1[] = "Você deverá passar pela Floresta da Decisão e desviar dos obstáculos.\nMas, tenha cuidado, você tem apenas 3 vidas. Encoste nos inimigos e você verá sua vida diminuir.\nO tempo urge, viajante, vamos!";
    char trasicaoFase1p2[] = "Parabéns... Você sobreviveu à passagem na Floresta da Decisão. Agora você está apto ao ato final: enfrentar ACMáquina e seu exército de assemblies, para, então, desalocá-lo.\nPrepare-se, pois o verdadeiro desafio começa agora!";
    char instrucaoFase2[] = "Você deve enfrentar ACMáquina e seus assemblies.\nVocê deverá sobreviver ao embate com ACMáquina por 3 minutos...\nAcredite, ele vai cansar!!\nAo final, não esqueça de dar o free, hein? Boa sorte, player!";
    char vitoria[] = "Parabéns, viajante. Você entendeu a importância do free e da revisão de código e ajudou Mano FF a sair do The Huxley...\nAgora... vamo jogar Free Fire?\n\nFeito por:\nEdson Júnior\nJosé Victor\nMário Mendonça\nNathalia Barbosa\nRicardo Ferreira";
    Rectangle container = {25, 25, screenWidth - 50, screenHeight - 50};

    BeginDrawing();
    //História da intro
    if (windowState == 0)
    {
        ClearBackground(DARKGRAY);
        DrawTextRec(GetFontDefault(), TextSubtext(historia, 0, ((*frameCounterHist) += 3) / 10), container, 25, 2, true, LIGHTGRAY);

        if ((*frameCounterHist) >= 850)
        {
            DrawText("Pressione qualquer tecla para continuar...", screenWidth / 4, screenHeight - 50, 20, SKYBLUE);

            if (GetKeyPressed())
            {
                gameState = 1;
                windowState = 1;
                (*frameCounterHist) = 0;
            }
        }
    }

    //Instruções da fase um
    else if (windowState == 1)
    {
        ClearBackground(DARKGRAY);
        DrawTextRec(GetFontDefault(), TextSubtext(instrucaoFase1, 0, ((*frameCounterHist) += 3) / 10), container, 30, 2, true, LIGHTGRAY);

        if ((*frameCounterHist) >= 850)
        {
            DrawText("Pressione qualquer tecla para continuar...", screenWidth / 4, screenHeight - 50, 20, SKYBLUE);
            if (GetKeyPressed())
            {
                gameState = 3;
                (*frameCounterHist) = 0;
            }
        }
    }

    //Transição entre fase um e dois
    else if (windowState == 2)
    {
        ClearBackground(DARKGRAY);
        DrawTextRec(GetFontDefault(), TextSubtext(trasicaoFase1p2, 0, ((*frameCounterHist) += 3) / 10), container, 30, 2, true, LIGHTGRAY);

        if ((*frameCounterHist) >= 850)
        {
            DrawText("Pressione qualquer tecla para continuar...", screenWidth / 4, screenHeight - 50, 20, SKYBLUE);

            if (GetKeyPressed())
            {
                windowState = 3;
                (*frameCounterHist) = 0;
            }
        }
    }

    //Instruções da fase dois
    else if (windowState == 3)
    {
        ClearBackground(DARKGRAY);
        DrawTextRec(GetFontDefault(), TextSubtext(instrucaoFase2, 0, ((*frameCounterHist) += 3) / 10), container, 30, 2, true, LIGHTGRAY);

        if ((*frameCounterHist) >= 850)
        {
            DrawText("Pressione qualquer tecla para continuar...", screenWidth / 4, screenHeight - 50, 20, SKYBLUE);

            if (GetKeyPressed())
            {
                gameState = 5;
                (*frameCounterHist) = 0;
            }
        }
    }

    //Final, parabéns!
    else // (windowState == 4)
    {
        ClearBackground(DARKGRAY);
        DrawTextRec(GetFontDefault(), TextSubtext(vitoria, 0, ((*frameCounterHist) += 3) / 10), container, 30, 2, true, LIGHTGRAY);
    }

    EndDrawing();
}

void menuPrincipal(int *frameCounterNome, int *frameCounterGrupo)
{
    int posXQuadrado = 10, posYQuadrado = 60, tamQuadrado = 100;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangle(0, 0, screenWidth, 50, BLUE);

    DrawRectangle(posXQuadrado, posYQuadrado, tamQuadrado, tamQuadrado, RED);
    DrawText("5", posXQuadrado + tamQuadrado / 3, posYQuadrado + tamQuadrado / 6, 80, WHITE);
    DrawText(TextSubtext(NOME_DO_JOGO, 0, ((*frameCounterNome) += 2) / 10), posXQuadrado + tamQuadrado + 10, posYQuadrado + tamQuadrado / 5, 60, BLACK);

    DrawRectangle(0, posYQuadrado + tamQuadrado + 10, screenWidth, 5, BLUE);
    DrawText(TextSubtext("Cadastrado por: \nejapj\njvls\nmsfm\nnfab\nrfc3", 0, ((*frameCounterGrupo) += 2) / 10), 10, posYQuadrado + tamQuadrado + 20, 20, DARKGRAY);

    DrawRectangle(0, posYQuadrado + tamQuadrado + 200, screenWidth / 2 - 3, 25, BLUE);
    DrawRectangle(screenWidth / 2 + 3, posYQuadrado + tamQuadrado + 200, screenWidth / 2 - 5, 25, GRAY);

    DrawText("Press SPACE to START", (screenWidth / 2 - 3) / 5, posYQuadrado + tamQuadrado + 204, 20, WHITE);

    DrawText("Instruções do jogo", screenWidth / 4, posYQuadrado + tamQuadrado + 240, 40, BLUE);
    DrawText("Ajude o nosso protagonista a voltar para casa!\nPule usando W ou seta para cima para escapar dos inimigos.\nFuja usando D/A ou seta para direita/esquerda.\nAtire usando F.", posXQuadrado, posYQuadrado + tamQuadrado + 300, 20, GRAY);

    if (IsKeyPressed(KEY_SPACE))
        gameState = 2;
    EndDrawing();
}

void faseUm()
{
    int currentFrameRun = 0, currentFrameJump = 0, framesCounter = 0, framesSpeed = 9;
    int currentFrameCarang = 0, currentFrameMuric = 0, veloc = 0, contaTempo = 0, vidas = 3;
    bool pulando = 0, colisao = 0, gameOver = 0, enter_portal = 0;
    float scrollingBack = 0.0f, scrollingFore = 0.0f;

    Texture2D background = LoadTexture("../assets/backgrounds/bg_1.png");
    Texture2D foreground = LoadTexture("../assets/backgrounds/bg_2.png");
    Texture2D protag_run = LoadTexture("../assets/protag_run.png");
    Texture2D protag_jump = LoadTexture("../assets/protag_jump.png");
    Texture2D caranguejo = LoadTexture("../assets/enemy/carang_corre.png");
    Texture2D muricoca = LoadTexture("../assets/enemy/muricoca.png");
    Texture2D quebrada = LoadTexture("../assets/pose_quebrada.png");
    Texture2D portal = LoadTexture("../assets/portal.png");

    Vector2 posProtag = {50.0f, 530.0f};
    Vector2 posCarang[2] = {{screenWidth, 580.0f}, {screenWidth, 580.0f}};
    Vector2 posMuric = {screenWidth, 500.0f};
    Vector2 posPort = {screenWidth, 550.0f};
    Rectangle personajao = {0.0f, 0.0f, (float)protag_run.width / 8, (float)protag_run.height};
    Rectangle caranguejao = {0.0, 0.0f, (float)caranguejo.width / 6, (float)caranguejo.height};
    Rectangle muricocao = {0.0f, 0.0f, (float)muricoca.width / 8, (float)muricoca.height};
    Rectangle pose_quebrada = {0.0f, 0.0f, (float)quebrada.width, (float)quebrada.height};
    Rectangle portalzao = {0.0f, 0.0f, (float)portal.width, (float)portal.height};

    //Retângulos de colisão
    Rectangle person_colisao = {50.0f, 530.0f, 50, 15};
    Rectangle carang_colisao[2] = {{screenWidth, 530.0f, 1, 15}, {screenWidth, 530.0f, 1, 15}};
    Rectangle murico_colisao = {screenWidth, 500.0f, 1, 15};
    Rectangle portal_colisao = {screenWidth, 530.0f, 1, 200};

    InitAudioDevice();
    Sound colisao_sound = LoadSound("../assets/musicas/edson.mp3");
    Music song = LoadMusicStream("../assets/musicas/Bipedal Mech.mp3");
    Music gameOver_song = LoadMusicStream("../assets/musicas/miss-game-over.mp3");
    PlayMusicStream(song);
    SetMusicVolume(song, 0.1f);
    SetSoundVolume(colisao_sound, 0.1f);

    while (!WindowShouldClose() && gameState == 3)
    {
        if (!gameOver)
        {
            UpdateMusicStream(song);
            scrollingBack -= 0.7f;
            scrollingFore -= 1.0f;

            if (scrollingBack <= -background.width * 2)
                scrollingBack = 0;
            if (scrollingFore <= -foreground.width * 2)
                scrollingFore = 0;

            framesCounter++;

            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;

                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || pulando)
                {
                    pulando = 1;
                    currentFrameJump++;
                    personajao.x = (float)currentFrameJump * (float)protag_jump.width / 7;
                    //se ele está subindo no pulo a pos.y diminui (sobe na tela)
                    if (currentFrameJump >= 0 && currentFrameJump <= 3)
                        posProtag.y -= 20;

                    //se ele está descendo no pulo a pos.y aumenta (desce na tela)
                    else
                        posProtag.y += 10;

                    //qnd atinge o frame 6 o pulo acaba e ele volta a pos.y padrão
                    if (currentFrameJump > 6)
                    {
                        posProtag.y = 530;
                        pulando = 0;
                    }
                }

                else
                {
                    currentFrameRun++;
                    if (currentFrameRun > 7)
                        currentFrameRun = 0;
                    personajao.x = (float)currentFrameRun * (float)protag_run.width / 8;
                    pulando = 0;
                    currentFrameJump = 0;
                }

                if (posCarang[0].x >= 0 || posCarang[1].x >= 0)
                {
                    currentFrameCarang++;
                    caranguejao.x = (float)currentFrameCarang * (float)caranguejo.width / 6;
                }

                if (posMuric.x >= 0)
                {
                    currentFrameMuric++;
                    muricocao.x = (float)currentFrameMuric * (float)muricoca.width / 8;
                }

                //Aparecimento do portal
                if (contaTempo >= 6900)
                {
                    posPort.x -= 12;
                    portal_colisao.x -= 12;
                }

                //---------------------------- mexer na posição dos bicho nos tempos indicados -------------
                if ((contaTempo >= 0 && contaTempo <= 480) || (contaTempo >= 540 && contaTempo <= 1020) ||
                    (contaTempo >= 1080 && contaTempo <= 1560) || (contaTempo >= 1620 && contaTempo <= 2100) ||
                    (contaTempo >= 2160 && contaTempo <= 2640) || (contaTempo >= 2700 && contaTempo <= 3180) ||
                    (contaTempo >= 3240 && contaTempo <= 3720) || (contaTempo >= 3780 && contaTempo <= 4260) ||
                    (contaTempo >= 4320 && contaTempo <= 4800) || (contaTempo >= 4860 && contaTempo <= 5340) ||
                    (contaTempo >= 5400 && contaTempo <= 5880) || (contaTempo >= 5940 && contaTempo <= 6420) ||
                    (contaTempo >= 6480 && contaTempo <= 6960))
                {
                    posCarang[0].x -= 12 - veloc;

                    //---------------------- variável p/ aumentar a velocidade dos bicho
                    if (contaTempo == 1080 || contaTempo == 2160 || contaTempo == 3240 || contaTempo == 4320 ||
                        contaTempo == 5400 || contaTempo == 6480)
                        veloc++;
                }

                else
                    posCarang[0].x = screenWidth;

                if ((contaTempo >= 65 && contaTempo <= 545) || (contaTempo >= 605 && contaTempo <= 1085) ||
                    (contaTempo >= 1145 && contaTempo <= 1625) || (contaTempo >= 1685 && contaTempo <= 2165) ||
                    (contaTempo >= 2225 && contaTempo <= 2705) || (contaTempo >= 2765 && contaTempo <= 3245) ||
                    (contaTempo >= 3305 && contaTempo <= 3785) || (contaTempo >= 3845 && contaTempo <= 4325) ||
                    (contaTempo >= 4385 && contaTempo <= 4865) || (contaTempo >= 4925 && contaTempo <= 5405) ||
                    (contaTempo >= 5465 && contaTempo <= 5945) || (contaTempo >= 6005 && contaTempo <= 6485) ||
                    (contaTempo >= 6545 && contaTempo <= 7025))
                    posMuric.x -= 12 - veloc;

                else
                    posMuric.x = screenWidth;

                if ((contaTempo >= 1300 && contaTempo <= 1780) || (contaTempo >= 1840 && contaTempo <= 2320) ||
                    (contaTempo >= 2395 && contaTempo <= 2875) || (contaTempo >= 2915 && contaTempo <= 3395) ||
                    (contaTempo >= 3455 && contaTempo <= 3935) || (contaTempo >= 3995 && contaTempo <= 4475) ||
                    (contaTempo >= 4535 && contaTempo <= 5015) || (contaTempo >= 5075 && contaTempo <= 5555) ||
                    (contaTempo >= 5615 && contaTempo <= 6095))
                    posCarang[1].x -= 12 - veloc;

                else
                    posCarang[1].x = screenWidth;

                //igualando a posição dos retângulos de colisão com a posição dos personagens
                person_colisao.y = posProtag.y;
                carang_colisao[0].x = posCarang[0].x;
                carang_colisao[1].x = posCarang[1].x;
                murico_colisao.x = posMuric.x;
            }

            //---------------------------Checar se houve colisão
            if ((CheckCollisionRecs(person_colisao, carang_colisao[0]) || CheckCollisionRecs(person_colisao, carang_colisao[1]) || CheckCollisionRecs(person_colisao, murico_colisao)) && colisao == 0)
            {
                vidas--;
                colisao = 1;
                PlaySound(colisao_sound);
            }

            if (GetCollisionRec(person_colisao, carang_colisao[0]).x == 0 && GetCollisionRec(person_colisao, carang_colisao[1]).x == 0 && GetCollisionRec(person_colisao, murico_colisao).x == 0)
                colisao = 0;

            if (CheckCollisionRecs(person_colisao, portal_colisao))
                enter_portal = 1;

            contaTempo++;
            //----------------------Desenhar texturas
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw background image twice
            // NOTE: Texture is scaled twice its size
            DrawTextureEx(background, (Vector2){scrollingBack, -850}, 0.0f, 2.0f, WHITE);
            DrawTextureEx(background, (Vector2){background.width * 2 + scrollingBack, -850}, 0.0f, 2.0f, WHITE);

            DrawTextureEx(foreground, (Vector2){scrollingFore, -850}, 0.0f, 2.0f, WHITE);
            DrawTextureEx(foreground, (Vector2){foreground.width * 2 + scrollingFore, -850}, 0.0f, 2.0f, WHITE);

            if (contaTempo >= 6900)
                DrawTextureRec(portal, portalzao, posPort, WHITE);

            DrawTextureRec(pulando ? protag_jump : protag_run, personajao, posProtag, colisao ? RED : WHITE);
            DrawText(TextFormat("TEMPO PERCORRIDO: %.1fs", (float)contaTempo / 60), 10, 10, 20, RAYWHITE);
            DrawText(TextFormat("VIDAS RESTANTES: %d", vidas), 10, 30, 20, RED);

            //------------------------------------ Spawnar os bicho no tempo indicado -------------
            if ((contaTempo >= 0 && contaTempo <= 480) || (contaTempo >= 540 && contaTempo <= 1020) ||
                (contaTempo >= 1080 && contaTempo <= 1560) || (contaTempo >= 1620 && contaTempo <= 2100) ||
                (contaTempo >= 2160 && contaTempo <= 2640) || (contaTempo >= 2700 && contaTempo <= 3180) ||
                (contaTempo >= 3240 && contaTempo <= 3720) || (contaTempo >= 3780 && contaTempo <= 4260) ||
                (contaTempo >= 4320 && contaTempo <= 4800) || (contaTempo >= 4860 && contaTempo <= 5340) ||
                (contaTempo >= 5400 && contaTempo <= 5880) || (contaTempo >= 5940 && contaTempo <= 6420) ||
                (contaTempo >= 6480 && contaTempo <= 6960))
                DrawTextureRec(caranguejo, caranguejao, posCarang[0], WHITE);

            if ((contaTempo >= 65 && contaTempo <= 545) || (contaTempo >= 605 && contaTempo <= 1085) ||
                (contaTempo >= 1145 && contaTempo <= 1625) || (contaTempo >= 1685 && contaTempo <= 2165) ||
                (contaTempo >= 2225 && contaTempo <= 2705) || (contaTempo >= 2765 && contaTempo <= 3245) ||
                (contaTempo >= 3305 && contaTempo <= 3785) || (contaTempo >= 3845 && contaTempo <= 4325) ||
                (contaTempo >= 4385 && contaTempo <= 4865) || (contaTempo >= 4925 && contaTempo <= 5405) ||
                (contaTempo >= 5465 && contaTempo <= 5945) || (contaTempo >= 6005 && contaTempo <= 6485) ||
                (contaTempo >= 6545 && contaTempo <= 7025))
                DrawTextureRec(muricoca, muricocao, posMuric, WHITE);

            if ((contaTempo >= 1300 && contaTempo <= 1780) || (contaTempo >= 1840 && contaTempo <= 2320) ||
                (contaTempo >= 2395 && contaTempo <= 2875) || (contaTempo >= 2915 && contaTempo <= 3395) ||
                (contaTempo >= 3455 && contaTempo <= 3935) || (contaTempo >= 3995 && contaTempo <= 4475) ||
                (contaTempo >= 4535 && contaTempo <= 5015) || (contaTempo >= 5075 && contaTempo <= 5555) ||
                (contaTempo >= 5615 && contaTempo <= 6095))
                DrawTextureRec(caranguejo, caranguejao, posCarang[1], WHITE);

            EndDrawing();
            //--------------Completou a fase
            if (enter_portal)
            {
                StopMusicStream(song);
                gameState = 4;
            }
            //--------------Perdeu
            if (vidas <= 0)
                gameOver = 1;
        }

        //--------------Game over
        else
        {
            if (IsMusicPlaying(song))
                StopMusicStream(song);
            PlayMusicStream(gameOver_song);
            SetMusicVolume(gameOver_song, 0.2f);
            UpdateMusicStream(gameOver_song);

            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("game over", screenWidth / 2, screenHeight / 2, 50, WHITE);
            DrawText("lamentavel di vdd 9nha", screenWidth / 2 + 50, screenHeight / 2 + 50, 20, LIME);

            DrawTextureRec(quebrada, pose_quebrada, (Vector2){screenWidth / 2, screenHeight / 2 - 65}, WHITE);

            DrawText("F para jogar dnv", screenWidth / 2, screenHeight - 30, 20, LIME);

            if (IsKeyPressed(KEY_F))
            {
                //Fase um novamente
                faseUm();
            }

            EndDrawing();
        }
    }

    //----------------------------Unload nas texturas e sons
    UnloadTexture(background);
    UnloadTexture(foreground);
    UnloadTexture(protag_run);
    UnloadTexture(protag_jump);
    UnloadTexture(caranguejo);
    UnloadTexture(muricoca);
    UnloadTexture(quebrada);
    UnloadTexture(portal);

    UnloadSound(colisao_sound);
    UnloadMusicStream(song);
    UnloadMusicStream(gameOver_song);

    CloseAudioDevice();

    //Caso você feche o jogo sem ganhar, gameState vai pra 10, damos unload na main e fechamos a janela
    //Caso você ganhe, a janela não fecha, pois gameState será 4. Então, voltamos para main e continuamos normalmente
    if (gameState == 3)
        gameState = 10;
}

void faseDois(void)
{
    static Enemy enemy[NUM_MAX_ENEMIES] = {0};
    static Shoot shoot[NUM_SHOOTS] = {0};
    static int activeEnemies = 2;
    int framesCounter = 0;
    int shootRate = 0;
    bool tipo = true;

    int currentFrameRun = 0, currentFrameJump = 0, currentFrameQuieto = 0, framesSpeed = 9;
    int contaTempo = 0, vidas = 3;
    bool pulando = 0, atirando = 0, andando = 0, colisao = 0, gameOver = 0;

    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        (tipo == true) ? (tipo = false) : (tipo = true); //para alternar o tipo a cada inimigo
        enemy[i].speed.x = 3;
        enemy[i].speed.y = 3;
        enemy[i].active = true;
        enemy[i].color = GRAY;
        enemy[i].type = tipo;
        //-------------------Colisão dos inimigos:
        enemy[i].pos.x = GetRandomValue(screenHeight, screenWidth - 80);
        enemy[i].pos.y = GetRandomValue(570, 520);
        enemy[i].pos.width = 1;
        enemy[i].pos.height = 20;
        ////-------------------Sprite:
        enemy[i].rec.x = 0.0f;
        enemy[i].rec.y = 0.0f;
        enemy[i].rec.width = 20;
        enemy[i].rec.height = 20;
    }

    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = 0;
        shoot[i].rec.y = 0;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }

    Texture2D background = LoadTexture("../assets/backgrounds/bg_sci.png");
    Texture2D protag_run = LoadTexture("../assets/protag_run.png");
    Texture2D protag_jump = LoadTexture("../assets/protag_jump.png");
    Texture2D protag_quieto = LoadTexture("../assets/protag_quieto.png");
    Texture2D protag_shoot = LoadTexture("../assets/protag_shoot.png");
    Texture2D zero = LoadTexture("../assets/enemy/zero.png");
    Texture2D um = LoadTexture("../assets/enemy/um.png");
    Texture2D acm = LoadTexture("../assets/enemy/acmaquina.png");
    Texture2D quebrada = LoadTexture("../assets/pose_quebrada.png");

    Vector2 posProtag = {50.0f, 500.0f};
    Rectangle personajao = {0.0f, 0.0f, (float)protag_run.width / 8, (float)protag_run.height};
    Rectangle pose_quebrada = {0.0f, 0.0f, (float)quebrada.width, (float)quebrada.height};
    Rectangle acmaquina = {0.0f, 0.0f, (float)acm.width, (float)acm.height};

    //-----------------------------Retângulos de colisão
    Rectangle person_colisao = {50.0f, 500.0f, 30, 70};

    //-----------------------------Áudio
    InitAudioDevice();
    Music song = LoadMusicStream("../assets/musicas/cyber city 2-b.mp3");
    Music gameOver_song = LoadMusicStream("../assets/musicas/miss-game-over.mp3");
    Sound colisao_sound = LoadSound("../assets/musicas/edson.mp3");
    PlayMusicStream(song);
    SetSoundVolume(colisao_sound, 0.2f);
    SetMusicVolume(song, 0.1f);

    while (!WindowShouldClose() && gameState == 5)
    {
        if (!gameOver)
        {
            UpdateMusicStream(song);
            framesCounter++;

            //----------------------Movimento dos inimigos
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    enemy[i].pos.x -= enemy[i].speed.x;

                    if (enemy[i].pos.x < 0)
                        enemy[i].pos.x = GetRandomValue(screenHeight, screenWidth - 80);
                }
            }

            //----------------------Movimento do personagem
            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                //pular
                if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || pulando)
                {
                    pulando = 1;
                    atirando = 0;
                    andando = 0;
                    currentFrameJump++;
                    personajao.x = (float)currentFrameJump * (float)protag_jump.width / 7;
                    //Ao pular, o personagem sobe/desce e vai p/ frente
                    if (currentFrameJump >= 0 && currentFrameJump <= 3)
                    {
                        posProtag.x += 10;
                        posProtag.y -= 20;
                    }

                    else
                    {
                        posProtag.x += 5;
                        posProtag.y += 10;
                    }

                    if (currentFrameJump > 6)
                    {
                        posProtag.y = 500;
                        pulando = 0;
                    }
                }
                //andar
                else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
                {
                    currentFrameRun++;
                    if (currentFrameRun > 7)
                        currentFrameRun = 0;
                    personajao.x = (float)currentFrameRun * (float)protag_run.width / 8;

                    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
                        posProtag.x += 12;
                    else
                        posProtag.x -= 12;

                    pulando = 0;
                    atirando = 0;
                    andando = 1;
                    currentFrameJump = 0;
                }
                //atirar
                else if (IsKeyDown(KEY_F))
                {
                    personajao.x = (float)protag_shoot.width;
                    atirando = 1;
                    pulando = 0;
                    andando = 0;
                    currentFrameJump = 0;
                }
                //parado
                else
                {
                    currentFrameQuieto++;
                    if (currentFrameQuieto > 3)
                        currentFrameQuieto = 0;
                    personajao.x = (float)currentFrameQuieto * (float)protag_quieto.width / 4;
                    pulando = 0;
                    atirando = 0;
                    andando = 0;
                    currentFrameJump = 0;
                }

                //evitar que o personagem "fuja" da tela
                if (posProtag.x >= (screenWidth - 80))
                    posProtag.x = screenWidth - 80;

                else if (posProtag.x <= 0)
                    posProtag.x = 0;

                //igualando a posição do retângulo de colisão com a posição do personagem
                person_colisao.x = posProtag.x;
                person_colisao.y = posProtag.y;
            }

            if (IsKeyDown(KEY_F))
            {
                shootRate += 5;

                for (int i = 0; i < NUM_SHOOTS; i++)
                {
                    if (!shoot[i].active && shootRate % 20 == 0)
                    {
                        shoot[i].rec.x = (posProtag.x + 60);
                        shoot[i].rec.y = (posProtag.y + 33);
                        shoot[i].active = true;
                        break;
                    }
                }
            }

            // Gerador de Tiros
            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot[i].active)
                {
                    // Movimento do tiro
                    shoot[i].rec.x += shoot[i].speed.x;

                    // Sumir com o retângulo inimigo
                    for (int j = 0; j < activeEnemies; j++)
                    {
                        if (enemy[j].active)
                        {
                            if (CheckCollisionRecs(shoot[i].rec, enemy[j].pos))
                            {
                                shoot[i].active = false;
                                enemy[j].pos.x = GetRandomValue(screenWidth, screenWidth + 1000);
                                shootRate = 0;
                            }
                        }

                        if (shoot[i].rec.x + shoot[i].rec.width >= screenWidth)
                        {
                            shoot[i].active = false;
                            shootRate = 0;
                        }
                    }
                }
            }

            //----------------------Checar colisões com inimigos
            for (int j = 0; j < activeEnemies; j++)
            {
                if (enemy[j].active && CheckCollisionRecs(person_colisao, enemy[j].pos))
                {
                    enemy[j].active = false;
                    enemy[j].pos.x = GetRandomValue(screenWidth, screenWidth + 1000);
                    vidas--;
                    colisao = 1;
                    PlaySound(colisao_sound);
                }
            }

            contaTempo++;
            //----------------------Desenhar texturas
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(background, 0.0f, 0.0f, WHITE);

            //operador ternário aninhado para ver qual textura desenhar
            DrawTextureRec(pulando ? protag_jump : atirando ? protag_shoot
                                               : andando    ? protag_run
                                                            : protag_quieto,
                           personajao, posProtag, colisao ? RED : WHITE);

            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    //operador ternário para ver qual textura desenhar
                    DrawTextureRec((enemy[i].type == true) ? zero : um, enemy[i].rec, (Vector2){enemy[i].pos.x, enemy[i].pos.y}, enemy[i].color);
                }
                else
                    enemy[i].active = true;
            }

            for (int i = 0; i < NUM_SHOOTS; i++)
                if (shoot[i].active)
                    DrawRectangleRec(shoot[i].rec, shoot[i].color);

            DrawTextureRec(acm, acmaquina, (Vector2){screenWidth - 80, 520.0f}, WHITE);

            DrawText(TextFormat("TEMPO PERCORRIDO: %.1fs", (float)contaTempo / 60), 10, 10, 20, RAYWHITE);
            DrawText(TextFormat("VIDAS RESTANTES: %d", vidas), 10, 30, 20, RED);
            EndDrawing();

            colisao = 0;

            //--------------Perdeu
            if (vidas <= 0)
                gameOver = 1;
            //-------------Ganhou depois de três minutos
            if (contaTempo >= 10800)
            {
                StopMusicStream(song);
                gameState = 6;
            }
        }

        //--------------Game over
        else
        {
            if (IsMusicPlaying(song))
                StopMusicStream(song);
            PlayMusicStream(gameOver_song);
            SetMusicVolume(gameOver_song, 0.2f);
            UpdateMusicStream(gameOver_song);

            BeginDrawing();
            ClearBackground(BLACK);

            DrawText("game over", screenWidth / 2, screenHeight / 2, 50, WHITE);
            DrawText("lamentavel di vdd 9nha", screenWidth / 2 + 50, screenHeight / 2 + 50, 20, LIME);

            DrawTextureRec(quebrada, pose_quebrada, (Vector2){screenWidth / 2, screenHeight / 2 - 65}, WHITE);

            DrawText("F para jogar dnv", screenWidth / 2, screenHeight - 30, 20, LIME);

            if (IsKeyPressed(KEY_F))
            {
                //Fase dois novamente
                faseDois();
            }

            EndDrawing();
        }
    }

    //----------------------------Unload nas texturas e sons
    UnloadTexture(zero);
    UnloadTexture(um);
    UnloadTexture(background);
    UnloadTexture(protag_run);
    UnloadTexture(protag_jump);
    UnloadTexture(protag_quieto);
    UnloadTexture(protag_shoot);
    UnloadTexture(quebrada);
    UnloadTexture(acm);

    UnloadSound(colisao_sound);
    UnloadMusicStream(song);
    UnloadMusicStream(gameOver_song);

    //Caso você feche o jogo sem ganhar, gameState vai pra 10, damos unload na main e fechamos a janela
    //Caso você ganhe, a janela não fecha, pois gameState será 6. Então, voltamos para main e continuamos normalmente
    //Não chamamos a CloseAudioDevice() pois haverá música ainda
    if (gameState == 5)
        gameState = 10;
}