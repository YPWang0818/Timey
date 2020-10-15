#pragma once
#include "timey_pch.h"
#include "TimeyCore.h"

namespace Timey {

	struct WindowUISettings {

		uint32_t Width;
		uint32_t Hight;
		std::string Title;
	};


	class UIWindow {

	public:
		~UIWindow() = default;
		UIWindow(const WindowUISettings& settings);

		virtual void onUIRender() {};

		void SetVisibility(bool visible) { m_visible = visible; };
		bool IsVisible() const { return m_visible; };

		uint32_t getWidth() const { return m_settings.Width; };
		uint32_t getHight() const { return m_settings.Hight; };
		TimeyID getWindowID() const { return m_WindowID; };

		
		std::vector<Ref<UIWindow>>& getChildrenList() { return m_children_list; };
		void PushWindow(const Ref<UIWindow>& window);


	protected:
		void RenderAllChildren();
		TimeyID m_WindowID;
		WindowUISettings m_settings;

	private:
		std::vector<Ref<UIWindow>> m_children_list;
		bool m_visible = false;

	};


	class MinimalViewWindow : public UIWindow
	{

	public:
		MinimalViewWindow(const WindowUISettings& settings)
			:UIWindow(settings)
		{};

		~MinimalViewWindow();

		virtual void onUIRender() override;

	};


	class StandardlViewWindow : public UIWindow
	{

	public:
		StandardlViewWindow(const WindowUISettings& settings)
			:UIWindow(settings)
		{};

		~StandardlViewWindow();

		virtual void onUIRender() override;

	};















}