#include "main_menu.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>



/* El booleano enable nos dice que ya acabo el proceso de inspeccion y podemos llenar el menu con los datos
   Tambien debera recibir una referencia al vector de objetos de la clase pcb que se creara en main 
   std::vector<PCB> &pcbs
*/
void create_main_menu(bool enable, const std::vector<PCB>& pcbs_vector){
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
        
        ImGui::SetNextWindowPos(ImVec2(700, 30));
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

            extern std::size_t item_selected_idx;
            const char* combo_preview_value = pcbs_vector[item_selected_idx].get_id().c_str();
            
    
            if (ImGui::BeginTabBar("Main Menu")){

                // COMPONENTS
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x / 3);
                if (ImGui::BeginTabItem("Components"))
                {
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::BeginCombo("##combo", combo_preview_value, 0))
                    {
                        for (std::size_t n = 0; n < pcbs_vector.size(); n++)
                        {
                            const bool is_selected = (item_selected_idx == n); // creacion dinamica de booleanos
                            if (ImGui::Selectable(pcbs_vector[n].get_id().c_str(), is_selected)){
                                item_selected_idx = n;
                            }   

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    fill_components_tab(pcbs_vector);
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
                ImGui::Text("Hello guys");
            }

        }

        ImGui::End();

}
/* Puede que esta funcion requiera como argumento la window de PCBs paraa dibujar la bounding box*/
void fill_components_tab(const std::vector<PCB>& pcbs){
    extern std::size_t item_selected_idx;
    /* Creamos un mapa de componentes con sus respectivos booleanos para mostrar su imagen y bounding box, asi 
        ya no hay problema con tener el mismo booleano para diferentes componentes y sus respectivos checkboxs
     */
    static std::unordered_map<std::string, std::pair<bool,bool>> component_states;

    for (auto & item : pcbs[item_selected_idx].get_components()){ /* por cada categoria creamos un header colapsable*/
        const char* label = item.first;
        if(ImGui::CollapsingHeader(label,ImGuiTreeNodeFlags_None)){ /* dentro del collapsing header ponemos las instancias*/
            for (auto & comp : item.second){
                /* unique_id es el id de la pcb mas el label del componente*/
                std::string unique_id = pcbs[item_selected_idx].get_id() + "_" + std::string(comp.get_label());
                if (component_states.find(unique_id) == component_states.end()) {
                    component_states[unique_id] = {false, false};
                }

                bool& show_image = component_states[unique_id].first;
                bool& show_box = component_states[unique_id].second;
                // si hay componentes con el mismo label en otra pcb no hay conflicto porque no se muestran al mismo tiempo
                std::string im =  std::string(comp.get_label()) + " show image"; 
                std::string bx =  std::string(comp.get_label()) + " show box";

                ImGui::Checkbox(im.c_str(),&show_image); 
                ImGui::SameLine();
                ImGui::Checkbox(bx.c_str(), &show_box);

                int image_width = comp.get_rgb_dimensions().first;
                int image_height = comp.get_rgb_dimensions().second;

                if (show_image){
                    ImGui::SetNextWindowSize(ImVec2(image_width+30, image_height+30));
                    ImGui::Begin(comp.get_label(), &show_image);
                    ImGui::Image((ImTextureID)(intptr_t)comp.get_rgb_image(), ImVec2(image_width, image_height)); 
                    ImGui::End();               
                }

                if (show_box){
                    
                    extern ImVec2 pcb_window_position;
                    extern ImDrawList* pcb_window_draw_list;
                    extern std::unordered_map<std::string,std::pair<float,float>> components_dimensions;
                    extern std::unordered_map<std::string,std::pair<float,float>> components_centers;

                    /* faltaria agregar manejo de errores, por si se introduce un label incorrecto*/
                    //std::cout<<"Center: "<<components_centers[comp.get_label()].first<<", "<<components_centers[comp.get_label()].second<<std::endl;
                    //std::cout<<"Dimensions: "<<components_dimensions[comp.get_label()].first<<", "<<components_dimensions[comp.get_label()].second<<std::endl;

                    std::string comp_label = std::string(comp.get_label());
                    float x = pcb_window_position.x + components_centers.at(comp_label).first - components_dimensions.at(comp_label).first / 2;
                    float y = pcb_window_position.y + components_centers.at(comp_label).second - components_dimensions.at(comp_label).second / 2;
                    
                    ImVec2 rect_start(x, y); // Esquina superior izquierda
                    ImVec2 rect_end(x + components_dimensions.at(comp_label).first, y + components_dimensions.at(comp_label).second); // Esquina inferior derecha
                    pcb_window_draw_list->AddRect(rect_start, rect_end, IM_COL32(255, 0, 0, 255), 5.0f,0,3.0f); 


                }

            }
        }

    }
}

void free_textures(std::vector<PCB>& pcbs_vector){
    for (auto pcb : pcbs_vector){
        SDL_DestroyTexture(pcb.get_rgb_image());
        // SDL_DestroyTexture(pcb.get_ir_image()); NOT CALLING RIGHT NOW CUZ PASSING NULL POINTER TRIGGERS AN ERROR
        for (auto components : pcb.get_components()){
            for (auto component : components.second){
                SDL_DestroyTexture(component.get_rgb_image());
                // SDL_DestroyTexture(component.get_ir_image()); NOT CALLING RIGHT NOW CUZ PASSING NULL POINTER TRIGGERS AN ERROR
            }
        }
    }
}

void fill_sumary_tab(){
    
}