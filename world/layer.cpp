#include "layer.h"
#include <vector>

std::vector< Layer* >* Layer::layers;

Layer::Layer() {
    m_texture.create(3200, 3200);
    if (layers == NULL) {
        layers = new std::vector< Layer* >();

    }
    layers->push_back(this);
    printf("Layer constructor called\n");
    //printf("%i\n", (int) layers->size());
}

Layer::Layer(const Layer& l) {
    printf("copy constructor called\n");

}

Layer::~Layer() {
}

void Layer::draw(sf::RenderTarget* target) {

    if (m_visible) {

        m_texture.display();
        //target->setView(target->getDefaultView());
        target->draw(sf::Sprite(m_texture.getTexture()));

    }

}

void Layer::drawLayers(sf::RenderTarget* target) {

    for (auto& layer : *layers) {
        layer->draw(target);
    }


}
