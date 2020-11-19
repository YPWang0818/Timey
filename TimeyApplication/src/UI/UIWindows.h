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

		std::function<void(void*)> onShowCallback = nullptr;
		std::function<void(void*)> onHideCallback = nullptr;
	};


	class UIWindow {

	public:
		~UIWindow() = default;
		UIWindow(const WindowUISettings& settings);

		virtual void SetToCurrentWindow() {};

		void SetVisibility(bool visible, void* data = nullptr);
		bool IsVisible() const { return m_visible; };

		uint32_t getWidth() const { return m_settings.Width; };
		uint32_t getHight() const { return m_settings.Hight; };
		TimeyID getWindowID() const { return m_WindowID; };
		UIWindowMapping getChildrenList() { return m_children_list; };
		Ref<UIWindow> getChildWindowByID(const TimeyID& ID);
		Ref<UIWindow> getChildWindowByName(const std::string& name);


		void PushWindow(const Ref<UIWindow>& window);
		void onUpdate();

		virtual void onWindowShow(void* data) {};
		virtual void onWindowHide(void* data) {};
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


	class StandardViewWindow : public UIWindow
	{

	public:
		StandardViewWindow(const WindowUISettings& settings);
		

		~StandardViewWindow() = default;

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
			m_flag = InternalFlags::brandnew_session;

			m_settings.onShowCallback = TIMEY_BIND_CALLBACK(SessionViewWindow::onWindowShow);
			m_settings.onHideCallback = TIMEY_BIND_CALLBACK(SessionViewWindow::onWindowHide);

		};

		SessionViewWindow() = default;

		virtual void Begin() override;
		virtual void End() override;
		virtual void onUIRender() override;

		virtual void onWindowShow(void* data) override;
		virtual void onWindowHide(void* data) override;

		void setToCachedOnShow() { m_flag = InternalFlags::cached_session; };
		void setToNewOnShow() { m_flag = InternalFlags::brandnew_session; };


	private:

		void pushNewSession();
		int sanitizeInput();
		void loadSession();
		void loadNewSession();

	private:
		const static uint32_t maxTitleSize = 128;
		const static uint32_t maxDescriptionSize = 1024;
		inline const static float scrlspd = 0.5f;

		std::string m_title;
		std::string m_description;

		DateTime m_startTime;
		DateTime m_endTime;
		Time m_duration;


		enum class InternalFlags {
			brandnew_session,
			cached_session
		} m_flag;

	};


	};


