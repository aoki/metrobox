//
// Task.xaml.cpp
// Task クラスの実装
//

#include "pch.h"
#include "Task.xaml.h"

using namespace MetroBox;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// 基本ページのアイテム テンプレートについては、http://go.microsoft.com/fwlink/?LinkId=234237 を参照してください

Task::Task()
{
	InitializeComponent();
}

/// <summary>
/// このページには、移動中に渡されるコンテンツを設定します。前のセッションからページを
/// 再作成する場合は、保存状態も指定されます。
/// </summary>
/// <param name="navigationParameter">このページが最初に要求されたときに
/// <see cref="Frame::Navigate(Type, Object)"/> に渡されたパラメーター値。
/// </param>
/// <param name="pageState">前のセッションでこのページによって保存された状態のマップ。
/// ページに初めてアクセスするとき、状態は null になります。</param>
void Task::LoadState(Object^ navigationParameter, IMap<String^, Object^>^ pageState)
{
	(void) navigationParameter;	// 未使用のパラメーター
	(void) pageState;	// 未使用のパラメーター
}

/// <summary>
/// アプリケーションが中断される場合、またはページがナビゲーション キャッシュから破棄される場合、
/// このページに関連付けられた状態を保存します。値は、
/// <see cref="SuspensionManager::SessionState"/> のシリアル化の条件に準拠する必要があります。
/// </summary>
/// <param name="pageState">シリアル化可能な状態を設定される空のマップ。</param>
void Task::SaveState(IMap<String^, Object^>^ pageState)
{
	(void) pageState;	// 未使用のパラメーター
}

Windows::Foundation::IAsyncOperationWithProgress<int, int>^ DoSomeWorkAsync(concurrency::cancellation_token_source cts){
	return concurrency::create_async( [cts](concurrency::progress_reporter<int> progress) {
		for (int i = 0; i < 10000; i ++) {
			if(cts.get_token().is_canceled()) {
				concurrency::cancel_current_task();
			} else {
				progress.report(i);
			}
		}
		return 1;
	});
}

void Task::Execute_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	using namespace concurrency;
	using namespace Windows::Foundation;

	Execute->IsEnabled = false;
	Cancel->IsEnabled = true;

	this->Result->Text = "Start running...";

	cts = cancellation_token_source();
	work = DoSomeWorkAsync(cts);
	work->Progress = ref new AsyncOperationProgressHandler<int, int>(
		[this](IAsyncOperationWithProgress<int, int>^ info, int progressValue)
		{
			this->Progress->Value = progressValue;
		}
	);
	
	create_task( work ).then( [this](task<int> t)
	{
		try{
			t.get();
			Result->Text = "Loop finished";
			Execute->IsEnabled = true;
			Cancel->IsEnabled = false;
		} catch(task_canceled){
			Progress->Value = 0;
			Result->Text = "Cancelled";
			Cancel->IsEnabled = false;
		}
	});

	cts_delay = cancellation_token_source();
	delay = LongCalulationAsync(20, cts_delay);
	create_task( delay ).then( [this](task<void> t)
	{
		try {
			t.get();
		} catch (task_canceled) {
			auto x = 0;
		}
	});

}

void Task::Cancel_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Execute->IsEnabled = true;
	cts.cancel();
	cts_delay.cancel();
}


//void SetTimeoutOperation(int seconds, Concurrency::cancellation_token_source cts) {
//	Windows::Foundation::TimeSpan Period;
//	Period.Duration = seconds * 1000 * 10000;
//	Windows::System::Threading::ThreadPoolTimer::CreateTimer(
//		ref new Windows::System::Threading::TimerElapsedHandler([cts](Windows::System::Threading::ThreadPoolTimer^ timer){
//				cts.cancel();
//			}), Period
//		);
//}
//


IAsyncAction^ Task::LongCalulationAsync(int seconds, concurrency::cancellation_token_source cts) {
	return concurrency::create_async([seconds, cts, this]() {
		using namespace Windows::Globalization;

		Calendar^ stop = ref new Calendar;
		Calendar^ current = ref new Calendar;
		stop->SetToNow();
		stop->AddSeconds(seconds);
		do {
			if (cts.get_token().is_canceled()) {
				concurrency::cancel_current_task();
			}
			current->SetToNow();
		} while (current->GetDateTime().UniversalTime < stop->GetDateTime().UniversalTime);
	});
}