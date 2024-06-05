#include "Leds/Leds.h"
#include "debug.h"
#include <Arduino.h>

#if CONFIG_IDF_TARGET_ESP32
    #define GPIO_LEDS_RGB GPIO_NUM_25  // SOLO PARA ESP32!!!
#else
    #define GPIO_LEDS_RGB GPIO_NUM_8  // SOLO PARA ESP32!!!
#endif
#define CANTIDAD_DE_LEDS_RGB      10
#define CANTIDAD_DE_LEDS_LOGO     0
#define CANTIDAD_DE_LEDS_SEMAFORO 0

uint8_t CantidadLedsTraseros = CANTIDAD_DE_LEDS_RGB;
uint8_t CantidadLedsLogo     = CANTIDAD_DE_LEDS_LOGO;
uint8_t CantidadLedsSemaforo = CANTIDAD_DE_LEDS_SEMAFORO;
int CantidadLedsTotales      = CANTIDAD_DE_LEDS_RGB + CANTIDAD_DE_LEDS_SEMAFORO + CANTIDAD_DE_LEDS_LOGO;

CRGB *leds = nullptr;
// CRGB *ledsBackup             = nullptr;
bool EsperaDeEncendido = false;  // hay un efecto que bloquea los efectos y espera un tiempo para encenderlos
uint32_t TimeoutEspera = 0;      // "
uint8_t brilloBackup   = 255;

void LedsInit()
{
    static bool inicializado = false;
    if (!inicializado)
    {
        leds = (CRGB *)malloc(sizeof(CRGB) * CantidadLedsTotales);
        // ledsBackup = (CRGB *)malloc(sizeof(CRGB) * CantidadLedsTotales);
        CFastLED::addLeds<WS2812B, GPIO_LEDS_RGB, GRB>(leds, 10);
        AllLeds(CRGB::Black);
        inicializado = true;
    }
    EsperaDeEncendido = false;
    TimeoutEspera     = 0;
}

// static void Push()
// {
//     memcpy(ledsBackup, leds, sizeof(CRGB) * CantidadLedsTotales);
//     brilloBackup = FastLED.getBrightness();
// }

// static void Pop()
// {
//     memcpy(leds, ledsBackup, sizeof(CRGB) * CantidadLedsTotales);
//     FastLED.setBrightness(brilloBackup);
//     FastLED.show();
// }

// es el loop del Arduino que controla el tiempo fuera para el 'EsperaDeEncendido'
void LoopLeds()
{
    static uint32_t T = 0;
    if (millis() < T) return;
    T = millis() + 1000;
    if (TimeoutEspera == 0)
        EsperaDeEncendido = false;
    else
        TimeoutEspera--;
}

void LedsTemporizadosOFF()
{
    TimeoutEspera     = 0;
    EsperaDeEncendido = false;
}

void NextLed(CRGB col, bool dejarCola /*true*/, bool pingPong /*true*/)
{
    if (EsperaDeEncendido) return;

    static int led      = 0;
    static bool sentido = true;
    if (!dejarCola) leds[led] = CRGB::Black;
    if (sentido)
    {
        if (++led == CantidadLedsTraseros - (pingPong ? 1 : 0))
        {
            if (!pingPong)
                led = 0;
            else
                sentido = false;
        }
    }
    else
    {
        if (--led == 0) sentido = true;
    }
    if (dejarCola) fadeToBlackBy(leds, CantidadLedsTraseros, 150);
    leds[led] = col;
    FastLED.show();
}

// TODO: ARREGLAR ... tiene que distribuir los colores entre los leds del semaforo,
// pero equitativamente...
// (colores no es la misma cantidad de leds que el semaforo)
void LedsSemaforo(uint8_t *colores, int size)
{
    float banda = (float)CantidadLedsSemaforo / (float)size;
    float acum  = banda;
    int ini     = CantidadLedsTraseros;
    int cant    = CantidadLedsSemaforo;
    for (int j = 0, i = ini; i < cant; i++)
    {
        leds[i] = CHSV(colores[j], 255, 255);
        if (i >= (int)acum)
        {
            acum += banda;
            if (j + 1 < size) j++;
        }
    }
    FastLED.show();
}

