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

void PCB::set_ir_image(const char* image_path, SDL_Renderer* renderer){
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
    this->ir_dimensions.first = desired_width; 
    this->ir_dimensions.second = desired_height; 
    this->ir_image = scaledTexture;
}

SDL_Texture* PCB::get_rgb_image() const{
    return this->rgb_image;
}

SDL_Texture* PCB::get_ir_image() const{
    return this->ir_image;
}

std::pair<int,int> PCB::get_rgb_dimensions() const{
    return this->rgb_dimensions;
}

std::pair<int,int> PCB::get_ir_dimensions() const{
    return this->ir_dimensions;
}

std::unordered_map<const char*, std::vector<Component>> PCB::get_components() const{
    return this->components;
}

void PCB::set_components(SDL_Renderer* renderer){
    // en la funcion final, primero deberiamos crear el componente y pushear solo cuando tenga
    // todas sus caracteristicas
    this->components["capacitor_misplaced"].push_back(Component("C1"));
    this->components["capacitor_misplaced"][0].set_rgb_image("/home/sebastian_pulido/pcb_scan/tests/c1_rgb.jpg",renderer);
    this->components["uC_misplaced"].push_back(Component("uC1"));
    this->components["uC_misplaced"][0].set_rgb_image("/home/sebastian_pulido/pcb_scan/tests/uC1_rgb.jpg",renderer);
}