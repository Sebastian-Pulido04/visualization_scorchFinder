#ifndef PCB_H
#define PCB_H

#include <string>
#include <Component.h>
#include <unordered_map>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class PCB{
    private:
        std::string id;
        std::unordered_map<std::string,std::vector<Component>> components;
        // RGB image
        SDL_Texture* rgb_image;
        // Heat image 
        SDL_Texture* ir_image;
        // image width and height
        std::pair<int,int> rgb_dimensions;
        std::pair<int,int> ir_dimensions;

    public:
        void show_info();
        void set_rgb_image(const char* image_path, SDL_Renderer* renderer);
        SDL_Texture* get_rgb_image();
        void set_ir_image(const char* image_path, SDL_Renderer* renderer);
        SDL_Texture* get_ir_image();
        void set_rgb_dimensions(int width, int height);
        std::pair<int,int> get_rgb_dimensions();


};

#endif