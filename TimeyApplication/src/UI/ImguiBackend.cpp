#include "timey_pch.h"
#include "glad/glad.h"
#include "glfw/glfw3.h" 

#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw/glfw3native.h"


#include "ImguiBackend.h"
#include "AppCore/Application.h"

namespace Timey {

	namespace ImguiBackend {

		static void _init_opengl();
		static void _init_glfw();
		static void _compile_shader(const GLchar* vertex_src, const GLchar* frag_src);
		static void _load_font_texture();
		static void _glfw_update_monitor();
		static void _glfw_monitor_callback(GLFWmonitor* monitor, int code);
		static void _glfw_new_frame();
		static void _setup_render_states(ImDrawData* drawdata, int fb_width, int fb_hight, GLuint vertex_array);
		static void _draw_render_data(ImDrawData* drawdata);
		static void _imgui_update_contrl_keys();
		static void _opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

		static void _init_platform_interface();
		static void _imgui_create_window(ImGuiViewport* viewport);
		static void _imgui_distory_window(ImGuiViewport* viewport);
		static void _imgui_show_window(ImGuiViewport* viewport);
		static void _imgui_set_window_pos(ImGuiViewport* viewport, ImVec2 pos);
		static ImVec2 _imgui_get_window_pos(ImGuiViewport* viewport);
		static void  _imgui_set_window_size(ImGuiViewport* viewport, ImVec2 size);
		static ImVec2 _imgui_get_window_size(ImGuiViewport* viewport);
		static void _imgui_set_window_focus(ImGuiViewport* viewport);
		static bool _imgui_get_window_focus(ImGuiViewport* viewport);
		static bool _imgui_get_window_minimized(ImGuiViewport* viewport);
		static void _imgui_set_window_title(ImGuiViewport* viewport, const char* title);
		static void _imgui_render_window(ImGuiViewport* viewport, void* render_arg);
		static void _imgui_swap_buffer(ImGuiViewport* viewport, void* render_arg);
		static void _imgui_render_viewports(ImGuiViewport* viewport, void* render_args);



		static GLuint imgui_vertex_shader = 0;
		static GLuint imgui_fragment_shader = 0;
		static GLuint imgui_shader_program = 0;
		static GLuint imgui_vertex_buffer = 0;
		static GLuint imgui_index_buffer = 0;
		static GLuint imgui_font_texture = 0;


		static bool need_update_monitor = true;


		static struct uniform_location {
			GLint texture;
			GLint projection_mtx;
		} imgui_uniforms;

		static struct attrib_location {
			GLint Position;
			GLint UV;
			GLint Color;
		} imgui_attributes;


		static Ref<BaseWindow> _get_app_main_window() { return Application::getApplication().getWindowHandle(); };

		static struct ImguiViewPortData {
			Ref<BaseWindow> window;
			bool        window_owned = false;
			int			ignore_window_pos_event_frame = -1;
			int         ignore_window_size_event_frame = -1;

			~ImguiViewPortData()
			{
				window_owned = false;
				window = nullptr;
				window.reset();
			};

		};




		void Init() {

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();

			io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
			io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
			io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
			io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

			io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
			io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
			io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
			io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
			io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
			io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
			io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
			io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
			io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
			io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
			io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
			io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
			io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
			io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
			io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
			io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
			io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
			io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
			io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
			io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
			io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
			io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;


			_init_opengl();
			_init_glfw();

		}

		void NewFrame()
		{

			ImGuiIO& io = ImGui::GetIO();
			Application& app = Application::getApplication();
			int w = app.getWindowHandle()->getWidth();
			int h = app.getWindowHandle()->getHight();

			io.DisplaySize = ImVec2((float)w, (float)h);

			_glfw_new_frame();
			ImGui::NewFrame();
		}

		void DrawRenderData(ImDrawData* drawdata)
		{

			ImGuiIO& io = ImGui::GetIO();

			_draw_render_data(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			};

		}

