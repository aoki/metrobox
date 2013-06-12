//
// Drag.xaml.h
// Drag クラスの宣言
//

#pragma once

#include "Common\LayoutAwarePage.h" // 生成されたヘッダーによって要求されます
#include "Drag.g.h"

namespace MetroBox
{
	/// <summary>
	/// 多くのアプリケーションに共通の特性を指定する基本ページ。
	/// </summary>
	public ref class Drag sealed
	{
	public:
		Drag();

	protected:
		virtual void LoadState(Platform::Object^ navigationParameter,
			Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState) override;
		virtual void SaveState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState) override;

	private:
		Windows::UI::Xaml::Media::TranslateTransform^ dragTranslation;
		void Drag_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e);
	};
}