void AllLeds(CRGB col, uint8_t brillo)
{
    if (EsperaDeEncendido) return;
    fill_solid(leds, CantidadLedsTotales, col);
    if (brillo < 255) fadeLightBy(leds, CantidadLedsTotales, brillo);
    FastLED.show();
}

void AllLedsTemporizados(CRGB col, int segundos)
{
    EsperaDeEncendido = true;
    TimeoutEspera     = (segundos == 0) ? 1 : segundos;
    fill_solid(leds, CantidadLedsTotales, col);
    FastLED.show();
}

void LedsLogoTemporizados(CRGB col, int segundos)
{
    EsperaDeEncendido = true;
    TimeoutEspera     = (segundos == 0) ? 1 : segundos;
    // me voy con un offset hasta los leds del Logo
    fill_solid(leds + CantidadLedsSemaforo, CantidadLedsLogo, col);
    FastLED.show();
}

void LedBlink(int ledNum, CRGB col)
{
    // Push();

    leds[ledNum] = col;
    FastLED.setBrightness(255);
    FastLED.show();
    delay(50);

    // Pop();
}

// enciende un LED y apaga el resto
void LedON(int ledNum, CRGB colorON, CRGB colorOFF)
{
    fill_solid(leds, CantidadLedsTraseros, colorOFF);
    leds[ledNum] = colorON;
    FastLED.setBrightness(255);
    FastLED.show();
}

// enciende un led y no toca el resto.
void LedON(int ledNum, CRGB colorON)
{
    leds[ledNum] = colorON;
    FastLED.setBrightness(255);
    FastLED.show();
}

void FlashGordon(CRGB col, bool alverre)
{
    // Push();

    FastLED.setBrightness(255);

    for (int i = 0; i < CantidadLedsTraseros + CantidadLedsSemaforo; ++i)
    {
        memset(leds, 0, CantidadLedsTraseros + CantidadLedsSemaforo);

        if (alverre)
            leds[CantidadLedsTraseros + CantidadLedsSemaforo - i - 1] = col;
        else
            leds[i] = col;

        FastLED.show();
        delay(4);
    }

    // Pop();
}

// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void FrameOrgullo()
{
    static uint16_t sPseudotime = 0;
    static uint16_t sLastMillis = 0;
    static uint16_t sHue16      = 0;

    uint8_t sat8                  = beatsin88(87, 220, 250);
    uint8_t brightdepth           = beatsin88(341, 96, 224);
    uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
    uint8_t msmultiplier          = beatsin88(147, 23, 60);

    uint16_t hue16    = sHue16;  // gHue * 256;
    uint16_t hueinc16 = beatsin88(113, 1, 3000);

    uint16_t ms      = millis();
    uint16_t deltams = ms - sLastMillis;
    sLastMillis      = ms;
    sPseudotime += deltams * msmultiplier;
    sHue16 += deltams * beatsin88(400, 5, 9);
    uint16_t brightnesstheta16 = sPseudotime;

    for (uint16_t i = 0; i < CantidadLedsTotales; i++)
    {
        hue16 += hueinc16;
        uint8_t hue8 = hue16 / 256;

        brightnesstheta16 += brightnessthetainc16;
        uint16_t b16 = sin16(brightnesstheta16) + 32768;

        uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
        uint8_t bri8   = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
        bri8 += (255 - brightdepth);

        CRGB newcolor = CHSV(hue8, sat8, bri8);

        uint16_t pixelnumber = i;
        pixelnumber          = (CantidadLedsTotales - 1) - pixelnumber;

        nblend(leds[pixelnumber], newcolor, 64);
    }
    FastLED.show();
}

#define COOLING    55
#define SPARKING   120
#define BRIGHTNESS 200
bool gReverseDirection = false;

