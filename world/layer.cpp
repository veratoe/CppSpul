#include "layer.h"
#include <vector>

std::vector< Layer* > Layer::layers;

Layer::Layer() {

    m_texture.create(3200, 3200); 
    layers.push_back(this);
    printf("nieuwe layer aangemaakt\n");
    printf("aantal lagen: %i\n", (int) layers.size());

}


void Layer::fade() {
/*
    if (rand() % 100 < 90) return;

    sf::RenderTexture copy;
    copy.create(3200, 3200);
    copy.clear({ 0, 0, 0, 0 });

    sf::Sprite sprite(m_texture.getTexture());
    sprite.setColor({ 255, 255, 255, 254});

    copy.draw(sprite);

    m_texture.clear({ 0, 0, 0, 0 });
    m_texture.draw(sf::Sprite(copy.getTexture()));
    //printf("fade!\n");

*/
}


void Layer::draw(sf::RenderTarget& target) {

    //printf("laag tekenene\n");

    if (m_visible) {

        m_texture.display();
        target.draw(sf::Sprite(m_texture.getTexture()));

    }

    fade();

    //m_texture.clear({ 0, 0, 0, 0 });

}

void Layer::drawLayers(sf::RenderTarget& target) {

    for (auto& layer : layers) {
        layer->draw(target);
    }


}
