//
// SearchBoxSample.xaml.h
// SearchBoxSample クラスの宣言
//

#pragma once

#include "SearchBoxSample.g.h"
#include "Common\NavigationHelper.h"
#include "Common\BooleanToVisibilityConverter.h" // 生成されたヘッダーによって要求されます

namespace MetroBox
{
	/// <summary>
	/// 多くのアプリケーションに共通の特性を指定する基本ページ。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SearchBoxSample sealed
	{
	public:
		SearchBoxSample();

		/// <summary>
		/// This can be changed to a strongly typed view model.
		/// </summary>
		property Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^ DefaultViewModel
		{
			Windows::Foundation::Collections::IObservableMap<Platform::String^, Platform::Object^>^  get();
		}

		/// <summary>
		/// NavigationHelper is used on each page to aid in navigation and 
		/// process lifetime management
		/// </summary>
		property Common::NavigationHelper^ NavigationHelper
		{
			Common::NavigationHelper^ get();
		}

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
		virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void LoadState(Platform::Object^ sender, Common::LoadStateEventArgs^ e);
		void SaveState(Platform::Object^ sender, Common::SaveStateEventArgs^ e);

		static Windows::UI::Xaml::DependencyProperty^ _defaultViewModelProperty;
		static Windows::UI::Xaml::DependencyProperty^ _navigationHelperProperty;
	};
}
