#include <vita2d.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/power.h> 
#include <psp2/io/fcntl.h>
#include <string>

#include "main.hpp"
#include "net/download.hpp"
#include "utils/search.hpp"
#include "utils/format.hpp"

#include "screens/list.hpp"
#include "screens/details.hpp"
#include "screens/popup.hpp"

SceCtrlData pad;

int main() {
    vita2d_init();
    vita2d_set_clear_color(RGBA8(255,255,255,255));

    vita2d_texture *bgIMG = vita2d_load_PNG_file("ux0:app/ESPL00009/resources/bg.png");

    // netInit();
    // httpInit();
    // curlDownload("http://rinnegatamante.it/vitadb/list_plugins_json.php", "ux0:json.json");
    // httpTerm();
    // netTerm();

    SharedData sharedData;
    
    ifstream plp("ux0:data/Easy_Plugins/plugins.json");
    plp >> sharedData.plugins;
    plp.close();

    sharedData.original = sharedData.plugins;

    ifstream blb(sharedData.taiConfigPath+"config.txt");
    blb >> sharedData.taiConfig;
    blb.close();

    if(doesDirExist("ux0:tai")) sharedData.taiConfigPath = "ux0:tai/";
    else if(doesDirExist("ur0:tai")) sharedData.taiConfigPath = "ur0:tai/";
    else return 0;

    List listView;
    Popup popupView;
    Details detailsView;

    while(1) {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        vita2d_start_drawing();
        vita2d_clear_screen();

        vita2d_draw_texture(bgIMG, 0, 0);

        if(pad.buttons != SCE_CTRL_CROSS) sharedData.blockCross = false;
        
        if(sharedData.scene == 0) listView.draw(sharedData, pad.buttons);
        
        if(sharedData.scene == 1) detailsView.draw(sharedData, pad.buttons);

        if(sharedData.scene == 2) popupView.draw(sharedData, pad.buttons);

        vita2d_end_drawing();
        vita2d_swap_buffers();

        if(pad.buttons == SCE_CTRL_SELECT) {
            break;
        }
        if(pad.buttons == SCE_CTRL_START) {
            scePowerRequestColdReset();
        }
    }

    vita2d_free_font(sharedData.font);
    vita2d_free_texture(bgIMG);
    listView.free();
    detailsView.free();
    vita2d_fini();
    
    sceKernelExitProcess(0);
    return 0;
}