		void ShutDown()
		{


			if (imgui_vertex_buffer) glDeleteBuffers(1, &imgui_vertex_buffer);
			if (imgui_index_buffer)  glDeleteBuffers(1, &imgui_index_buffer);
			if (imgui_shader_program && imgui_vertex_shader) glDetachShader(imgui_shader_program, imgui_vertex_shader);
			if (imgui_shader_program && imgui_fragment_shader) glDetachShader(imgui_shader_program, imgui_fragment_shader);
			if (imgui_vertex_shader) glDeleteShader(imgui_vertex_shader);
			if (imgui_fragment_shader) glDeleteShader(imgui_fragment_shader);
			if (imgui_shader_program) glDeleteProgram(imgui_shader_program);



			if (imgui_font_texture)
			{
				ImGuiIO& io = ImGui::GetIO();
				glDeleteTextures(1, &imgui_font_texture);
				io.Fonts->TexID = 0;
				imgui_font_texture = 0;
			};


			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;


			if (data) {
				if (data->window_owned) {
					data->~ImguiViewPortData();
					// We don't explicitly delete the data, since it might mess with the shared pointer inside it . 
				};
			};


			viewport->PlatformHandle = nullptr;
			viewport->PlatformHandleRaw = nullptr;
			viewport->PlatformUserData = nullptr;

			ImGui::DestroyPlatformWindows();
			ImGui::DestroyContext();
		}



		void OnEvent(Event& e)
		{

			EventDispatcher dispatcher(e);

			dispatcher.DispatchEvents<KeyPressedEvent>(TIMEY_BIND_STATIC_CALLBACK(KeyPressedCallBack));
			dispatcher.DispatchEvents<KeyReleasedEvent>(TIMEY_BIND_STATIC_CALLBACK(KeyReleasedCallBack));
			dispatcher.DispatchEvents<MousePressedEvent>(TIMEY_BIND_STATIC_CALLBACK(MousePressedCallBack));
			dispatcher.DispatchEvents<MouseReleasedEvent>(TIMEY_BIND_STATIC_CALLBACK(MouseReleasedCallBack));
			dispatcher.DispatchEvents<MouseMovedEvent>(TIMEY_BIND_STATIC_CALLBACK(MouseMovedCallBack));
			dispatcher.DispatchEvents<MouseScrolledEvent>(TIMEY_BIND_STATIC_CALLBACK(MouseScrolledCallBack));
			dispatcher.DispatchEvents<KeyTypedEvent>(TIMEY_BIND_STATIC_CALLBACK(KeyTypedCallBack));
			dispatcher.DispatchEvents<WindowResizeEvent>(TIMEY_BIND_STATIC_CALLBACK(WindowResizeCallBack));
			dispatcher.DispatchEvents<WindowMovedEvent>(TIMEY_BIND_STATIC_CALLBACK(WindowMovedCallBack));


		}

		bool KeyPressedCallBack(KeyPressedEvent& e)
		{

			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[e.getKeyCode()] = true;
			_imgui_update_contrl_keys();

			return true;
		}

		bool KeyReleasedCallBack(KeyReleasedEvent& e)
		{

			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[e.getKeyCode()] = false;
			_imgui_update_contrl_keys();

			return true;

		}

		bool MousePressedCallBack(MousePressedEvent& e)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[e.getMosueButton()] = true;

			return true;
		}

