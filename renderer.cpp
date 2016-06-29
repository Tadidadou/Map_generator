#include <iostream>
#include <noise/interp.h>
#include <noise/mathconsts.h>
#include "renderer.hpp"
#include "constants.hpp"

using namespace noise::utils;

RendererVertex::RendererVertex (sf::VertexArray destVertexArray):
  m_isLightEnabled    (false),
  m_isWrapEnabled     (false),
  m_lightAzimuth      (45.0),
  m_lightBrightness   (1.0),
  m_lightColor        (255, 255, 255, 255),
  m_lightContrast     (1.0),
  m_lightElev         (45.0),
  m_lightIntensity    (1.0),
  m_pBackgroundImage  (NULL),
  m_pDestImage        (NULL),
  m_pSourceNoiseMap   (NULL),
  m_recalcLightValues (true)
{
  m_pDestVertexArray = destVertexArray;
  BuildGrayscaleGradient ();
};


void RendererVertex::AddGradientPoint (double gradientPos, const Color& gradientColor)
{
  m_gradient.AddGradientPoint (gradientPos, gradientColor);
}


void RendererVertex::BuildGrayscaleGradient ()
{
  ClearGradient ();
  m_gradient.AddGradientPoint (-1.0, Color (  0,   0,   0, 255));
  m_gradient.AddGradientPoint ( 1.0, Color (255, 255, 255, 255));
}


void RendererVertex::BuildTerrainGradient ()
{
  ClearGradient ();
  m_gradient.AddGradientPoint (-1.00, Color (  0,   0, 128, 255));
  m_gradient.AddGradientPoint (-0.20, Color ( 32,  64, 128, 255));
  m_gradient.AddGradientPoint (-0.04, Color ( 64,  96, 192, 255));
  m_gradient.AddGradientPoint (-0.02, Color (192, 192, 128, 255));
  m_gradient.AddGradientPoint ( 0.00, Color (  0, 192,   0, 255));
  m_gradient.AddGradientPoint ( 0.25, Color (192, 192,   0, 255));
  m_gradient.AddGradientPoint ( 0.50, Color (160,  96,  64, 255));
  m_gradient.AddGradientPoint ( 0.75, Color (128, 255, 255, 255));
  m_gradient.AddGradientPoint ( 1.00, Color (255, 255, 255, 255));
}


Color RendererVertex::CalcDestColor (const Color& sourceColor, const Color& backgroundColor, double lightValue) const
{
  double sourceRed   = (double)sourceColor.red   / 255.0;
  double sourceGreen = (double)sourceColor.green / 255.0;
  double sourceBlue  = (double)sourceColor.blue  / 255.0;
  double sourceAlpha = (double)sourceColor.alpha / 255.0;
  double backgroundRed   = (double)backgroundColor.red   / 255.0;
  double backgroundGreen = (double)backgroundColor.green / 255.0;
  double backgroundBlue  = (double)backgroundColor.blue  / 255.0;

  // First, blend the source color to the background color using the alpha
  // of the source color.
  double red   = LinearInterp (backgroundRed,   sourceRed  , sourceAlpha);
  double green = LinearInterp (backgroundGreen, sourceGreen, sourceAlpha);
  double blue  = LinearInterp (backgroundBlue,  sourceBlue , sourceAlpha);

  if (m_isLightEnabled) {

    // Now calculate the light color.
    double lightRed   = lightValue * (double)m_lightColor.red   / 255.0;
    double lightGreen = lightValue * (double)m_lightColor.green / 255.0;
    double lightBlue  = lightValue * (double)m_lightColor.blue  / 255.0;

    // Apply the light color to the new color.
    red   *= lightRed  ;
    green *= lightGreen;
    blue  *= lightBlue ;
  }

  // Clamp the color channels to the (0..1) range.
  red   = (red   < 0.0)? 0.0: red  ;
  red   = (red   > 1.0)? 1.0: red  ;
  green = (green < 0.0)? 0.0: green;
  green = (green > 1.0)? 1.0: green;
  blue  = (blue  < 0.0)? 0.0: blue ;
  blue  = (blue  > 1.0)? 1.0: blue ;

  // Rescale the color channels to the noise::uint8 (0..255) range and return
  // the new color.
  Color newColor (
    (noise::uint8)((noise::uint)(red   * 255.0) & 0xff),
    (noise::uint8)((noise::uint)(green * 255.0) & 0xff),
    (noise::uint8)((noise::uint)(blue  * 255.0) & 0xff),
    GetMax (sourceColor.alpha, backgroundColor.alpha));
  return newColor;
}


double RendererVertex::CalcLightIntensity (double center, double left,
  double right, double down, double up) const
{
  // Recalculate the sine and cosine of the various light values if
  // necessary so it does not have to be calculated each time this method is
  // called.
  if (m_recalcLightValues) {
    m_cosAzimuth = cos (m_lightAzimuth * DEG_TO_RAD);
    m_sinAzimuth = sin (m_lightAzimuth * DEG_TO_RAD);
    m_cosElev    = cos (m_lightElev    * DEG_TO_RAD);
    m_sinElev    = sin (m_lightElev    * DEG_TO_RAD);
    m_recalcLightValues = false;
  }

  // Now do the lighting calculations.
  const double I_MAX = 1.0;
  double io = I_MAX * SQRT_2 * m_sinElev / 2.0;
  double ix = (I_MAX - io) * m_lightContrast * SQRT_2 * m_cosElev
    * m_cosAzimuth;
  double iy = (I_MAX - io) * m_lightContrast * SQRT_2 * m_cosElev
    * m_sinAzimuth;
  double intensity = (ix * (left - right) + iy * (down - up) + io);
  if (intensity < 0.0) {
    intensity = 0.0;
  }
  return intensity;
}


