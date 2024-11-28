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
    // image_surface->h es el alto original de la imagen
    // image_surface->w es el ancho original de la imagen

    SDL_Texture* original_image = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
    int desired_width = 600;
    int desired_height = 600;
    SDL_Texture* scaledTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, desired_width, desired_height);
    SDL_SetRenderTarget(renderer, scaledTexture);
    SDL_Rect destRect = {0, 0, desired_width, desired_height};
    SDL_RenderCopy(renderer, original_image, NULL, &destRect);

    // Restaurar el render target al predeterminado (pantalla)
    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyTexture(original_image);

    // aqui determinamos las dimensiones de la imagen escalada
    this->rgb_dimensions.first = desired_width; 
    this->rgb_dimensions.second = desired_height; 
    this->rgb_image = scaledTexture;
}

std::pair<int,int> Component::get_rgb_dimensions() const{
    return this->rgb_dimensions;
}