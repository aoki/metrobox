//
// SearchBoxSample.xaml.cpp
// SearchBoxSample クラスの実装
//

#include "pch.h"
#include "SearchBoxSample.xaml.h"

using namespace MetroBox;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// 基本ページのアイテム テンプレートについては、http://go.microsoft.com/fwlink/?LinkId=234237 を参照してください

SearchBoxSample::SearchBoxSample()
{
	InitializeComponent();
	SetValue(_defaultViewModelProperty, ref new Map<String^,Object^>(std::less<String^>()));
	auto navigationHelper = ref new Common::NavigationHelper(this);
	SetValue(_navigationHelperProperty, navigationHelper);
	navigationHelper->LoadState += ref new Common::LoadStateEventHandler(this, &SearchBoxSample::LoadState);
	navigationHelper->SaveState += ref new Common::SaveStateEventHandler(this, &SearchBoxSample::SaveState);
}

DependencyProperty^ SearchBoxSample::_defaultViewModelProperty =
	DependencyProperty::Register("DefaultViewModel",
		TypeName(IObservableMap<String^,Object^>::typeid), TypeName(SearchBoxSample::typeid), nullptr);

/// <summary>
/// 単純なビュー モデルとして使用されるように設計されています。
/// </summary>
IObservableMap<String^, Object^>^ SearchBoxSample::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

DependencyProperty^ SearchBoxSample::_navigationHelperProperty =
	DependencyProperty::Register("NavigationHelper",
		TypeName(Common::NavigationHelper::typeid), TypeName(SearchBoxSample::typeid), nullptr);

/// <summary>
/// <see cref="NavigationHelper"/> の実装を取得します。これは、
/// 単純なビュー モデルとして使用されるように設計されています。
/// </summary>
Common::NavigationHelper^ SearchBoxSample::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

#pragma region Navigation support

/// The methods provided in this section are simply used to allow
/// NavigationHelper to respond to the page's navigation methods.
/// 
/// Page specific logic should be placed in event handlers for the  
/// <see cref="NavigationHelper::LoadState"/>
/// and <see cref="NavigationHelper::SaveState"/>.
/// The navigation parameter is available in the LoadState method 
/// in addition to page state preserved during an earlier session.

void SearchBoxSample::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void SearchBoxSample::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);
}

#pragma endregion

/// <summary>
/// このページには、移動中に渡されるコンテンツを設定します。前のセッションからページを
/// 再作成する場合は、保存状態も指定されます。
/// </summary>
/// <param name="navigationParameter">このページが最初に要求されたときに
/// <see cref="Frame::Navigate(Type, Object)"/> に渡されたパラメーター値。
/// </param>
/// <param name="pageState">前のセッションでこのページによって保存された状態のマップ。
/// ページに初めてアクセスするとき、状態は null になります。</param>
void SearchBoxSample::LoadState(Object^ sender, Common::LoadStateEventArgs^ e)
{
	(void) sender;	// 未使用のパラメーター
	(void) e;	// 未使用のパラメーター
}

/// <summary>
/// アプリケーションが中断される場合、またはページがナビゲーション キャッシュから破棄される場合、
/// このページに関連付けられた状態を保存します。値は、
/// <see cref="SuspensionManager::SessionState"/> のシリアル化の条件に準拠する必要があります。
/// </summary>
/// <param name="pageState">シリアル化可能な状態を設定される空のマップ。</param>
void SearchBoxSample::SaveState(Object^ sender, Common::SaveStateEventArgs^ e){
	(void) sender;	// 未使用のパラメーター
	(void) e; // 未使用のパラメーター
}
