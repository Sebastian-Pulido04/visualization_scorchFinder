#include "main_menu.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <vector>


/* El booleano enable nos dice que ya acabo el proceso de inspeccion y podemos llenar el menu con los datos
   Tambien debera recibir una referencia al vector de objetos de la clase pcb que se creara en main 
   std::vector<PCB> &pcbs
*/
void create_main_menu(bool enable ){
        // para cada tab, podemos crear una child window

        // tab styling
        ImGuiStyle& style = ImGui::GetStyle();
        style.TabRounding = 0.0f;
        //style.TabBarBorderSize = 0;
        style.WindowPadding = ImVec2(0, 0); // Cambiar el padding
        style.ItemSpacing = ImVec2(5, 5);   // Cambiar el espaciado entre items
        style.ItemInnerSpacing = ImVec2(0, 4); // Cambiar el espaciado interno
        style.TabBorderSize = 0.8f;
        
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoResize;
        
        ImGui::SetNextWindowPos(ImVec2(650, 30));
        ImGui::SetNextWindowSize(ImVec2(550,680));
        if(ImGui::Begin("Main Menu",NULL, window_flags)){
            
            /* Creacion de los botones del menu principal
            TODO: modificar el size de los tab_items y la posicion xx
                  crear los child windows con el contenido de cada tab 
                  crear las funciones para llenar (or retrieve) el contenido de cada tab
                  como voy a hacer que se creen nuevas instancias de la clase Pcb cuando se corra el proceso?
                  la solucion mas sencilla es que la aplicacion solo sea un visualizador, es decir, no comenzara 
                  el proceso de inspeccion, solo se ejecutara una vez el proceso de inspeccion haya terminado y el 
                  programa pueda obtener los datos que necesita de las carpetas
                  
            */

            /* Aqui van a ir los labels de las PCBs, probablemente haya que pasarle a la funcion la referencia al vector 
               de pcbs (de la clase PCB), y acceder a los nombres con  pcbs[n].get_id()
             */

            std::vector<const char*> pcbs = {"PCB_0", "PCB_1", "PCB_2"};
            extern std::size_t item_selected_idx;
            const char* combo_preview_value = pcbs[item_selected_idx];
            
    
            if (ImGui::BeginTabBar("Main Menu")){

                // COMPONENTS
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 3);
                if (ImGui::BeginTabItem("Components"))
                {
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::BeginCombo("##combo", combo_preview_value, 0))
                    {
                        for (std::size_t n = 0; n < pcbs.size(); n++)
                        {
                            const bool is_selected = (item_selected_idx == n); // creacion dinamica de booleanos
                            if (ImGui::Selectable(pcbs[n], is_selected)){
                                item_selected_idx = n;
                                
                            }   

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    // pcbs[item_selected_idx].show_information();
                    ImGui::Text("Ítem seleccionado: %lu", item_selected_idx); 
                    ImGui::Text("COMPONENTES"); // llamar a la funcion de components()
                    ImGui::EndTabItem();
                }

                // HEATMAP
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 3);
                if (ImGui::BeginTabItem("Heatmap"))
                {
                    ImGui::Text("FUEGO CORRAN FUEGO"); // llamar a la funcion de Heatmap()
                    ImGui::EndTabItem();
                }

                // SUMMARY
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 3);
                if (ImGui::BeginTabItem("Summary"))
                {
                    ImGui::Text("RESUMEN");     // llamar a la funcion de summary()
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar(); // Finaliza el contenedor de tabs
            }

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
