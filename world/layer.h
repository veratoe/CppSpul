#ifndef LAYER_H
#define LAYER_H

#include <SFML/Graphics.hpp>
#include <vector>

class Layer {

    static std::vector< Layer* > layers;

    public:
    
        Layer();
        void draw(sf::RenderTarget& target);
        void fade();


        static void drawLayers(sf::RenderTarget& target);
        



        bool m_visible = true;

        sf::RenderTexture m_texture;


    private:


};

#endif // LAYER_H
