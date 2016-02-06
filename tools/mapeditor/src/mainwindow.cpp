// generated by Fast Light User Interface Designer (fluid) version 1.0302

#include "mainwindow.hpp"
#include <cstdint>
#include <ctime>
#include <array>
#include <map>
#include <string>
#include <cstring>
#include "misc.hpp"
#include "mir2map.hpp"
#include <unordered_map>
#include "wilimagepackage.hpp"

void MainWindow::cb_m_Window_i(Fl_Double_Window*, void*) {
  exit(0);
}
void MainWindow::cb_m_Window(Fl_Double_Window* o, void* v) {
  ((MainWindow*)(o->user_data()))->cb_m_Window_i(o,v);
}

void MainWindow::cb_New_i(Fl_Menu_*, void*) {
  SetWorkingPathName();
}
void MainWindow::cb_New(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_New_i(o,v);
}

void MainWindow::cb_Load_i(Fl_Menu_*, void*) {
  {
    extern std::string g_WilFilePathName;
    if(g_WilFilePathName == ""){
        SetWilFilePathName();
    }

    Fl_Native_File_Chooser stFileChooser;
    stFileChooser.title("Set Map File Path...");
    stFileChooser.options(Fl_Native_File_Chooser::NO_OPTIONS);
    stFileChooser.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);

    stFileChooser.directory(".");

    switch(stFileChooser.show()){
        case -1:
            {
                fl_alert(stFileChooser.errmsg());
                break;
            }
        case 1:
            {
                break;
            }
        default:
            {
                extern std::string g_WorkingPathName;
                g_WorkingPathName = "";
                MakeWorkingFolder();

                std::string szPath = stFileChooser.filename();
                std::replace(szPath.begin(), szPath.end(), '\\', '/');

                if(szPath.back() == '/'){
                    szPath += "desc.bin";
                }else{
                    szPath += "/desc.bin";
                }

                if(FileExist(szPath.c_str())){
                    extern Mir2Map g_Map;
                    if(!g_Map.NewLoadMap(szPath.c_str()) || !g_Map.Valid()){
                        fl_alert("load map %s failed", szPath.c_str());
                    }
                    AfterLoadMir2Map();
                }else{
                    fl_alert("invalid map folder: %s", szPath.c_str());
                }
            }
    }
};
}
void MainWindow::cb_Load(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_Load_i(o,v);
}

void MainWindow::cb_Load1_i(Fl_Menu_*, void*) {
  {
    // maybe default or setted one, depends
    MakeWorkingFolder();

    extern std::string g_WilFilePathName;
    if(g_WilFilePathName == ""){
        SetWilFilePathName();
    }

    if(g_WilFilePathName != ""){
        LoadMir2Map();
    }
};
}
void MainWindow::cb_Load1(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_Load1_i(o,v);
}

void MainWindow::cb_Extract_i(Fl_Menu_*, void*) {
  {
    extern Mir2Map g_Map;
    if(!g_Map.Valid()){
        return;
    }

    extern std::string g_WorkingPathName;
    if(g_WorkingPathName == ""){
        return;
    }

    auto fnCheckExistFunc = [](uint32_t nFileIndex, uint32_t nImageIndex){
        extern std::string g_WorkingPathName;
        char szCachePNGName[512];
        char szIMGPNGName[512];

        if(g_WorkingPathName.back() != '/'){
            std::sprintf(szIMGPNGName, "%s/IMG/00%04d%05d.PNG", g_WorkingPathName.c_str(), nFileIndex, nImageIndex);
        }else{
            std::sprintf(szIMGPNGName, "%sIMG/00%04d%05d.PNG", g_WorkingPathName.c_str(), nFileIndex, nImageIndex);
        }

        if(FileExist(szIMGPNGName)){
            return true;
        }

        std::sprintf(szCachePNGName, "./cache/0%03d%05d.PNG", nFileIndex, nImageIndex);

        if(FileExist(szCachePNGName)){
           return MyCopyFile(szIMGPNGName, szCachePNGName);
        }

        return false;
    };

    auto fnSavePNGFunc = [this](uint32_t *pBuff, uint32_t nFileIndex, uint32_t nImageIndex, int nW, int nH, int nX, int nY){
        extern std::string g_WorkingPathName;
        char szIMGPNGName[512];

        if(g_WorkingPathName.back() != '/'){
            std::sprintf(szIMGPNGName, "%s/IMG/00%04d%05d.PNG", g_WorkingPathName.c_str(), nFileIndex, nImageIndex);
        }else{
            std::sprintf(szIMGPNGName, "%sIMG/00%04d%05d.PNG", g_WorkingPathName.c_str(), nFileIndex, nImageIndex);
        }

        if(!FileExist(szIMGPNGName)){
            SaveRGBABufferToPNG((uint8_t *)pBuff, nW, nH, szIMGPNGName);
        }
    };

    g_Map.ExtractBaseTile(fnCheckExistFunc, fnSavePNGFunc);
    g_Map.ExtractObjectTile(fnCheckExistFunc, fnSavePNGFunc);
};
}
void MainWindow::cb_Extract(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_Extract_i(o,v);
}

void MainWindow::cb_Save_i(Fl_Menu_*, void*) {
  {
    SaveMap();
};
}
void MainWindow::cb_Save(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_Save_i(o,v);
}

