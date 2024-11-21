#include "PCB.h"

PCB::PCB(std::string label){
    this->id = "pcb_" + label;
}

void PCB::set_rgb_image(const char* image_path, SDL_Renderer* renderer){
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

SDL_Texture* PCB::get_rgb_image(){
    return this->rgb_image;
}

std::pair<int,int> PCB::get_rgb_dimensions(){
    return this->rgb_dimensions;
}
