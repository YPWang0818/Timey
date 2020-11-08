#pragma once
#include "timey_pch.h"
#include "TimeyCore.h"


namespace Timey {

	class UIWindow;
	using UIWindowMapping = std::unordered_map<TimeyID, Ref<UIWindow>>;

	struct WindowUISettings {

		uint32_t Width;
		uint32_t Hight;
		std::string Title;
	};


	class UIWindow {

	public:
		~UIWindow() = default;
		UIWindow(const WindowUISettings& settings);

		virtual void SetToCurrentWindow() {};

		void SetVisibility(bool visible) { m_visible = visible; };
		bool IsVisible() const { return m_visible; };

		uint32_t getWidth() const { return m_settings.Width; };
		uint32_t getHight() const { return m_settings.Hight; };
		TimeyID getWindowID() const { return m_WindowID; };
		UIWindowMapping getChildrenList() { return m_children_list; };
		Ref<UIWindow> getChildWindowByID(const TimeyID& ID);
		Ref<UIWindow> getChildWindowByName(const std::string& name);


		void PushWindow(const Ref<UIWindow>& window);
		void onUpdate();

	protected:

		void RenderAllChildren();
		virtual void onUIRender() {};
		virtual void Begin() {};
		virtual void End() {};

		TimeyID m_WindowID;
		WindowUISettings m_settings;

	private:
		UIWindowMapping m_children_list;
		bool m_visible = false;

	};


	class MinimalViewWindow : public UIWindow
	{

	public:
		MinimalViewWindow(const WindowUISettings& settings);

		~MinimalViewWindow() = default;

		virtual void Begin() override;
		virtual void End() override;
		virtual void onUIRender() override;
		virtual void SetToCurrentWindow() override;
	private:
		TimeyID m_sessionWindowID = 0;
	};


	class StandardlViewWindow : public UIWindow
	{

	public:
		StandardlViewWindow(const WindowUISettings& settings)
			:UIWindow(settings)
		{};

		~StandardlViewWindow() = default;

		virtual void Begin() override;
		virtual void End() override;
		virtual void onUIRender() override;
		virtual void SetToCurrentWindow() override;
	};


	class SessionViewWindow : public UIWindow
	{
	public:

		SessionViewWindow(const WindowUISettings& settings)
			:UIWindow(settings)
		{
			m_title.resize(maxTitleSize);
			m_description.resize(maxDescriptionSize);

		};

		SessionViewWindow() = default;

		virtual void Begin() override;
		virtual void End() override;
		virtual void onUIRender() override;
	private:
		const static uint32_t maxTitleSize = 128;
		const static uint32_t maxDescriptionSize = 1024;
		inline const static float scrlspd = 0.5f;

		std::string m_title;
		std::string m_description;

		DateTime m_startTime;
		DateTime m_endTime;
		Time m_duration;




	};


	};