void MainWindow::cb_Working_i(Fl_Menu_*, void*) {
  {
    SetWorkingPathName();
};
}
void MainWindow::cb_Working(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_Working_i(o,v);
}

void MainWindow::cb_Wil_i(Fl_Menu_*, void*) {
  {
    extern std::string g_WorkingPathName;
    Fl_Native_File_Chooser stFileChooser;
    stFileChooser.title("Set Map Editing Working Path...");
    stFileChooser.options(Fl_Native_File_Chooser::NO_OPTIONS);
    stFileChooser.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);

    stFileChooser.directory(".");

    switch(stFileChooser.show()){
        case -1:
            {
                fl_alert(stFileChooser.errmsg());
                break;
            }
        case  1:
            {
                break;
            }
        default:
            {
                g_WorkingPathName = stFileChooser.filename();
                std::replace(g_WorkingPathName.begin(), g_WorkingPathName.end(), '\\', '/');
                break;
            }
    }
};
}
void MainWindow::cb_Wil(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_Wil_i(o,v);
}

void MainWindow::cb_m_MenuItemEnableEdit_i(Fl_Menu_*, void*) {
  m_MenuItemReversedGroundInfoLine->clear();
}
void MainWindow::cb_m_MenuItemEnableEdit(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_m_MenuItemEnableEdit_i(o,v);
}

void MainWindow::cb_include_i(Fl_Menu_*, void*) {
  {
    extern Mir2Map g_Map;
    if(g_Map.Valid()){
        g_Map.Optimize();
    }
};
}
void MainWindow::cb_include(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_include_i(o,v);
}

void MainWindow::cb_Crop_i(Fl_Menu_*, void*) {
  {
    extern CropConfigureWindow *g_CropConfigureWindow;
    g_CropConfigureWindow->ShowAll();
};
}
void MainWindow::cb_Crop(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_Crop_i(o,v);
}

void MainWindow::cb_Ground_i(Fl_Menu_*, void*) {
  extern GroundInfoWindow *g_GroundInfoWindow;
g_GroundInfoWindow->ShowAll();
}
void MainWindow::cb_Ground(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_Ground_i(o,v);
}

void MainWindow::cb_About_i(Fl_Menu_*, void*) {
  extern AboutWindow *g_AboutWindow;
g_AboutWindow->ShowAll();
}
void MainWindow::cb_About(Fl_Menu_* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_About_i(o,v);
}

