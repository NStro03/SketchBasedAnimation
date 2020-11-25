// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
#include "Draw.h"
// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Sketch Based Animation", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec4 marker_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    ImVec4 sheet_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
    ImVec2 toolbarsize = ImVec2(320,720);
    ImVec2 toolbarpos = ImVec2(960,0);
    ImVec2 sheetsize = ImVec2(960,720);
    ImVec2 sheetpos = ImVec2(0,0);
    static int choice = 1, anchorX=-1, anchorY=-1, choose_anchor=0, choose_obj=0;
    const ImU32 MarkerCol32 = ImColor(marker_color);
    const ImU32 SelectCol32 = ImColor(ImVec4(1.0f, 0.1875f, 0.0625f, 1.00f));
    const ImU32 HoverCol32 = ImColor(ImVec4(1.0f, 1.0f, 0.0f, 1.00f));
    const ImU32 White32 = ImColor(ImVec4(1.0f, 1.0f, 1.0f, 1.00f));
    // Main loop


    
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        // if (show_demo_window)
            // ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
       

        // 3. Show another simple window.
        // if (show_another_window)
        // {
        //     ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //     ImGui::Text("Hello from another window!");
        //     if (ImGui::Button("Close Me"))
        //         show_another_window = false;
        //     ImGui::End();
        // }
         {
            // static float f = 0.0f;
            // static int counter = 0;
            ImGui::PushStyleColor(ImGuiCol_WindowBg, sheet_color);

            ImGui::Begin("Canvas", NULL, 38);                          // Create a window called "Hello, world!" and append into it.

            ImGui::SetWindowPos(sheetpos);
            ImGui::SetWindowSize(sheetsize);
            // static ImVec2 points[] = { { 100, 120 }, { 140, 800 }, { 50, 50 } };
            // printf("%s\n", leftMouseDown ? "true" : "false");
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            int i = 0;
            for(i=0;i<PlObjects.size();i++){
                // std::vector<ImVec2> points = PlObjects[ObjectCount].getPoints();
                //printf("%d %d\n",i,Objects[i].size);
                if(selected==i+1){
                    draw_list->AddPolyline(PlObjects[i].getPoints(),PlObjects[i].getSize(), SelectCol32, false, 3.0f);
                }
                else if(hovered==i+1){
                    draw_list->AddPolyline(PlObjects[i].getPoints(),PlObjects[i].getSize(), HoverCol32, false, 3.0f);
                }
                else{
                    draw_list->AddPolyline(PlObjects[i].getPoints(),PlObjects[i].getSize(), MarkerCol32, false, 3.0f);
                }
            }

            // ImGui::GetWindowDrawList()->AddCallback(draw_callback, NULL);

            // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            // ImGui::Checkbox("Another Window", &show_another_window);

            // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            // ImGui::ColorEdit3("clear color", (float*)&sheet_color); // Edit 3 floats representing a color

            // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //     counter++;
            // ImGui::SameLine();
            // ImGui::Text("counter = %d", counter);

            // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            
            ImGui::End();
            ImGui::PopStyleColor();


            
        }




         {
            // static float f = 0.0f;
            // static int counter = 0;

            ImGui::Begin("Toolbar", NULL, 38);                          // Create a window called "Hello, world!" and append into it.

            ImGui::SetWindowPos(toolbarpos);
            ImGui::SetWindowSize(toolbarsize);
            // ImGui::Dummy(ImVec2(0.0f, 20.0f));

            // ImGui::ColorEdit3("Ink color", (float*)&marker_color);    // Edit 3 floats representing a color
            // ImGui::Dummy(ImVec2(0.0f, 20.0f));

            if (ImGui::Button("Draw", ImVec2(145.0f, 50.0f))){
                choice = 1;
                selected = 0;
                hovered = 0;
            }
            ImGui::SameLine();
            if(ImGui::Button("Select", ImVec2(145.0f, 50.0f))){
                choice = 2;
            }
            if(choose_anchor==1){
                if(ImGui::GetIO().MouseClicked[0]){
                    ImVec2 distFromClick(io.MouseClickedPos[0]);
                    x = (int)(distFromClick.x);
                    y = (int)(distFromClick.y);
                    if(x>0 && x<960 && y>0 && y<720){
                        anchorX = x;
                        anchorY = y;
                        choose_anchor = 0;
                    }
                }
            }
            else if(choice==1){
                draw(window);
            }
            else{
                selectCurve(io);
            }
            //ImGui::Dummy(ImVec2(0.0f, 20.0f));

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list-> AddLine(ImVec2(965.0f, 90.0f), ImVec2(1275.0f, 90.0f), White32);

            ImVec2 cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(10.0f+cursor.x, 20.0f+cursor.y));

            ImGui::SliderFloat("translate X", &transX, -1.0f, 1.0f);

            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(10.0f+cursor.x, 0.0f+cursor.y));
            ImGui::SliderFloat("translate y", &transY, -1.0f, 1.0f);



            draw_list-> AddLine(ImVec2(980.0f, 155.0f), ImVec2(1260.0f, 155.0f), White32);
            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(20.0f+cursor.x, 20.0f+cursor.y));

            ImGui::SliderFloat("Scale X", &ScaleX, 0.01f, 10.0f);

            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(20.0f+cursor.x, 0.0f+cursor.y));
            ImGui::SliderFloat("Scale y", &ScaleY, 0.01f, 10.0f);




            draw_list-> AddLine(ImVec2(980.0f, 220.0f), ImVec2(1260.0f, 220.0f), White32);
            
            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(20.0f+cursor.x, 20.0f+cursor.y));

            ImGui::SliderFloat("Rotate", &Rotate, -3.14f, 3.14f);


            draw_list-> AddLine(ImVec2(980.0f, 265.0f), ImVec2(1260.0f, 265.0f), White32);

            cursor = ImGui::GetCursorPos();
            if(selected>0 && choose_anchor==2){
                choose_anchor=0;
            }

            if(choose_anchor==0){
                ImGui::SetCursorPos(ImVec2(55.0f+cursor.x, 20.0f+cursor.y));
                ImGui::Text("Try Basic Transformations");
            }
            else if(choose_anchor==1){
                ImGui::SetCursorPos(ImVec2(50.0f+cursor.x, 20.0f+cursor.y));
                ImGui::Text("Please Select Anchor Points");
            }
            else{
                ImGui::SetCursorPos(ImVec2(55.0f+cursor.x, 20.0f+cursor.y));
                ImGui::Text("Please Select An Object");
            }

            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(40.0f+cursor.x, 5.0f+cursor.y));

            if (ImGui::Button("Transform", ImVec2(100.0f, 30.0f))){
                if(selected>0){
                    choose_anchor = 0;
                    if(transX!=0 || transY!=0){
                        PlObjects[selected-1].translate(transX, transY);
                        transX = 0;
                        transY = 0;
                        choose_anchor = 0;
                    }
                    if(ScaleX!=1.0 || ScaleY!=1.0){
                        if(anchorX==-1){
                            choose_anchor = 1;
                        }
                        else{
                            PlObjects[selected-1].scale(anchorX, anchorY, ScaleX, ScaleY);
                            choose_anchor = 0;
                            ScaleX = 1;
                            ScaleY = 1;
                        }
                    }
                    if(Rotate!=0){
                        if(anchorX==-1){
                            choose_anchor = 1;
                        }
                        else{
                            PlObjects[selected-1].rotate(anchorX, anchorY, Rotate);
                            choose_anchor = 0;
                            Rotate = 0;
                        }
                    }
                    
                }
                else{
                    choose_anchor = 2;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Reset", ImVec2(100.0f, 30.0f))){
                Rotate = 0;
                ScaleX = 1;
                ScaleY = 1;
                transX = 0;
                transY = 0;
                anchorX = -1;
                anchorY = -1;
                choose_anchor = 0;
            }

            // cursor = ImGui::GetCursorPos();
            // ImGui::SetCursorPos(ImVec2(30.0f+cursor.x, 20.0f+cursor.y));
            draw_list-> AddLine(ImVec2(965.0f, 340.0f), ImVec2(1275.0f, 340.0f), White32);

            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(cursor.x, 20.0f+cursor.y));

            if(selected>0 && choose_obj==1)
                choose_obj = 0;

            if(choose_obj==0){
                ImGui::SetCursorPos(ImVec2(70.0f+cursor.x, 20.0f+cursor.y));
                ImGui::Text("Try Kinetic Textures");
            }
            else{
                ImGui::SetCursorPos(ImVec2(50.0f+cursor.x, 20.0f+cursor.y));
                ImGui::Text("Please Select Source Object");
            }

            cursor = ImGui::GetCursorPos();
            ImGui::SetCursorPos(ImVec2(cursor.x, 5.0f+cursor.y));

            if (ImGui::Button("Emitting", ImVec2(145.0f, 50.0f))){
                if(choose_anchor==0){
                    if(selected==0){
                        choose_obj = 1;
                    }
                    else{
                        choose_obj = 0;
                    }
                }
            }
            ImGui::SameLine();
            if(ImGui::Button("Oscillating", ImVec2(145.0f, 50.0f))){
                if(choose_anchor==0){
                    if(selected==0){
                        choose_obj = 1;
                    }
                    else{
                        choose_obj = 0;
                    }
                }
            }

            // ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            // ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            // ImGui::Checkbox("Another Window", &show_another_window);

            // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //     counter++;
            // ImGui::SameLine();
            // ImGui::Text("counter = %d", counter);

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();


            
        }
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        setleftmouseDown(io);
        glfwSwapBuffers(window);
    
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