		bool MouseReleasedCallBack(MouseReleasedEvent& e)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[e.getMosueButton()] = false;
			return true;
		}

		bool MouseMovedCallBack(MouseMovedEvent& e)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

			for (int i = 0; i < platform_io.Viewports.Size; i++) {

				ImGuiViewport* viewport = platform_io.Viewports[i];
				ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
				TIMEY_CORE_ASSERT(data, "PlatformUserData is uninitiallized.");

				if (data->window->isFocused()) {

					int mouse_x = e.getXPos();
					int mouse_y = e.getYPos();

					if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

						int window_x = data->window->getPos().first;
						int window_y = data->window->getPos().second;

						io.MousePos = ImVec2((float)mouse_x + window_x, (float)mouse_y + window_y);

					}
					else {
						io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
					}

				}

			}

			return true;
		}

		bool MouseScrolledCallBack(MouseScrolledEvent& e)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.MouseWheelH += (float)e.getxOffset();
			io.MouseWheel += (float)e.getyOffset();
			return true;
		}

		bool KeyTypedCallBack(KeyTypedEvent& e)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.AddInputCharacter(e.getKeyCode());
			_imgui_update_contrl_keys();

			return true;
		}

		bool WindowResizeCallBack(WindowResizeEvent& e)
		{
			ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(e.WindowHandle);
			TIMEY_CORE_ASSERT(viewport, "Cannot find viewport given the window handle.");

			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			TIMEY_CORE_ASSERT(data, "No User data in viewport handle.");

			bool ignore_event = (ImGui::GetFrameCount() <= data->ignore_window_size_event_frame + 1);
			if (ignore_event) viewport->PlatformRequestResize = true;

			return true;
		}

		bool WindowMovedCallBack(WindowMovedEvent& e)
		{
			ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(e.WindowHandle);
			TIMEY_CORE_ASSERT(viewport, "Cannot find viewport given the window handle.");

			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			TIMEY_CORE_ASSERT(data, "No User data in viewport handle.");

			bool ignore_event = (ImGui::GetFrameCount() <= data->ignore_window_pos_event_frame + 1);
			if (ignore_event) viewport->PlatformRequestMove = true;


			return true;
		}




		void _init_glfw() {

			ImguiViewPortData* imgui_viewport_data = IM_NEW(ImguiViewPortData);

			Ref<BaseWindow> window;
			window = _get_app_main_window();

			void* native_window = window->getNativeWindow();


			ImGuiViewport* main_viewport = ImGui::GetMainViewport();
			main_viewport->PlatformHandle = native_window;

			imgui_viewport_data->window = window;
			imgui_viewport_data->window_owned = true;
			imgui_viewport_data->ignore_window_pos_event_frame = -1;
			imgui_viewport_data->ignore_window_size_event_frame = -1;

			main_viewport->PlatformUserData = (void*)imgui_viewport_data;

			#ifdef _WIN32
			main_viewport->PlatformHandleRaw = glfwGetWin32Window((GLFWwindow*)native_window);
			#endif

			ImGuiIO& io = ImGui::GetIO();

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
				_init_platform_interface();
			}

			glfwSetMonitorCallback(_glfw_monitor_callback);

		};



		static void _opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
		{

			std::string msg;
			msg.reserve(length + 1);
			msg = message;

			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH		: TIMEY_CORE_ERROR("OpenGL Error: {0}", msg); TIMEY_CORE_ASSERT(false, "");
			case GL_DEBUG_SEVERITY_MEDIUM	: TIMEY_CORE_WARN("OpenGL Warning: {0}", msg);
			case GL_DEBUG_SEVERITY_LOW		: TIMEY_CORE_INFO("OpenGL Message: {0}", msg);
			case GL_DEBUG_SEVERITY_NOTIFICATION	: TIMEY_CORE_TRACE("OpenGL Message: {0}", msg);
			
			}
		};
		



		static void _init_opengl() {

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			glDebugMessageCallback(Timey::ImguiBackend::_opengl_error_callback, nullptr);

	
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			//glEnable(GL_SCISSOR_TEST);
			#ifdef GL_POLYGON_MODE
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			#endif

			
				

			glGenBuffers(1, &imgui_vertex_buffer);
			glGenBuffers(1, &imgui_index_buffer);
			glGenTextures(1, &imgui_font_texture);



			// Set Up can Compile Shaders.
			const GLchar* imgui_vertex_shader_src =
				"#version 410 core \n"
				"layout (location = 0) in vec2 Position;\n"
				"layout (location = 1) in vec2 UV;\n"
				"layout (location = 2) in vec4 Color;\n"
				"uniform mat4 ProjMtx;\n"
				"out vec2 Frag_UV;\n"
				"out vec4 Frag_Color;\n"
				"void main()\n"
				"{\n"
				"    Frag_UV = UV;\n"
				"    Frag_Color = Color;\n"
				"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
				"}\n";

			const GLchar* imgui_fragment_shader_src =
				"#version 410 core \n"
				"in vec2 Frag_UV;\n"
				"in vec4 Frag_Color;\n"
				"uniform sampler2D Texture;\n"
				"layout (location = 0) out vec4 Out_Color;\n"
				"void main()\n"
				"{\n"
				"    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
				"}\n";

			_compile_shader(imgui_vertex_shader_src, imgui_fragment_shader_src);


			// Set Up textures


			glBindTexture(GL_TEXTURE_2D, imgui_font_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);




			imgui_uniforms.texture = glGetUniformLocation(imgui_shader_program, "Texture");
			imgui_uniforms.projection_mtx = glGetUniformLocation(imgui_shader_program, "ProjMtx");


		};

		static void _compile_shader(const GLchar* vertex_src, const GLchar* frag_src) {



			imgui_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(imgui_vertex_shader, 1, &vertex_src, nullptr);
			glCompileShader(imgui_vertex_shader);

			GLint is_compiled = 0;
			glGetShaderiv(imgui_vertex_shader, GL_COMPILE_STATUS, &is_compiled);

			if (is_compiled == GL_FALSE)
			{
				GLint max_length = 0;
				glGetShaderiv(imgui_vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

				std::vector<GLchar> log(max_length);
				glGetShaderInfoLog(imgui_vertex_shader, max_length, &max_length, &log[0]);

				glDeleteShader(imgui_vertex_shader);

				TIMEY_CORE_ERROR("{0}", log.data());
				TIMEY_CORE_ASSERT(false, "Vertex Shader Compile Failure.");

			}


			imgui_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(imgui_fragment_shader, 1, &frag_src, nullptr);
			glCompileShader(imgui_fragment_shader);


			glGetShaderiv(imgui_fragment_shader, GL_COMPILE_STATUS, &is_compiled);

			if (is_compiled == GL_FALSE)
			{
				GLint max_length = 0;
				glGetShaderiv(imgui_fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

				std::vector<GLchar> log(max_length);
				glGetShaderInfoLog(imgui_fragment_shader, max_length, &max_length, &log[0]);

				glDeleteShader(imgui_fragment_shader);

				TIMEY_CORE_ERROR("{0}", log.data());
				TIMEY_CORE_ASSERT(false, "Fragment Shader Compile Failure.");
			}



			imgui_shader_program = glCreateProgram();
			glAttachShader(imgui_shader_program, imgui_vertex_shader);
			glAttachShader(imgui_shader_program, imgui_fragment_shader);
			glLinkProgram(imgui_shader_program);



			GLint is_linked = 0;
			glGetProgramiv(imgui_shader_program, GL_LINK_STATUS, (int*)&is_linked);
			if (is_linked == GL_FALSE)
			{
				GLint max_length = 0;
				glGetProgramiv(imgui_shader_program, GL_INFO_LOG_LENGTH, &max_length);

				std::vector<GLchar> log(max_length);
				glGetProgramInfoLog(imgui_shader_program, max_length, &max_length, &log[0]);


				glDeleteProgram(imgui_shader_program);

				glDeleteShader(imgui_vertex_shader);
				glDeleteShader(imgui_fragment_shader);

				TIMEY_CORE_ERROR("{0}", log.data());
				TIMEY_CORE_ASSERT(false, "Program Linking Error.");
			}

			glValidateProgram(imgui_shader_program);
		}


		static void _load_font_texture()
		{
			unsigned char* data;
			int width, hight;
			ImGuiIO& io = ImGui::GetIO();

			io.Fonts->GetTexDataAsRGBA32(&data, &width, &hight);


			glBindTexture(GL_TEXTURE_2D, imgui_font_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, hight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);


			io.Fonts->TexID = (ImTextureID)(intptr_t)imgui_font_texture;

		}

		static void _glfw_update_monitor()
		{

			// TODO Abstract this in the future.

			ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
			int monitors_count = 0;

			GLFWmonitor** glfw_monitors = glfwGetMonitors(&monitors_count);
			platform_io.Monitors.resize(0);
			for (int n = 0; n < monitors_count; n++)
			{
				ImGuiPlatformMonitor monitor;
				int x, y;
				glfwGetMonitorPos(glfw_monitors[n], &x, &y);

				const GLFWvidmode* vid_mode = glfwGetVideoMode(glfw_monitors[n]);

				monitor.MainPos = ImVec2((float)x, (float)y);
				monitor.MainSize = ImVec2((float)vid_mode->width, (float)vid_mode->height);
				int w, h;

				glfwGetMonitorWorkarea(glfw_monitors[n], &x, &y, &w, &h);
				monitor.WorkPos = ImVec2((float)x, (float)y);
				monitor.WorkSize = ImVec2((float)w, (float)h);
				monitor.MainPos = monitor.WorkPos = ImVec2((float)x, (float)y);
				monitor.MainSize = monitor.WorkSize = ImVec2((float)vid_mode->width, (float)vid_mode->height);
				// Warning: the validity of monitor DPI information on Windows depends on the application DPI awareness settings, which generally needs to be set in the manifest or at runtime.

				float x_scale, y_scale;
				glfwGetMonitorContentScale(glfw_monitors[n], &x_scale, &y_scale);
				monitor.DpiScale = x_scale;
				platform_io.Monitors.push_back(monitor);
			}

			need_update_monitor = false;

		}

		static void _glfw_monitor_callback(GLFWmonitor* monitor, int code) {
			need_update_monitor = true;
		};

		static void _glfw_new_frame()
		{
			if (need_update_monitor) _glfw_update_monitor();
			_load_font_texture();
		}

		static void _setup_render_states(ImDrawData* drawdata, int fb_width, int fb_hight, GLuint vertex_array) {



			glBindVertexArray(vertex_array);
			glUseProgram(imgui_shader_program);


			glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_hight);

			// SetUp Projection matrix
			float L = drawdata->DisplayPos.x;
			float R = drawdata->DisplayPos.x + drawdata->DisplaySize.x;
			float T = drawdata->DisplayPos.y;
			float B = drawdata->DisplayPos.y + drawdata->DisplaySize.y;
			const float ortho_projection[4][4] =
			{
				{ 2.0f / (R - L),	0.0f,			0.0f,		0.0f},
				{ 0.0f,			2.0f / (T - B),		0.0f,		0.0f },
				{ 0.0f,         0.0f,			-1.0f,		0.0f },
				{ (R + L) / (L - R),  (T + B) / (B - T),	0.0f,		1.0f },
			};

			glUniformMatrix4fv(imgui_uniforms.projection_mtx, 1, GL_FALSE, &ortho_projection[0][0]);
			glBindSampler(0, 0);



			imgui_attributes.Position = glGetAttribLocation(imgui_shader_program, "Position");
			imgui_attributes.UV = glGetAttribLocation(imgui_shader_program, "UV");
			imgui_attributes.Color = glGetAttribLocation(imgui_shader_program, "Color");



			glBindBuffer(GL_ARRAY_BUFFER, imgui_vertex_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imgui_index_buffer);

			glEnableVertexAttribArray(imgui_attributes.Position);
			glEnableVertexAttribArray(imgui_attributes.UV);
			glEnableVertexAttribArray(imgui_attributes.Color);

			glVertexAttribPointer(imgui_attributes.Position, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
			glVertexAttribPointer(imgui_attributes.UV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
			glVertexAttribPointer(imgui_attributes.Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

			
		}

		static void _draw_render_data(ImDrawData* drawdata)
		{

			int fb_width = (int)(drawdata->DisplaySize.x * drawdata->FramebufferScale.x);
			int fb_hight = (int)(drawdata->DisplaySize.y * drawdata->FramebufferScale.y);
			if (fb_width <= 0 || fb_hight <= 0) return;

			GLint backup_vertex_array;  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &backup_vertex_array);

			GLuint vertex_array = 0;
			glGenVertexArrays(1, &vertex_array);

			_setup_render_states(drawdata, fb_width, fb_hight, vertex_array);

			ImVec2 clip_offset = drawdata->DisplayPos;
			ImVec2 clip_scale = drawdata->FramebufferScale;


			for (int i = 0; i < drawdata->CmdListsCount; i++) {

				const ImDrawList* cmd_list = drawdata->CmdLists[i];

				// Fill in the buffers;

				glBindBuffer(GL_ARRAY_BUFFER, imgui_vertex_buffer);
				glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);



				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imgui_index_buffer);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);


				for (int i_cmd = 0; i_cmd < cmd_list->CmdBuffer.Size; i_cmd++) {

					const ImDrawCmd* cmd = &cmd_list->CmdBuffer[i_cmd];


					if (cmd->UserCallback != nullptr) {

						if (cmd->UserCallback == ImDrawCallback_ResetRenderState)
							_setup_render_states(drawdata, fb_width, fb_hight, vertex_array);
						else
							cmd->UserCallback(cmd_list, cmd);
						continue;
					}

					ImVec4 clip_rect;
					clip_rect.x = (cmd->ClipRect.x - clip_offset.x) * clip_scale.x;
					clip_rect.y = (cmd->ClipRect.y - clip_offset.y) * clip_scale.y;
					clip_rect.z = (cmd->ClipRect.z - clip_offset.x) * clip_scale.x;
					clip_rect.w = (cmd->ClipRect.w - clip_offset.y) * clip_scale.y;

					if (clip_rect.x < fb_width && clip_rect.y < fb_hight && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
					{

						glScissor((int)clip_rect.x, (int)(fb_hight - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));




						glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)cmd->TextureId);
						glBindBuffer(GL_ARRAY_BUFFER, imgui_vertex_buffer);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imgui_index_buffer);



						glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)cmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(cmd->IdxOffset * sizeof(ImDrawIdx)), (GLint)cmd->VtxOffset);



					}
				}
			}

			glDeleteVertexArrays(1, &vertex_array);
			glBindVertexArray(backup_vertex_array);


		}


		static void _init_platform_interface() {

			ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

			platform_io.Platform_CreateWindow = _imgui_create_window;
			platform_io.Platform_DestroyWindow = _imgui_distory_window;
			platform_io.Platform_ShowWindow = _imgui_show_window;
			platform_io.Platform_SetWindowPos = _imgui_set_window_pos;
			platform_io.Platform_GetWindowPos = _imgui_get_window_pos;
			platform_io.Platform_SetWindowSize = _imgui_set_window_size;
			platform_io.Platform_GetWindowSize = _imgui_get_window_size;
			platform_io.Platform_SetWindowFocus = _imgui_set_window_focus;
			platform_io.Platform_GetWindowFocus = _imgui_get_window_focus;
			platform_io.Platform_GetWindowMinimized = _imgui_get_window_minimized;
			platform_io.Platform_SetWindowTitle = _imgui_set_window_title;
			platform_io.Platform_RenderWindow = _imgui_render_window;
			platform_io.Platform_SwapBuffers = _imgui_swap_buffer;
			platform_io.Renderer_RenderWindow = _imgui_render_viewports;

			#if GLFW_HAS_WINDOW_ALPHA
			platform_io.Platform_SetWindowAlpha = ImGui_ImplGlfw_SetWindowAlpha;
			#endif



		};



		static void _imgui_create_window(ImGuiViewport* viewport)
		{

			ImguiViewPortData* data = IM_NEW(ImguiViewPortData);
			WindowProps props;


			props.Title = "New Viewport.";
			props.Width = viewport->Size.x;
			props.Hight = viewport->Size.y;
			props.isVisble = false;
			props.isFocusedOnCreate = false;
			props.isFocusedOnShow = false;
			props.isDecorated = (viewport->Flags & ImGuiViewportFlags_NoDecoration) ? false : true;
			props.isFloating = (viewport->Flags & ImGuiViewportFlags_TopMost) ? true : false;
			props.isVync = false;
			props.callback_fun = OnEvent;

			data->window = BaseWindow::Create(props, _get_app_main_window()->getGraphicsContex());
			data->window_owned = true;

			GLFWwindow* native_window = static_cast<GLFWwindow*>(data->window->getNativeWindow());
			viewport->PlatformUserData = static_cast<void*>(data);
			viewport->PlatformHandle = native_window;

			#ifdef _WIN32
			viewport->PlatformHandleRaw = glfwGetWin32Window(native_window);
			#endif

		};


		static void _imgui_distory_window(ImGuiViewport* viewport)
		{

			if (ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData) {

				if (data->window_owned) {
					data->window.reset();
					data->~ImguiViewPortData();
				};

			};

			viewport->PlatformHandle = nullptr;
			viewport->PlatformUserData = nullptr;
			viewport->PlatformHandleRaw = nullptr;
		};




		static void _imgui_show_window(ImGuiViewport* viewport) {

			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			#if defined(_WIN32)
			// GLFW hack: Hide icon from task bar
			HWND hwnd = (HWND)viewport->PlatformHandleRaw;
			if (viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
			{
				LONG ex_style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
				ex_style &= ~WS_EX_APPWINDOW;
				ex_style |= WS_EX_TOOLWINDOW;
				::SetWindowLong(hwnd, GWL_EXSTYLE, ex_style);
			}
			#endif
			data->window->setVisbility(true);

		};


		static void _imgui_set_window_pos(ImGuiViewport* viewport, ImVec2 pos) {
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			data->window->setPos(std::pair<int, int>(pos.x, pos.y));
		};


		static ImVec2 _imgui_get_window_pos(ImGuiViewport* viewport) {
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;

			int x_pos = data->window->getPos().first;
			int y_pos = data->window->getPos().second;

			data->ignore_window_pos_event_frame = ImGui::GetFrameCount();
			return ImVec2(x_pos, y_pos);

		};


		static void  _imgui_set_window_size(ImGuiViewport* viewport, ImVec2 size)
		{
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			data->window->setSize(size.x, size.y);
			data->ignore_window_size_event_frame = ImGui::GetFrameCount();

		};

		static ImVec2 _imgui_get_window_size(ImGuiViewport* viewport) {

			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;

			unsigned int x = data->window->getWidth();
			unsigned int y = data->window->getHight();

			return ImVec2((float)x, (float)y);
		};

		static void _imgui_set_window_focus(ImGuiViewport* viewport) {
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			data->window->FocusWindow();
		};

		static bool _imgui_get_window_focus(ImGuiViewport* viewport)
		{
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			return data->window->isFocused();
		};

		static bool _imgui_get_window_minimized(ImGuiViewport* viewport)
		{
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			return data->window->isMinimized();
		};

		static void _imgui_set_window_title(ImGuiViewport* viewport, const char* title) {
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			data->window->setTitle(title);
		};

		static void _imgui_render_window(ImGuiViewport* viewport, void* render_arg)
		{
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			data->window->getGraphicsContex()->makeContexCurrent();

			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			//glEnable(GL_SCISSOR_TEST);
			#ifdef GL_POLYGON_MODE
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			#endif

		};


		static void _imgui_swap_buffer(ImGuiViewport* viewport, void* rander_arg) {
			ImguiViewPortData* data = (ImguiViewPortData*)viewport->PlatformUserData;
			data->window->getGraphicsContex()->makeContexCurrent();
			data->window->getGraphicsContex()->SwapBuffers();
		}

		static void _imgui_render_viewports(ImGuiViewport* viewport, void* render_args)
		{
			if (!(viewport->Flags & ImGuiViewportFlags_NoRendererClear))
			{

				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

			}
			_draw_render_data(viewport->DrawData);
		}


		static void _imgui_update_contrl_keys()
		{
			ImGuiIO& io = ImGui::GetIO();

			io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
			io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
			io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		}


	}

}



