void RendererVertex::ClearGradient ()
{
  m_gradient.Clear ();
}


sf::VertexArray RendererVertex::Render ()
{
  int i = 0;
  sf::Color color;
  if ( m_pSourceNoiseMap == NULL
    || m_pDestImage == NULL
    || m_pSourceNoiseMap->GetWidth  () <= 0
    || m_pSourceNoiseMap->GetHeight () <= 0
    || m_gradient.GetGradientPointCount () < 2) {
    throw noise::ExceptionInvalidParam ();
  }

  int width  = m_pSourceNoiseMap->GetWidth  ();
  int height = m_pSourceNoiseMap->GetHeight ();

  // If a background image was provided, make sure it is the same size the
  // source noise map.
  if (m_pBackgroundImage != NULL) {
    if ( m_pBackgroundImage->GetWidth  () != width
      || m_pBackgroundImage->GetHeight () != height) {
      throw noise::ExceptionInvalidParam ();
    }
  }

  // Create the destination image.  It is safe to reuse it if this is also the
  // background image.
  if (m_pDestImage != m_pBackgroundImage) {
    m_pDestImage->SetSize (width, height);
  }

  for (int y = 0; y < height; y++) {
    const Color* pBackground = NULL;
    if (m_pBackgroundImage != NULL) {
      pBackground = m_pBackgroundImage->GetConstSlabPtr (y);
    }
    const float* pSource = m_pSourceNoiseMap->GetConstSlabPtr (y);
    Color* pDest = m_pDestImage->GetSlabPtr (y);
    for (int x = 0; x < width; x++) {

      // Get the color based on the value at the current point in the noise
      // map.
      Color destColor = m_gradient.GetColor (*pSource);

      // If lighting is enabled, calculate the light intensity based on the
      // rate of change at the current point in the noise map.
      double lightIntensity;
      if (m_isLightEnabled) {

        // Calculate the positions of the current point's four-neighbors.
        int xLeftOffset, xRightOffset;
        int yUpOffset  , yDownOffset ;
        if (m_isWrapEnabled) {
          if (x == 0) {
            xLeftOffset  = (int)width - 1;
            xRightOffset = 1;
          } else if (x == (int)width - 1) {
            xLeftOffset  = -1;
            xRightOffset = -((int)width - 1);
          } else {
            xLeftOffset  = -1;
            xRightOffset = 1;
          }
          if (y == 0) {
            yDownOffset = (int)height - 1;
            yUpOffset   = 1;
          } else if (y == (int)height - 1) {
            yDownOffset = -1;
            yUpOffset   = -((int)height - 1);
          } else {
            yDownOffset = -1;
            yUpOffset   = 1;
          }
        } else {
          if (x == 0) {
            xLeftOffset  = 0;
            xRightOffset = 1;
          } else if (x == (int)width - 1) {
            xLeftOffset  = -1;
            xRightOffset = 0;
          } else {
            xLeftOffset  = -1;
            xRightOffset = 1;
          }
          if (y == 0) {
            yDownOffset = 0;
            yUpOffset   = 1;
          } else if (y == (int)height - 1) {
            yDownOffset = -1;
            yUpOffset   = 0;
          } else {
            yDownOffset = -1;
            yUpOffset   = 1;
          }
        }
        yDownOffset *= m_pSourceNoiseMap->GetStride ();
        yUpOffset   *= m_pSourceNoiseMap->GetStride ();

        // Get the noise value of the current point in the source noise map
        // and the noise values of its four-neighbors.
        double nc = (double)(*pSource);
        double nl = (double)(*(pSource + xLeftOffset ));
        double nr = (double)(*(pSource + xRightOffset));
        double nd = (double)(*(pSource + yDownOffset ));
        double nu = (double)(*(pSource + yUpOffset   ));

        // Now we can calculate the lighting intensity.
        lightIntensity = CalcLightIntensity (nc, nl, nr, nd, nu);
        lightIntensity *= m_lightBrightness;

      } else {

        // These values will apply no lighting to the destination image.
        lightIntensity = 1.0;
      }

      // Get the current background color from the background image.
      Color backgroundColor (255, 255, 255, 255);
      if (m_pBackgroundImage != NULL) {
        backgroundColor = *pBackground;
      }

      // Blend the destination color, background color, and the light
      // intensity together, then update the destination image with that
      // color.
      *pDest = CalcDestColor (destColor, backgroundColor, lightIntensity);

      //Fill the vertex array
      m_pDestVertexArray[i].position = sf::Vector2f(x, y);
      color.r = pDest->red;
      color.g = pDest->green;
      color.b = pDest->blue;
      m_pDestVertexArray[i].color = color;

      // Go to the next point.
      ++pSource;
      ++pDest;
      ++i;
      if (m_pBackgroundImage != NULL) {
        ++pBackground;
      }
    }
  }
  return m_pDestVertexArray;
}
