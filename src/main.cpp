#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

/* Nuestreos headers*/
#include "main_menu.h"
#include "PCB.h"
#include "Component.h"

/* Variables globales y de control*/
bool inspection_finished = false; // flag que comienza la creacion de los menus y las ventanas
// idx_pcb puede ser una variable global que se modifique en el tab de components con el drop-down menu
// se puede usar aqui para escoger la imagen de la pcb a elegir 
size_t item_selected_idx  = 0; 
// En main estara el vector de todas las pcbs analizadas ------->  std::vector<PCB> pcbs
// las imagenes se podran accesar con ------> pcbs[idx_pcb].get_image



int main(int, char**)
{
    /*-------------------------------- Initialization -------------------------------*/
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Scorch Finder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // Inicializar SDL_image y cargar imagen
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) 
    {
        printf("Error: %s\n", IMG_GetError());
        return -1;
    }
    /*-------------------------------- Initialization -------------------------------*/

    /*---------------------------------- Load Images --------------------------------*/
    // TODO: Implement function to load images from a folder
    // Ask the structure of the data that the visualizer will receive (components location)
    // There might be several images in the folder, so a function in the menu where we 
    // can change the image in display might be necessary.

    SDL_Surface* image_surface = IMG_Load("/home/sebastian_pulido/pcb_scan/tests/pcb_1.png");
    if (!image_surface) 
    {
        printf("Error loading image: %s\n", IMG_GetError());
        return -1;
    }
    
    // Getting original image dimentions to size the window 
    int image_width = image_surface->w;
    int image_height = image_surface->h;

    SDL_Texture* image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);
    /*---------------------------------- Load Images --------------------------------*/

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    bool show_image_window = true;
    bool done = false;

    // Main loop
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        /* PCB image window*/
        // set default size for image window, like ImVec2(600,600)
        ImGui::SetNextWindowSize(ImVec2(image_width+30, image_height+30));
        if (show_image_window) {
            ImGui::Begin("PCB", &show_image_window);
            //SDL_Texture* current_pcb_image = pcbs[idx_pcb].get_image();
            //int image_width = current_pcb_image.
            //ImGui::Image((ImTextureID)(intptr_t) pcbs[idx_pcb].get_image(), ImVec2(image_width, image_height));
            ImGui::Image((ImTextureID)(intptr_t)image_texture, ImVec2(image_width, image_height)); 
            if(ImGui::Button("Mostrar menu")) inspection_finished = !inspection_finished;
            ImGui::Text("The number is: %lu",item_selected_idx);
            ImGui::End();
        }
        ImGui::ShowDemoWindow();
        create_main_menu(inspection_finished);
        
        




        /* Rendering */

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 255, 144, 154, 255); // This changes the background color
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(),renderer);
        SDL_RenderPresent(renderer);
    }

    /* Clean up */
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture(image_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