Fl_Menu_Item MainWindow::menu_m_MenuBar[] = {
 {"Project", 0,  0, 0, 64, FL_NORMAL_LABEL, 4, 14, 0},
 {"New", 0,  (Fl_Callback*)MainWindow::cb_New, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {"Load", 0x4006f,  (Fl_Callback*)MainWindow::cb_Load, 0, 128, FL_NORMAL_LABEL, 4, 14, 0},
 {"Load MIR2EI Map", 0,  (Fl_Callback*)MainWindow::cb_Load1, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {"Extract PNG                 ", 0,  (Fl_Callback*)MainWindow::cb_Extract, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {"Save", 0x40073,  (Fl_Callback*)MainWindow::cb_Save, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {"Save As         ", 0x50073,  0, 0, 128, FL_NORMAL_LABEL, 4, 14, 0},
 {"Quit", 0x40071,  0, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Lines", 0,  0, 0, 64, FL_NORMAL_LABEL, 4, 14, 0},
 {"Base Tile", 0,  0, 0, 2, FL_NORMAL_LABEL, 4, 14, 0},
 {"Ground Object     ", 0,  0, 0, 2, FL_NORMAL_LABEL, 4, 14, 0},
 {"Over Ground Object          ", 0,  0, 0, 130, FL_NORMAL_LABEL, 4, 14, 0},
 {"Ground Info Line", 0,  0, 0, 6, FL_NORMAL_LABEL, 4, 14, 0},
 {"Reversed Ground Info Line", 0,  0, 0, 2, FL_NORMAL_LABEL, 4, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Layers", 0,  0, 0, 64, FL_NORMAL_LABEL, 4, 14, 0},
 {"Base Tile", 0,  0, 0, 6, FL_NORMAL_LABEL, 4, 14, 0},
 {"Ground Object     ", 0,  0, 0, 6, FL_NORMAL_LABEL, 4, 14, 0},
 {"Over Ground Object          ", 0,  0, 0, 6, FL_NORMAL_LABEL, 4, 14, 0},
 {"Clear Background         ", 0,  0, 0, 6, FL_NORMAL_LABEL, 4, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Setting", 0,  0, 0, 64, FL_NORMAL_LABEL, 4, 14, 0},
 {"Working Folder          ", 0,  (Fl_Callback*)MainWindow::cb_Working, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {"Wil File Folder      ", 0,  (Fl_Callback*)MainWindow::cb_Wil, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Edit", 0,  0, 0, 64, FL_NORMAL_LABEL, 4, 14, 0},
 {"Enable Edit                    ", 0,  (Fl_Callback*)MainWindow::cb_m_MenuItemEnableEdit, 0, 130, FL_NORMAL_LABEL, 4, 14, 0},
 {"Edit Ground Info", 0,  0, 0, 130, FL_NORMAL_LABEL, 4, 14, 0},
 {"Optimize", 0,  (Fl_Callback*)MainWindow::cb_include, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {"Crop Map", 0,  (Fl_Callback*)MainWindow::cb_Crop, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"Select", 0,  0, 0, 64, FL_NORMAL_LABEL, 4, 14, 0},
 {"Ground Bit Window                    ", 0,  (Fl_Callback*)MainWindow::cb_Ground, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {"About", 0,  0, 0, 64, FL_NORMAL_LABEL, 4, 14, 0},
 {"About Me          ", 0,  (Fl_Callback*)MainWindow::cb_About, 0, 0, FL_NORMAL_LABEL, 4, 14, 0},
 {0,0,0,0,0,0,0,0,0},
 {0,0,0,0,0,0,0,0,0}
};
Fl_Menu_Item* MainWindow::m_MenuItemBaseTileLine = MainWindow::menu_m_MenuBar + 10;
Fl_Menu_Item* MainWindow::m_MenuItemGroundObjectLine = MainWindow::menu_m_MenuBar + 11;
Fl_Menu_Item* MainWindow::m_MenuItemOverGroundObjectLine = MainWindow::menu_m_MenuBar + 12;
Fl_Menu_Item* MainWindow::m_MenuItemGroundInfoLine = MainWindow::menu_m_MenuBar + 13;
Fl_Menu_Item* MainWindow::m_MenuItemReversedGroundInfoLine = MainWindow::menu_m_MenuBar + 14;
Fl_Menu_Item* MainWindow::m_MenuItemBaseTileLayer = MainWindow::menu_m_MenuBar + 17;
Fl_Menu_Item* MainWindow::m_MenuItemGroundObjectLayer = MainWindow::menu_m_MenuBar + 18;
Fl_Menu_Item* MainWindow::m_MenuItemOverGroundObjectLayer = MainWindow::menu_m_MenuBar + 19;
Fl_Menu_Item* MainWindow::m_MenuItemClearBackground = MainWindow::menu_m_MenuBar + 20;
Fl_Menu_Item* MainWindow::m_MenuItemEnableEdit = MainWindow::menu_m_MenuBar + 27;
Fl_Menu_Item* MainWindow::m_MenuItemEditGround = MainWindow::menu_m_MenuBar + 28;

void MainWindow::cb_m_VSBar_i(Fl_Scrollbar*, void*) {
  auto pWidget = ((Fl_Valuator *)(m_VSBar)); 
auto fValue  = pWidget->value();

extern Mir2Map g_Map;
if(g_Map.Valid()){
	int nValidYOffset = g_Map.Height() * 32 - m_DrawArea->h();
	nValidYOffset = (std::max)(0, nValidYOffset);
	int nYOffset = (int)(nValidYOffset * fValue);
	m_DrawArea->SetYOffset(nYOffset);
	m_Window->redraw();
};
}
void MainWindow::cb_m_VSBar(Fl_Scrollbar* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_m_VSBar_i(o,v);
}

void MainWindow::cb_m_SBar_i(Fl_Scrollbar*, void*) {
  auto pWidget = ((Fl_Valuator *)(m_SBar)); 
auto fValue  = pWidget->value();

extern Mir2Map g_Map;
if(g_Map.Valid()){
	int nValidXOffset = g_Map.Width() * 48 - m_DrawArea->w();
	nValidXOffset = (std::max)(0, nValidXOffset);
	int nXOffset = (int)(nValidXOffset * fValue);
	m_DrawArea->SetXOffset(nXOffset);
	m_Window->redraw();
};
}
void MainWindow::cb_m_SBar(Fl_Scrollbar* o, void* v) {
  ((MainWindow*)(o->parent()->user_data()))->cb_m_SBar_i(o,v);
}

MainWindow::MainWindow() {
  { m_Window = new Fl_Double_Window(985, 665);
    m_Window->callback((Fl_Callback*)cb_m_Window, (void*)(this));
    { m_DrawArea = new DrawArea(0, 20, 965, 626);
      m_DrawArea->box(FL_NO_BOX);
      m_DrawArea->color(FL_BACKGROUND_COLOR);
      m_DrawArea->selection_color(FL_BACKGROUND_COLOR);
      m_DrawArea->labeltype(FL_NORMAL_LABEL);
      m_DrawArea->labelfont(0);
      m_DrawArea->labelsize(14);
      m_DrawArea->labelcolor(FL_FOREGROUND_COLOR);
      m_DrawArea->align(Fl_Align(FL_ALIGN_CENTER));
      m_DrawArea->when(FL_WHEN_RELEASE);
      Fl_Group::current()->resizable(m_DrawArea);
    } // DrawArea* m_DrawArea
    { m_MenuBar = new Fl_Menu_Bar(0, 0, 993, 20);
      m_MenuBar->box(FL_THIN_UP_BOX);
      m_MenuBar->labelfont(4);
      m_MenuBar->textfont(4);
      m_MenuBar->menu(menu_m_MenuBar);
    } // Fl_Menu_Bar* m_MenuBar
    { m_VSBar = new Fl_Scrollbar(965, 20, 20, 647);
      m_VSBar->step(0.001);
      m_VSBar->callback((Fl_Callback*)cb_m_VSBar);
    } // Fl_Scrollbar* m_VSBar
    { m_SBar = new Fl_Scrollbar(0, 647, 965, 20);
      m_SBar->type(1);
      m_SBar->labelfont(4);
      m_SBar->step(0.001);
      m_SBar->callback((Fl_Callback*)cb_m_SBar);
    } // Fl_Scrollbar* m_SBar
    m_Window->end();
  } // Fl_Double_Window* m_Window
  {
      if(!FileExist("./cache")){
          MakeDir("./cache");
      }
  }
}

void MainWindow::ShowAll() {
  m_Window->show();
}

void MainWindow::LoadMir2MapImage() {
  {
      const char *szFileName[] = {
          "Tilesc",
          "Tiles30c",
          "Tiles5c",
          "Smtilesc",
          "Housesc",
          "Cliffsc",
          "Dungeonsc",
          "Innersc",
          "Furnituresc",
          "Wallsc",
          "SmObjectsc",
          "Animationsc",
          "Object1c",
          "Object2c",
          "Custom",
          "Wood/Tilesc",
          "Wood/Tiles30c",
          "Wood/Tiles5c",
          "Wood/Smtilesc",
          "Wood/Housesc",
          "Wood/Cliffsc",
          "Wood/Dungeonsc",
          "Wood/Innersc",
          "Wood/Furnituresc",
          "Wood/Wallsc",
          "Wood/SmObjectsc",
          "Wood/Animationsc",
          "Wood/Object1c",
          "Wood/Object2c",
          "Wood/Custom",
          "Sand/Tilesc",
          "Sand/Tiles30c",
          "Sand/Tiles5c",
          "Sand/Smtilesc",
          "Sand/Housesc",
          "Sand/Cliffsc",
          "Sand/Dungeonsc",
          "Sand/Innersc",
          "Sand/Furnituresc",
          "Sand/Wallsc",
          "Sand/SmObjectsc",
          "Sand/Animationsc",
          "Sand/Object1c",
          "Sand/Object2c",
          "Sand/Custom",
          "Snow/Tilesc",
          "Snow/Tiles30c",
          "Snow/Tiles5c",
          "Snow/Smtilesc",
          "Snow/Housesc",
          "Snow/Cliffsc",
          "Snow/Dungeonsc",
          "Snow/Innersc",
          "Snow/Furnituresc",
          "Snow/Wallsc",
          "Snow/SmObjectsc",
          "Snow/Animationsc",
          "Snow/Object1c",
          "Snow/Object2c",
          "Snow/Custom",
          "Forest/Tilesc",
          "Forest/Tiles30c",
          "Forest/Tiles5c",
          "Forest/Smtilesc",
          "Forest/Housesc",
          "Forest/Cliffsc",
          "Forest/Dungeonsc",
          "Forest/Innersc",
          "Forest/Furnituresc",
          "Forest/Wallsc",
          "Forest/SmObjectsc",
          "Forest/Animationsc",
          "Forest/Object1c",
          "Forest/Object2c",
          "Forest/Custom",
          ""
      };
  
      extern std::string		g_WilFilePathName;
      extern WilImagePackage	g_WilImagePackage[128];
      for(int i = 0; std::strlen(szFileName[i]) > 0; ++i){
          g_WilImagePackage[i].Load(g_WilFilePathName.c_str(), szFileName[i], ".wil");
      }
  }
}

void MainWindow::LoadMir2Map() {
  {
      extern Mir2Map          g_Map;
      extern WilImagePackage  g_WilImagePackage[128];
      g_Map.LoadMapImage(g_WilImagePackage);
  
      Fl_Native_File_Chooser fileChooser;
      fileChooser.title("Select .map file");
      fileChooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
  
      char matchedFileNames[] =
          "MAP\t*.map\n"
          ;
  
      fileChooser.filter(matchedFileNames);
      fileChooser.directory(".");
      switch(fileChooser.show()){
          case -1:
              fl_alert(fileChooser.errmsg());
              break;
          case  1:
              break;
          default:
              {
                  extern Mir2Map g_Map;
                  if(!g_Map.LoadMap(fileChooser.filename())){
                      fl_alert("Load %s failed...", fileChooser.filename());
                  }
                  
                  AfterLoadMir2Map();
  
                  Fl::remove_timeout(UpdateFrame);
                  Fl::add_timeout(0.5, UpdateFrame);
              }
              break;
      }
      m_DrawArea->redraw();
  }
}

void MainWindow::AfterLoadMir2Map() {
  {
      extern Mir2Map g_Map;
      extern std::vector<std::vector<
          std::unordered_map<uint32_t, Fl_Shared_Image *>>> g_PNGCache;
      extern std::vector<std::vector<std::array<uint32_t, 4>>> g_GroundInfo;
  
      g_PNGCache.clear();
      g_GroundInfo.clear();
  
      g_PNGCache = std::vector<std::vector<
          std::unordered_map<uint32_t, Fl_Shared_Image *>>>(g_Map.Width(),
                  std::vector<std::unordered_map<uint32_t, Fl_Shared_Image *>>(
                      g_Map.Height()));
  
      // g_Map.Height(), {0, 0, 0, 0}));  // bug of fltk, doesn't accept
      g_GroundInfo = std::vector<std::vector<
          std::array<uint32_t, 4>>>(g_Map.Width(),
                  std::vector<std::array<uint32_t, 4>>(
                      g_Map.Height()));
  
      for(int nX = 0; nX < g_Map.Width(); ++nX){
          for(int nY = 0; nY < g_Map.Height(); ++nY){
              g_GroundInfo[nX][nY].fill(0XFFFFFFFF);
          }
      }
  
      // actually in Mir2Map the bFlag only has 0 / 1 available 
      auto fnSetGroundInfoFunc = [this](uint32_t nGroundInfo, int nX, int nY, int nIndex){
          extern std::vector<std::vector<std::array<uint32_t, 4>>> g_GroundInfo;
          if(nIndex >= 0 || nIndex < 4){
              g_GroundInfo[nX][nY][nIndex] = nGroundInfo;
          }
      };
  
      g_Map.ExtractGroundInfo(fnSetGroundInfoFunc);
  }
}

void MainWindow::SetWorkingPathName() {
  {
      extern std::string g_WorkingPathName;
      Fl_Native_File_Chooser stFileChooser;
      stFileChooser.title("Set Working Folder...");
      stFileChooser.options(Fl_Native_File_Chooser::NO_OPTIONS);
      stFileChooser.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
  
      stFileChooser.directory(".");
  
      switch(stFileChooser.show()){
          case -1:
              {
                  fl_alert(stFileChooser.errmsg());
                  break;
              }
          case 1:
              {
                  break;
              }
          default:
              {
                  g_WorkingPathName = stFileChooser.filename();
                  std::replace(g_WorkingPathName.begin(), g_WorkingPathName.end(), '\\', '/');
                  MakeWorkingFolder();
              }
              break;
      }
  }
}

void MainWindow::SetWilFilePathName() {
  {
      extern std::string g_WilFilePathName;
      Fl_Native_File_Chooser stFileChooser;
      stFileChooser.title("Set *.wil File Path...");
      stFileChooser.options(Fl_Native_File_Chooser::NO_OPTIONS);
      stFileChooser.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
  
      stFileChooser.directory(".");
  
      switch(stFileChooser.show()){
          case -1:
              {
                  fl_alert(stFileChooser.errmsg());
                  break;
              }
          case 1:
              {
                  break;
              }
          default:
              {
                  std::string szOldWilFilePathName = g_WilFilePathName;
                  g_WilFilePathName = stFileChooser.filename();
                  std::replace(g_WilFilePathName.begin(), g_WilFilePathName.end(), '\\', '/');
                  if(szOldWilFilePathName != g_WilFilePathName){
                      LoadMir2MapImage();
                      break;
                  }
              }
      }
  }
}

void MainWindow::CachePNG(uint32_t nFileIndex, uint32_t nImageIndex, int nX, int nY) {
  {
      extern std::vector<std::vector<std::unordered_map<uint32_t, Fl_Shared_Image *>>> g_PNGCache;
      uint32_t nKey = (((uint32_t)nFileIndex) << 16) + (uint32_t)nImageIndex;
      g_PNGCache[nX][nY][nKey] = RetrievePNG(nFileIndex, nImageIndex);
  }
}

void MainWindow::SavePNG(uint32_t *pBuff, uint32_t nFileIndex, uint32_t nImageIndex, int nW, int nH) {
  {
      char szPNGName[512];
      std::sprintf(szPNGName, "./cache/0%03d%05d.PNG", nFileIndex, nImageIndex);
      if(!FileExist(szPNGName)){
          SaveRGBABufferToPNG((uint8_t *)pBuff, nW, nH, szPNGName);
      }
  }
}

Fl_Shared_Image * MainWindow::RetrievePNG(uint32_t nFileIndex, uint32_t nImageIndex) {
  {
      char szPNGName[512];
      std::sprintf(szPNGName, "./cache/0%03d%05d.PNG", nFileIndex, nImageIndex);
      return Fl_Shared_Image::get(szPNGName);
  }
}

Fl_Shared_Image * MainWindow::RetrieveCachedPNG(uint32_t nFileIndex, uint32_t nImageIndex, int nX, int nY) {
  {
  	uint32_t nKey = ((uint32_t)nFileIndex << 16) + (uint32_t)nImageIndex;
  	extern std::vector<std::vector<std::unordered_map<uint32_t, Fl_Shared_Image *>>> g_PNGCache;
  	
  	if(g_PNGCache[nX][nY].find(nKey) != g_PNGCache[nX][nY].end()){
  		return g_PNGCache[nX][nY][nKey];
  	}else{
  		auto pImage = RetrievePNG(nFileIndex, nImageIndex);
          if(pImage){
              g_PNGCache[nX][nY][nKey] = pImage;
              return pImage;
          }else{
              auto fnSavePNGFunc = [this](uint32_t *pBuff,
                      uint32_t nFileIndex, uint32_t nImageIndex,
                      int nW, int nH, int nX, int nY){
                  SavePNG(pBuff, nFileIndex, nImageIndex, nW, nH);
                  // CachePNG(nFileIndex, nImageIndex, nX, nY);
              };
              extern Mir2Map g_Map;
              g_Map.ExtractOneBaseTile(fnSavePNGFunc, nX, nY);
              g_Map.ExtractOneObjectTile(fnSavePNGFunc, nX, nY);
  
              auto pImage = RetrievePNG(nFileIndex, nImageIndex);
  
              // here we don't check whether pImage is nullptr
              // since we have extract from map
              // if is null, means it invalid, then put nullptr here
              // can suppress next retrieving
              // 
              // maybe nullptr but it's intended to be
              g_PNGCache[nX][nY][nKey] = pImage;
              return pImage;
          }
      }
  }
}

void MainWindow::RedrawAll() {
  {
  	m_Window->redraw();
  }
}

void MainWindow::UpdateScrollBar(double fXP, double fYP) {
  auto pWidgetX = ((Fl_Valuator *)(m_SBar));
  auto pWidgetY = ((Fl_Valuator *)(m_VSBar));
  
  fXP = (std::min)(1.0, (std::max)(0.0, fXP));
  fYP = (std::min)(1.0, (std::max)(0.0, fYP));
  
  pWidgetX->value(pWidgetX->round(fXP));
  pWidgetY->value(pWidgetY->round(fYP));
}

void MainWindow::UpdateFrame(void *) {
  {
      extern Mir2Map g_Map;
      if(g_Map.Valid()){
          g_Map.SetAniTileFrame(50);
          extern MainWindow *g_MainWindow;
          g_MainWindow->RedrawAll();
      }
      Fl::repeat_timeout(0.5, UpdateFrame);
  }
}

bool MainWindow::ShowBaseTileLine() {
  {
  	return m_MenuItemBaseTileLine->value();
  }
}

bool MainWindow::ShowGroundObjectLine() {
  {
  	return m_MenuItemGroundObjectLine->value();
  }
}

bool MainWindow::ShowOverGroundObjectLine() {
  {
  	return m_MenuItemOverGroundObjectLine->value();
  }
}

bool MainWindow::ShowGroundInfoLine() {
  {
  	return m_MenuItemGroundInfoLine->value();
  }
}

bool MainWindow::ReversedShowGroundInfoLine() {
  {
  	return m_MenuItemReversedGroundInfoLine->value();
  }
}

bool MainWindow::ShowBaseTileLayer() {
  {
  	return m_MenuItemBaseTileLayer->value();
  }
}

bool MainWindow::ShowGroundObjectLayer() {
  {
  	return m_MenuItemGroundObjectLayer->value();
  }
}

bool MainWindow::ShowOverGroundObjectLayer() {
  {
  	return m_MenuItemOverGroundObjectLayer->value();
  }
}

bool MainWindow::EnableEdit() {
  {
  	return m_MenuItemEnableEdit->value();
  }
}

void MainWindow::MakeWorkingFolder() {
  {
      extern std::string g_WorkingPathName;
      if(g_WorkingPathName == "" || (g_WorkingPathName.size() > 0 && g_WorkingPathName[0] == '.')){
          time_t     stRawTime;
          struct tm *stTimeInfo;
          char       szBuffer[64];
  
          time(&stRawTime);
          stTimeInfo = localtime(&stRawTime);
          strftime(szBuffer, 64, "%Y%m%d%H%M%S", stTimeInfo);
          g_WorkingPathName = std::string("./") + szBuffer;
      }
  
      RemoveDir(g_WorkingPathName.c_str());
      MakeDir(g_WorkingPathName.c_str());
  
      std::string szTmpPath = g_WorkingPathName;
      if(szTmpPath.back() != '/'){
          szTmpPath += "/";
      }
      szTmpPath += "IMG";
      MakeDir(szTmpPath.c_str());
  }
}

void MainWindow::SaveMap() {
  // save map
  {
      // currently use uint32_t as map grid attribute
      extern Mir2Map g_Map;
      if(!g_Map.Valid()){
          return;
      }
  
      // only make sense when sizeof(index) < sizeof(attribute)
      // but since here we use uint32_t as attribute, so we don't need it
      // std::vector<std::vector<std::array<uint32_t, 4>>> g_GroundInfo;
      // std::unordered_set<uint32_t, int> stMapAttributeMap;
      // int nCount = 0;
      // for(auto &p: g_GroundInfo){
      //     for(auto &q: p){
      //         for(auto &r: q){
      //             if(stMapAttributeMap.find(r) == stMapAttributeMap.end()){
      //                 stMapAttributeMap[r] = nCount;
      //                 nCount++;
      //             }
      //         }
      //     }
      // }
  
      int nBlkW = g_Map.Width()  / 8;
      int nBlkH = g_Map.Height() / 8;
  
      std::vector<bool> stGroundInfoBitV;
      std::vector<uint32_t> stGroundInfoV;
      // be careful here, first Y then X
      for(int nBlkY = 0; nBlkY < nBlkH; ++nBlkY){
          for(int nBlkX = 0; nBlkX < nBlkW; ++nBlkX){
              QTreePreOrder(nBlkX * 8, nBlkY * 8, 8, stGroundInfoBitV, stGroundInfoV);
          }
      }
  
      std::vector<bool> stTileInfoBitV;
      std::vector<uint32_t> stTileInfoV;
  
      g_Map.CompressBaseTileInfo(stTileInfoBitV, stTileInfoV);
      
      std::vector<bool> stCellInfoBitV;
      std::vector<CELLDESC> stCellInfoV;
      
      g_Map.CompressCellTileInfo(stCellInfoBitV, stCellInfoV);
  
      extern std::string g_WorkingPathName;
      std::string szTmpFileName = g_WorkingPathName;
  
      if(szTmpFileName.back() == '/'){
          szTmpFileName += "desc.bin";
      }else{
          szTmpFileName += "/desc.bin";
      }
  
      auto pFile = fopen(szTmpFileName.c_str(), "wb");
      if(pFile == nullptr){
          fl_alert("fail to open %s for writing", szTmpFileName.c_str());
          return;
      }
  
      { // size info
          uint16_t nTmpVar;
          nTmpVar = g_Map.Width();
          fwrite(&nTmpVar, sizeof(nTmpVar), 1, pFile);
  
          nTmpVar = g_Map.Height();
          fwrite(&nTmpVar, sizeof(nTmpVar), 1, pFile);
      }
  
      // ground info 
      { // ground information bit stream length, without align 
          uint32_t nTmpVarUint32 = (uint32_t)(stGroundInfoBitV.size());
          fwrite(&nTmpVarUint32, sizeof(uint32_t), 1, pFile);
      }
  
      { // groundinfo stream start
          // align to 32 * N bit
          if(stGroundInfoBitV.size() % 32){
              int nSize = stGroundInfoBitV.size();
              stGroundInfoBitV.insert(stGroundInfoBitV.end(), 32 - (nSize % 32), false);
          }
  
          uint32_t nTmpVarUint32 = 0;
          int nCurrentPos = 0;
          while((size_t)nCurrentPos < stGroundInfoBitV.size()){
              nTmpVarUint32 = (nTmpVarUint32 * 2 + (stGroundInfoBitV[nCurrentPos++] ? 1 : 0));
              if(!(nCurrentPos % (sizeof(nTmpVarUint32) * 8))){
                  fwrite(&nTmpVarUint32, sizeof(nTmpVarUint32), 1, pFile);
                  nTmpVarUint32 = 0;
              }
          }
  
          // since aligned by 32, so there won't be left
          // then we need to add another uint32_t or not for 64-align boundary
          if((stGroundInfoBitV.size() / 32) % 2 == 0){
              nTmpVarUint32 = 0;
              fwrite(&nTmpVarUint32, sizeof(nTmpVarUint32), 1, pFile);
          }
      }
  
      { // ground information struct count, without align 
          uint32_t nTmpVarUint32 = (uint32_t)(stGroundInfoV.size());
          fwrite(&nTmpVarUint32, sizeof(uint32_t), 1, pFile);
      }
  
      { // ground info insertion
          if(stGroundInfoV.size() > 0){
              fwrite(&stGroundInfoV[0], sizeof(stGroundInfoV[0]), stGroundInfoV.size(), pFile);
          }
          if(stGroundInfoV.size() % 2 == 0){
              uint32_t nTmpVarUint32 = 0;
              fwrite(&nTmpVarUint32, sizeof(uint32_t), 1, pFile);
          }
      }
  
      // base tile info
      { // tile bit stream length, without align 
          uint32_t nTmpVarUint32 = (uint32_t)(stTileInfoBitV.size());
          fwrite(&nTmpVarUint32, sizeof(uint32_t), 1, pFile);
      }
  
      { // tile stream start
          // align to 32 * N bit
          if(stTileInfoBitV.size() % 32){
              int nSize = stTileInfoBitV.size();
              stTileInfoBitV.insert(stTileInfoBitV.end(), 32 - (nSize % 32), false);
          }
  
          uint32_t nTmpVarUint32 = 0;
          int nCurrentPos = 0;
          while((size_t)nCurrentPos < stTileInfoBitV.size()){
              nTmpVarUint32 = (nTmpVarUint32 * 2 + (stTileInfoBitV[nCurrentPos++] ? 1 : 0));
              if(!(nCurrentPos % (sizeof(nTmpVarUint32) * 8))){
                  fwrite(&nTmpVarUint32, sizeof(nTmpVarUint32), 1, pFile);
                  nTmpVarUint32 = 0;
              }
          }
          // since aligned by 32, so there won't be left
      }
  
      {
          if((stTileInfoBitV.size() / 32) % 2 == 0){
              uint32_t nTmpVarUint32 = 0;
              fwrite(&nTmpVarUint32, sizeof(nTmpVarUint32), 1, pFile);
          }
  
      }
  
      { // base tile structure count, without align 
          uint32_t nTmpVarUint32 = (uint32_t)(stTileInfoV.size());
          fwrite(&nTmpVarUint32, sizeof(uint32_t), 1, pFile);
      }
  
      { // base tile structure insertion
          if(stTileInfoV.size() > 0){
              fwrite(&stTileInfoV[0], sizeof(stTileInfoV[0]), stTileInfoV.size(), pFile);
          }
          if(stTileInfoV.size() % 2 == 0){
              uint32_t nTmpVarUint32 = 0;
              fwrite(&nTmpVarUint32, sizeof(nTmpVarUint32), 1, pFile);
          }
      }
  
      // cell desc start
      { // tile bit stream length, without align 
          uint32_t nTmpVarUint32 = (uint32_t)(stCellInfoBitV.size());
          fwrite(&nTmpVarUint32, sizeof(uint32_t), 1, pFile);
      }
  
      { 
          // align to 32 * N bit
          if(stCellInfoBitV.size() % 32){
              int nSize = stCellInfoBitV.size();
              stCellInfoBitV.insert(stCellInfoBitV.end(), 32 - (nSize % 32), false);
          }
  
          uint32_t nTmpVarUint32 = 0;
          int nCurrentPos = 0;
          while((size_t)nCurrentPos < stCellInfoBitV.size()){
              nTmpVarUint32 = (nTmpVarUint32 * 2 + (stCellInfoBitV[nCurrentPos++] ? 1 : 0));
              if(!(nCurrentPos % (sizeof(nTmpVarUint32) * 8))){
                  fwrite(&nTmpVarUint32, sizeof(nTmpVarUint32), 1, pFile);
                  nTmpVarUint32 = 0;
              }
          }
          // since aligned by 32, so there won't be left
      }
  
      {
          if((stCellInfoBitV.size() / 32) % 2 == 0){
              uint32_t nTmpVarUint32 = 0;
              fwrite(&nTmpVarUint32, sizeof(nTmpVarUint32), 1, pFile);
          }
  
      }
  
      { // object structure count, without align 
          uint32_t nTmpVarUint32 = (uint32_t)(stCellInfoV.size());
          fwrite(&nTmpVarUint32, sizeof(uint32_t), 1, pFile);
      }
  
      { // obj info insertion
          if(stCellInfoV.size() > 0){
              fwrite(&stCellInfoV[0], sizeof(stCellInfoV[0]), stCellInfoV.size(), pFile);
          }
          // we don't have to align to 64 here, since CELLDESC is already aligned to 64
          // if(stCellInfoV.size() % 2 == 0){
          //     uint32_t nTmpVarUint32 = 0;
          //     fwrite(&nTmpVarUint32, sizeof(nTmpVarUint32), 1, pFile);
          // }
      }
  
      fclose(pFile);
      
      // save PNG's
  }
}

bool MainWindow::EqualGroundInfoBlock(int nStartX, int nStartY, int nSize) {
  {
      // nSize should be 8 4 2 1
      extern std::vector<std::vector<std::array<uint32_t, 4>>> g_GroundInfo;
      uint32_t nAttribute = g_GroundInfo[nStartX][nStartY][0];
      for(int nX = nStartX; nX < nStartX + nSize; ++nX){
          for(int nY = nStartY; nY < nStartY + nSize; ++nY){
              for(int nZ = 0; nZ < 4; ++nZ){
                  if(nAttribute != g_GroundInfo[nX][nY][nZ]){
                      return false;
                  }
              }
          }
      }
      return true;
  }
}

void MainWindow::QTreePreOrder(int nStartX, int nStartY, int nSize, std::vector<bool> &stBitV, std::vector<uint32_t> &stAttributeV) {
  {
      // when nSize = 8 4 2 1
      // +---+---+
      // | 0 | 1 |
      // +---+---+
      // | 2 | 3 |
      // +---+---+
      //
      // when for small gird
      // +-----------+
      // |     0     |
      // |   3 + 1   |
      // |     2     |
      // +-----------+
  
      extern std::vector<std::vector<std::array<uint32_t, 4>>> g_GroundInfo;
      if(EqualGroundInfoBlock(nStartX, nStartY, nSize)){
          if(g_GroundInfo[nStartX][nStartY][0] == 0XFFFFFFFF){
              // nothing, use 0 to indicate
              stBitV.push_back(false);
          }else{
              // unique and walkable
              stBitV.push_back(true);
              stBitV.push_back(false);
              stAttributeV.push_back(g_GroundInfo[nStartX][nStartY][0]);
          }
      }else{
          // combined
          stBitV.push_back(true);
          stBitV.push_back(true);
          if(nSize == 1){
              for(int nCnt = 0; nCnt < 4; ++nCnt){
                  if(g_GroundInfo[nStartX][nStartY][nCnt] != 0XFFFFFFFF){
                      stBitV.push_back(true);
                      stAttributeV.push_back(g_GroundInfo[nStartX][nStartY][nCnt]);
                  }else{
                      stBitV.push_back(false);
                  }
              }
          }else{
              QTreePreOrder(nStartX, nStartY, 
                      nSize / 2,
                      stBitV, stAttributeV);
  
              QTreePreOrder(nStartX + nSize / 2, nStartY,
                      nSize / 2,
                      stBitV, stAttributeV);
  
              QTreePreOrder(nStartX, nStartY + nSize / 2,
                      nSize / 2,
                      stBitV, stAttributeV);
  
              QTreePreOrder(nStartX + nSize / 2, nStartY + nSize / 2,
                      nSize / 2,
                      stBitV, stAttributeV);
          }
      }
  }
}

void MainWindow::MapCrop(int nStartX, int nStartY, int nW, int nH) {
  {
      extern Mir2Map g_Map;
      g_Map.CropSize(nStartX, nStartY, nW, nH);
  }
}

bool MainWindow::ClearBackground() {
  {
      return m_MenuItemClearBackground->value();
  }
}
