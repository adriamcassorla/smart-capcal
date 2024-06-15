#include <FastLED.h>

// Gradient palette "cw2_046_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/cw/2/tn/cw2-046.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE(darkGreenPalette){
  0, 4, 12, 7,
  127, 9, 40, 6,
  255, 45, 67, 5
};

// Gradient palette "Naturel_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/colo/smorin2002/tn/Naturel.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

DEFINE_GRADIENT_PALETTE(lightGreenPalette){
  0, 13, 62, 7,
  51, 13, 62, 7,
  51, 19, 68, 7,
  102, 19, 68, 7,
  102, 32, 77, 9,
  153, 32, 77, 9,
  153, 58, 79, 9,
  204, 58, 79, 9,
  204, 90, 80, 13,
  255, 90, 80, 13
};

CRGBPalette16 bottomPalette = darkGreenPalette;
CRGBPalette16 topPalette = lightGreenPalette;