void FrameFire()
{
    // Array of temperature readings at each simulation cell
    static uint8_t *heat = nullptr;
    if (heat == nullptr) heat = (uint8_t *)malloc(sizeof(uint8_t) * CantidadLedsTraseros);

    // Step 1.  Cool down every cell a little
    for (size_t i = 0; i < CantidadLedsTraseros; i++)
    {
        i = qsub8(heat[i], random8(0, ((COOLING * 10) / CantidadLedsTraseros) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = CantidadLedsTraseros - 1; k >= 2; k--)
    {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKING)
    {
        int y   = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < CantidadLedsTraseros; j++)
    {
        CRGB color = HeatColor(heat[j]);
        int pixelnumber;
        if (gReverseDirection)
        {
            pixelnumber = (CantidadLedsTraseros - 1) - j;
        }
        else
        {
            pixelnumber = j;
        }
        leds[pixelnumber] = color;
    }
    FastLED.show();  // display this frame
}

void LedsInFire()
{
    EVERY_N_MILLISECONDS(20) { FrameFire(); }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
    for (uint16_t i = 0; i < CantidadLedsTraseros; i++)
    {
        leds[i].blue  = scale8(leds[i].blue, 145);
        leds[i].green = scale8(leds[i].green, 200);
        leds[i] |= CRGB(2, 5, 7);
    }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
    uint8_t basethreshold = beatsin8(9, 55, 65);
    uint8_t wave          = beat8(7);

    for (uint16_t i = 0; i < CantidadLedsTraseros; i++)
    {
        uint8_t threshold = scale8(sin8(wave), 20) + basethreshold;
        wave += 7;
        uint8_t l = leds[i].getAverageLight();
        if (l > threshold)
        {
            uint8_t overage  = l - threshold;
            uint8_t overage2 = qadd8(overage, overage);
            leds[i] += CRGB(overage, overage2, qadd8(overage2, overage2));
        }
    }
}

// Add one layer of waves into the led array
void pacifica_one_layer(CRGBPalette16 &p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
    uint16_t ci             = cistart;
    uint16_t waveangle      = ioff;
    uint16_t wavescale_half = (wavescale / 2) + 20;
    for (uint16_t i = 0; i < CantidadLedsTraseros; i++)
    {
        waveangle += 250;
        uint16_t s16 = sin16(waveangle) + 32768;
        uint16_t cs  = scale16(s16, wavescale_half) + wavescale_half;
        ci += cs;
        uint16_t sindex16 = sin16(ci) + 32768;
        uint8_t sindex8   = scale16(sindex16, 240);
        CRGB c            = ColorFromPalette(p, sindex8, bri, LINEARBLEND);
        leds[i] += c;
    }
}

CRGBPalette16 pacifica_palette_1 = {0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50};
CRGBPalette16 pacifica_palette_2 = {0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F};
CRGBPalette16 pacifica_palette_3 = {0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF};

void FrameOcean()
{
    // Increment the four "color index start" counters, one for each wave layer.
    // Each is incremented at a different speed, and the speeds vary over time.
    static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
    static uint32_t sLastms = 0;
    uint32_t ms             = GET_MILLIS();
    uint32_t deltams        = ms - sLastms;
    sLastms                 = ms;
    uint16_t speedfactor1   = beatsin16(3, 179, 269);
    uint16_t speedfactor2   = beatsin16(4, 179, 269);
    uint32_t deltams1       = (deltams * speedfactor1) / 256;
    uint32_t deltams2       = (deltams * speedfactor2) / 256;
    uint32_t deltams21      = (deltams1 + deltams2) / 2;
    sCIStart1 += (deltams1 * beatsin88(1011, 10, 13));
    sCIStart2 -= (deltams21 * beatsin88(777, 8, 11));
    sCIStart3 -= (deltams1 * beatsin88(501, 5, 7));
    sCIStart4 -= (deltams2 * beatsin88(257, 4, 6));

    // Clear out the LED array to a dim background blue-green
    fill_solid(leds, CantidadLedsTraseros, CRGB(2, 6, 10));

    // Render each of four layers, with different scales and speeds, that vary over time
    pacifica_one_layer(pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0 - beat16(301));
    pacifica_one_layer(pacifica_palette_2, sCIStart2, beatsin16(4, 6 * 256, 9 * 256), beatsin8(17, 40, 80), beat16(401));
    pacifica_one_layer(pacifica_palette_3, sCIStart3, 6 * 256, beatsin8(9, 10, 38), 0 - beat16(503));
    pacifica_one_layer(pacifica_palette_3, sCIStart4, 5 * 256, beatsin8(8, 10, 28), beat16(601));

    // Add brighter 'whitecaps' where the waves lines up more
    pacifica_add_whitecaps();

    // Deepen the blues and greens a bit
    pacifica_deepen_colors();
    FastLED.show();
}

void LedsOcean()
{
    EVERY_N_MILLISECONDS(20) { FrameOcean(); }
}

// Espera que se pulse el botón, y espera que esté pulsado durante 5 segundos, sino abort!
bool VerificaBotonDeFabrica()
{
    bool pulsado;
    bool toggle        = false;
    const char *frase  = "\rSI DEJA PULSADO MAS DE 4 SEGUNDOS ESTE BOTON SE BORRARA LA CONFIGURACION !!!";
    const int lenFrase = strlen(frase);
    int indexFrase     = 0;
    int tiempo         = 0;

    pinMode(GPIO_NUM_0, INPUT_PULLUP);

    SetTerminalColor(TERM_MAGENTA, TERM_ITALICA);
    printf("Pulse BOOT durante 4 segundos para volver a valores de fabrica:\n");
    fflush(stdout);
    while (tiempo < 250)
    {
        pulsado = !digitalRead(GPIO_NUM_0);

        if (pulsado)
        {
            NextLed(blend(CRGB::Red, CRGB::Magenta, indexFrase), false);
            delay(lenFrase - indexFrase);
        }
        else
            FrameOrgullo();

        EVERY_N_MILLISECONDS(50)
        {
            if (pulsado)
            {
                toggle = true;
                printf("%c", frase[indexFrase++]);
                fflush(stdout);
            }
            else
            {
                if (toggle)
                    printf("\n");
                else
                    printf(".");
                fflush(stdout);
                indexFrase = 0;  // si no se pulsa se resetea el indexFrase
                toggle     = false;
            }

            // si se pulsa durante 4 segundos chau.
            if (indexFrase > lenFrase)
            {
                printf("!!!\n");
                fflush(stdout);
                SetTerminalColor(TERM_RESET);
                return true;  // PULSADO!
            }
            tiempo++;
        }
    }
    SetTerminalColor(TERM_RESET);
    printf("\n");  // NO-PULSADO!
    fflush(stdout);
    return false;
}

// Knight Industries Two Thousand
void LedsKitt()
{
    AllLeds(CRGB::Black);
    for (int i = 0; i < CantidadLedsTraseros + 2; ++i)
    {
        if (i < CantidadLedsTraseros) leds[i] = CRGB::Red;
        fadeToBlackBy(leds, CantidadLedsTraseros, 70);
        FastLED.show();
        FastLED.delay(55);
    }
    for (int i = 0; i < CantidadLedsTraseros * 2; ++i)
    {
        if (i < CantidadLedsTraseros) leds[CantidadLedsTraseros - i] = CRGB::Red;
        fadeToBlackBy(leds, CantidadLedsTraseros, 70);
        FastLED.show();
        FastLED.delay(55);
    }
}

[[noreturn]] void Navidad()
{
    // gpio_reset_pin(LORA_TXEN_GPIO);
    // gpio_set_direction(LORA_TXEN_GPIO, GPIO_MODE_OUTPUT);
    // gpio_set_level(LORA_TXEN_GPIO, 0);

    int ID            = 7;  // cada minion con un ID distinto
    int juego         = 0;
    int minion        = 0;
    int cambioDeColor = 0;
    int color         = 0;
    while (true)
    {
        EVERY_N_SECONDS(15) { ++juego; }

        EVERY_N_MILLISECONDS(18)
        {
            switch (juego)
            {
                case 0:
                    FrameFire();
                    break;
                case 1:
                    FrameOcean();
                    break;
                case 2:
                    FrameOrgullo();
                    break;
                case 3:
                    if (++cambioDeColor % 10 == 0) color++;
                    NextLed(color % 2 == 0 ? CRGB::Green : CRGB::Red);
                    break;
                case 4:
                    AllLeds(CRGB::Black);
                    if (minion++ == ID) FlashGordon(CHSV(random8(0, 255), 255, 255), ++color % 2 == 0);
                    if (minion > 7) minion = 0;
                    delay(100);
                    break;
                default:
                    juego = 0;
                    AllLeds(CRGB::Black);
                    break;
            }
        }
    }
}

void LedsLogo(CRGB col)
{
    auto inicioLedLogo = leds + CantidadLedsTraseros;
    fill_solid(inicioLedLogo, CantidadLedsLogo, col);
    FastLED.show();
}
