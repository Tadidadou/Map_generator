#include <SFML/Graphics.hpp>
#include "noiseutils.h"

using namespace noise::utils;

class RendererVertex {
public:
    RendererVertex (sf::VertexArray destVertexArray);

    void AddGradientPoint (double gradientPos, const Color& gradientColor);
    void BuildGrayscaleGradient ();
    void BuildTerrainGradient ();
    void ClearGradient ();

    void EnableLight (bool enable = true) {
      m_isLightEnabled = enable;
    }

    void EnableWrap (bool enable = true) {
      m_isWrapEnabled = enable;
    }

    noise::utils::GradientColor GetGradient () const {
        return m_gradient;
    }

    double GetLightAzimuth () const {
      return m_lightAzimuth;
    }

    double GetLightBrightness () const {
      return m_lightBrightness;
    }

    noise::utils::Color GetLightColor () const {
      return m_lightColor;
    }

    double GetLightContrast () const {
      return m_lightContrast;
    }

    double GetLightElev () const {
      return m_lightElev;
    }

    double GetLightIntensity () const {
      return m_lightIntensity;
    }

    sf::VertexArray GetDestVertexArray() const {
      return m_pDestVertexArray;
    }

    bool IsLightEnabled () const {
      return m_isLightEnabled;
    }

    bool IsWrapEnabled () const {
      return m_isWrapEnabled;
    }

    sf::VertexArray Render ();

    void SetBackgroundImage (const noise::utils::Image& backgroundImage) {
      m_pBackgroundImage = &backgroundImage;
    }

    void SetDestImage (noise::utils::Image& destImage) {
      m_pDestImage = &destImage;
    }

    void SetLightAzimuth (double lightAzimuth) {
      m_lightAzimuth = lightAzimuth;
      m_recalcLightValues = true;
    }

    void SetLightBrightness (double lightBrightness) {
      m_lightBrightness = lightBrightness;
      m_recalcLightValues = true;
    }

    void SetLightColor (const noise::utils::Color& lightColor) {
      m_lightColor = lightColor;
    }

    void SetLightContrast (double lightContrast) {
      if (lightContrast <= 0.0) {
        throw noise::ExceptionInvalidParam ();
      }

      m_lightContrast = lightContrast;
      m_recalcLightValues = true;
    }

    void SetLightElev (double lightElev) {
      m_lightElev = lightElev;
      m_recalcLightValues = true;
    }

    void SetLightIntensity (double lightIntensity) {
      if (lightIntensity < 0.0) {
        throw noise::ExceptionInvalidParam ();
      }

      m_lightIntensity = lightIntensity;
      m_recalcLightValues = true;
    }

    void SetSourceNoiseMap (const noise::utils::NoiseMap& sourceNoiseMap) {
      m_pSourceNoiseMap = &sourceNoiseMap;
    }

private:
    noise::utils::Color CalcDestColor (const Color& sourceColor, const Color& backgroundColor, double lightValue) const;

    double CalcLightIntensity (double center, double left, double right,
      double down, double up) const;

    mutable double m_cosAzimuth;
    mutable double m_cosElev;
    noise::utils::GradientColor m_gradient;
    bool m_isLightEnabled;
    bool m_isWrapEnabled;
    double m_lightAzimuth;
    double m_lightBrightness;
    noise::utils::Color m_lightColor;
    double m_lightContrast;
    double m_lightElev;
    double m_lightIntensity;
    const Image* m_pBackgroundImage;
    Image* m_pDestImage;
    sf::VertexArray m_pDestVertexArray;
    const NoiseMap* m_pSourceNoiseMap;
    mutable bool m_recalcLightValues;
    mutable double m_sinAzimuth;
    mutable double m_sinElev;
};
