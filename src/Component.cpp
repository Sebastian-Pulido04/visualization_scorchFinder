#include "Component.h"

Component::Component(const char* label){
    this->label = label;
}

const char* Component::get_label() const{
    return this->label;
}

SDL_Texture* Component::get_rgb_image() const{
    return this->rgb_image;
}

void Component::set_rgb_image(const char* image_path, SDL_Renderer* renderer){
    SDL_Surface* image_surface = IMG_Load(image_path);
    if (!image_surface) 
    {
        printf("Error loading image: %s\n", IMG_GetError());
        //return -1;
    }
    // Getting original image dimentions to size the window 
    this->rgb_dimensions.first = image_surface->w;
    this->rgb_dimensions.second = image_surface->h;

    this->rgb_image = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
}

std::pair<int,int> Component::get_rgb_dimensions() const{
    return this->rgb_dimensions;
}