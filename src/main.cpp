#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <vector>
#include <iostream>

/* Nuestreos headers*/
#include "main_menu.h"
#include "PCB.h"
#include "Component.h"

/* Variables globales y de control*/
// flag que comienza la creacion de los menus y las ventanas
bool inspection_finished = false; 
// idx_pcb puede ser una variable global que se modifique en el tab de components con el drop-down menu
// se puede usar aqui para escoger la imagen de la pcb a elegir 
size_t item_selected_idx  = 0; 
/* variable para poder dibujar las bounding boxes de los componentes de la pcbs*/
ImVec2 pcb_window_position;
ImDrawList* pcb_window_draw_list;
// En main estara el vector de todas las pcbs analizadas ------->  std::vector<PCB> pcbs
// las imagenes se podran accesar con ------> pcbs[idx_pcb].get_image
std::vector<PCB> pcbs;
std::unordered_map<std::string,std::pair<float,float>> components_dimensions;
std::unordered_map<std::string,std::pair<float,float>> components_centers;
/* NO SE PUEDE CORRER PUSH_BACK EN EL SCOPE GLOBAL */
// hay que hacer una funcion que limpie todas las sdl textures de las imagenes en la parte de clean up




int main(int, char**)
{
    /*-------------------------------- Initialization -------------------------------*/
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        printf("Error al obtener el modo de pantalla: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    int screenWidth = displayMode.w;
    
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Scorch Finder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, 720, window_flags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    // Inicializar SDL_image y cargar imagen
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) 
    {
        printf("Error: %s\n", IMG_GetError());
        return -1;
    }
    /*-------------------------------- Initialization -------------------------------*/

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    //bool show_image_window = true;
    bool done = false;

    // resolucion de la imagen. Considerar que habra un escalamiento para reducir las dimensiones.
    std::pair<int,int> rgb_resolution = {600,600};
    // medidas de la pcb (cuadrada) en milesimas de pulgada
    float pcb_side = 1732.28;
    // factor de escalamiento en pixeles por milesima de pulgada
    std::pair <float,float> scaling_factor = {rgb_resolution.first / pcb_side, rgb_resolution.second / pcb_side};
    /* mapa de ubicaciones de los componentes. Hay que restar resolution.y - posicion escalada porque el datasheet de 
    conectia mide de abajo hacia arriba*/
    components_centers = 
    {
        {"LDO1", {(300 * scaling_factor.first),rgb_resolution.second - (1240 * scaling_factor.second)}}, // regulador
        {"uC1", {(420 * scaling_factor.first),rgb_resolution.second - (320 * scaling_factor.second)}},
        {"R4", {(615 * scaling_factor.first),rgb_resolution.second - (355 * scaling_factor.second)}},
        {"R3", {(480 * scaling_factor.first),rgb_resolution.second - (1430 * scaling_factor.second)}},
        {"R2", {(475 * scaling_factor.first),rgb_resolution.second - (1520 * scaling_factor.second)}},
        {"R1", {(225 * scaling_factor.first),rgb_resolution.second - (340 * scaling_factor.second)}},
        {"Q2", {(670 * scaling_factor.first),rgb_resolution.second - (700.05 * scaling_factor.second)}},
        {"Q1", {(105 * scaling_factor.first),rgb_resolution.second - (340 * scaling_factor.second)}},
        {"D2", {(670 * scaling_factor.first),rgb_resolution.second - (1025.28 * scaling_factor.second)}},
        {"D1", {(305 * scaling_factor.first),rgb_resolution.second - (1500 * scaling_factor.second)}}, 
        {"C3", {(100 * scaling_factor.first),rgb_resolution.second - (1320.36 * scaling_factor.second)}},
        {"C2", {(550 * scaling_factor.first),rgb_resolution.second - (1300.71 * scaling_factor.second)}},
        {"C1", {(365 * scaling_factor.first),rgb_resolution.second - (482 * scaling_factor.second)}} 
        
    };
    /* dimensiones de los componentes en pixeles. component_dimensions.first = width, "".second = height*/
    components_dimensions = 
    {
        {"LDO1", {(314.96 * scaling_factor.first),(236.22 * scaling_factor.second)}}, // regulador
        {"uC1", {(314.96 * scaling_factor.first),(236.22 * scaling_factor.second)}},
        {"R4", {(118.11 * scaling_factor.first),(196.85 * scaling_factor.second)}},
        {"R3", {(196.85 * scaling_factor.first),(118.11 * scaling_factor.second)}},
        {"R2", {(196.85 * scaling_factor.first),(118.11 * scaling_factor.second)}},
        {"R1", {(118.11 * scaling_factor.first),(196.85 * scaling_factor.second)}},
        {"Q2", {(162.48 * scaling_factor.first),(170.48 * scaling_factor.second)}}, // ok
        {"Q1", {(157.48 * scaling_factor.first),(170.48 * scaling_factor.second)}},
        {"D2", {(157.48 * scaling_factor.first),(236.22 * scaling_factor.second)}},
        {"D1", {(157.48 * scaling_factor.first),(236.22 * scaling_factor.second)}},
        {"C3", {(118.11 * scaling_factor.second),(196.85 * scaling_factor.first)}},
        {"C2", {(196.85 * scaling_factor.first),(118.11 * scaling_factor.second)}},
        {"C1", {(196.85 * scaling_factor.first),(118.11 * scaling_factor.second)}}
    };

    /* Aqui va el llamado a fill_data(), que lee el JSON para todas las pcbs, y usa los datos de component centers
    para llenar los atributos de los componentes de las pcbs*/
    /* pcbs.push_back(PCB("00"));
    pcbs.push_back(PCB("01"));
    pcbs[0].set_rgb_image("/home/sebastian_pulido/pcb_scan/tests/pcb_og.jpg",renderer);
    pcbs[1].set_rgb_image("/home/sebastian_pulido/pcb_scan/tests/pcb_2.png",renderer);
    pcbs[0].set_ir_image("/home/sebastian_pulido/pcb_scan/tests/heatmap_pcb1.png",renderer);
    pcbs[1].set_ir_image("/home/sebastian_pulido/pcb_scan/tests/heatmap_pcb2.png",renderer);
    pcbs[0].set_components(renderer);
    pcbs[1].set_components(renderer); */

    read_json("/home/sebastian_pulido/vis2/visualization_scorchFinder/tests/inspection_data.json", pcbs, renderer);

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
        int image_width = pcbs[item_selected_idx].get_rgb_dimensions().first;
        int image_height = pcbs[item_selected_idx].get_rgb_dimensions().second;

        int ir_image_width = pcbs[item_selected_idx].get_ir_dimensions().first;
        int ir_image_height = pcbs[item_selected_idx].get_ir_dimensions().second;

        ImGui::SetNextWindowSize(ImVec2(image_width + 20, image_height + 20));
        
        ImGui::Begin("PCB", NULL);
        ImGui::Image((ImTextureID)(intptr_t)pcbs[item_selected_idx].get_rgb_image(), ImVec2(image_width, image_height)); 
        //if(ImGui::Button("Mostrar menu")) inspection_finished = !inspection_finished;
        //ImGui::Text("The number is: %lu",item_selected_idx);
        pcb_window_position = ImGui::GetWindowPos();
        pcb_window_draw_list = ImGui::GetWindowDrawList();
        ImGui::End();
        

        ImGui::SetNextWindowSize(ImVec2(ir_image_width + 20, ir_image_height + 20));
        ImGui::Begin("PCB_Heatmap", NULL);
        ImGui::Image((ImTextureID)(intptr_t)pcbs[item_selected_idx].get_ir_image(), ImVec2(ir_image_width, ir_image_height)); 
        ImGui::End();

        //ImGui::ShowDemoWindow();
        create_main_menu(inspection_finished, pcbs);
        
        
        




        /* Rendering */

        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 46, 71, 102, 255); // This changes the background color
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(),renderer);
        SDL_RenderPresent(renderer);
    }

    /* Clean up */

    free_textures(pcbs);
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
