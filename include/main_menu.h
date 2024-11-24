#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <PCB.h>

/* Definir el menu principal, con sus 3 ventanas: componentes, heatmap e historial*/
/* El menu principal se va a crear al inicio. Cuando termine la etapa de inspeccion de la CNC, se llenara
con los datos obtenidos.
En realidad no, ya que vamos a poder acceder a los datos generados en cualquier momento a traves del tab summary
*/
void create_main_menu(bool enable, const std::vector<PCB>& pcbs_vector);
void fill_components_tab(const std::vector<PCB>& pcbs);




#endif