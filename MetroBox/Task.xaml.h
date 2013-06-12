//
// Task.xaml.h
// Task クラスの宣言
//

#pragma once

#include "Common\LayoutAwarePage.h" // 生成されたヘッダーによって要求されます
#include "Task.g.h"
#include <ppltasks.h>

namespace MetroBox
{
	/// <summary>
	/// 多くのアプリケーションに共通の特性を指定する基本ページ。
	/// </summary>
	public ref class Task sealed
	{
	public:
		Task();

	protected:
		virtual void LoadState(Platform::Object^ navigationParameter,
			Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState) override;
		virtual void SaveState(Windows::Foundation::Collections::IMap<Platform::String^, Platform::Object^>^ pageState) override;

	private:
		void Execute_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Cancel_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		Windows::Foundation::IAsyncOperationWithProgress<int, int>^ work;
		Concurrency::cancellation_token_source cts;

		Windows::Foundation::IAsyncAction^ LongCalulationAsync(int seconcds, concurrency::cancellation_token_source cts);
		Concurrency::cancellation_token_source cts_delay;
		Windows::Foundation::IAsyncAction^ delay;
	};
}
