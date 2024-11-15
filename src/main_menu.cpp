#include "main_menu.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

/* El booleano enable nos dice que ya acabo el proceso de inspeccion y podemos llenar el menu con los datos*/
void create_main_menu(bool enable){
        // necesitamos crear una variable (o tres) para saber en que tab estamos 
        // para cada tab, podemos crear una child window

        //bool components_picked = true;
        //bool summary_picked = false;
        //bool heatmap_picked = false;
        
        
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        
        ImGui::SetNextWindowPos(ImVec2(650, 30));
        ImGui::SetNextWindowSize(ImVec2(550,680));
        if(ImGui::Begin("Main Menu",NULL, window_flags)){
            
            /* Creacion de los botones del menu principal*/

            if (ImGui::BeginTabBar("Main Menu")){

                // COMPONENTS
                if (ImGui::BeginTabItem("Components"))
                {
                    ImGui::Text("COMPONENTES"); // llamar a la funcion de components()
                    ImGui::EndTabItem();
                }

                // HEATMAP
                if (ImGui::BeginTabItem("Heatmap"))
                {
                    ImGui::Text("FUEGO CORRAN FUEGO"); // llamar a la funcion de Heatmap()
                    ImGui::EndTabItem();
                }

                // SUMMARY
                if (ImGui::BeginTabItem("Summary"))
                {
                    ImGui::Text("RESUMEN");     // llamar a la funcion de summary()
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar(); // Finaliza el contenedor de tabs
            }


            //if(ImGui::Button("Components")){}
            //if(ImGui::Button("Heatmap")){}
            //if(ImGui::Button("Summary")){}



            /* El tab de summary no depende de si el proceso de inspeccion ha terminado o no, por lo que habria 
            que llenarlo independientemente de "enable". Si hay informacion nueva, hay que actualizar el tab*/

            if (enable) {
                //Aqui se llena el menu de informacion
                ImGui::Text("Hello guys");
                // call fill_info()
            }

        }

        ImGui::End();

}
