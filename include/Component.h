#ifndef COMPONENT_H
#define COMPONENT_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Component{
    private:
        std::string label; // const char*
        double confidence;
        //SDL_Rect* box;
        //std::pair<int,int> position;
        //std::pair<int,int> width_height;
        SDL_Texture* rgb_image;
        SDL_Texture* ir_image;
        std::pair<int,int> rgb_dimensions;
        std::pair<int,int> ir_dimensions;

    
    public:
        Component(std::string label, double confidence, const char* rgb_root, const char* ir_root, SDL_Renderer* renderer);
        std::string get_label() const;
        void set_rgb_image(const char* image_path, SDL_Renderer* renderer);
        void set_ir_image(const char* image_path, SDL_Renderer* renderer);
        SDL_Texture* get_rgb_image() const;
        SDL_Texture* get_ir_image() const;
        std::pair<int,int> get_rgb_dimensions() const;
        std::pair<int,int> get_ir_dimensions() const;


};

#endif