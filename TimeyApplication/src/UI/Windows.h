#pragma once
#include "timey_pch.h"

namespace Timey {

	struct WindowSettings {

		uint32_t Width;
		uint32_t Hight;
		std::string Title;
	};


	class Window {

	public:
		~Window() = default;
		Window(const WindowSettings& settings);

		virtual void onUIRender() {};

		void SetVisibility(bool visible) { m_visible = visible; };
		bool IsVisible() const { return m_visible; };

		uint32_t getWidth() const { return m_settings.Width; };
		uint32_t getHight() const { return m_settings.Hight; }
		
		std::vector<Ref<Window>>& getChildrenList() { return m_children_list; };
		void PushWindow(const Ref<Window>& window);


	protected:
		void RenderAllChildren();
		WindowSettings m_settings;

	private:
		std::vector<Ref<Window>> m_children_list;
		bool m_visible = false;

	};


	class MinimalViewWindow : public Window 
	{
		
	public:
		MinimalViewWindow(const WindowSettings& settings)
			:Window(settings) {};

		~MinimalViewWindow();

		virtual void onUIRender() override;
	
	private:
		int m_clicked = 0;
	};


















}