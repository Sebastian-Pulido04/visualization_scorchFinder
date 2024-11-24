#include "PCB.h"

std::string PCB::get_id() const{
    return this->id;
}

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

SDL_Texture* PCB::get_rgb_image() const{
    return this->rgb_image;
}

std::pair<int,int> PCB::get_rgb_dimensions() const{
    return this->rgb_dimensions;
}

std::unordered_map<const char*, std::vector<Component>> PCB::get_components() const{
    return this->components;
}

void PCB::set_components(SDL_Renderer* renderer){
    // en la funcion final, primero deberiamos crear el componente y pushear solo cuando tenga
    // todas sus caracteristicas
    this->components["capacitor_misplaced"].push_back(Component("capacitor_misplaced_00"));
    this->components["capacitor_misplaced"][0].set_rgb_image("/home/sebastian_pulido/pcb_scan/tests/c1_rgb.jpg",renderer);
    this->components["uC_misplaced"].push_back(Component("uC_misplaced_00"));
    this->components["uC_misplaced"][0].set_rgb_image("/home/sebastian_pulido/pcb_scan/tests/uC1_rgb.jpg",renderer);
}