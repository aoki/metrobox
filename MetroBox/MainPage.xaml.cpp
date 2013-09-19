//
// MainPage.xaml.cpp
// MainPage クラスの実装。
//

#include "pch.h"
#include "MainPage.xaml.h"

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

// 空白ページのアイテム テンプレートについては、http://go.microsoft.com/fwlink/?LinkId=234238 を参照してください

MainPage::MainPage()
{
	InitializeComponent();
}

/// <summary>
/// このページがフレームに表示されるときに呼び出されます。
/// </summary>
/// <param name="e">このページにどのように到達したかを説明するイベント データ。Parameter 
/// プロパティは、通常、ページを構成するために使用します。</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// 未使用のパラメーター
}


void MetroBox::MainPage::GoToTaskPage_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	using namespace Windows::UI::Xaml::Interop;
	Frame->Navigate(TypeName(MetroBox::Task::typeid));
}


void MetroBox::MainPage::GoToDragPage_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	using namespace Windows::UI::Xaml::Interop;
	Frame->Navigate(TypeName(MetroBox::Drag::typeid));
}


void MetroBox::MainPage::GoToSearchPage_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {
	using namespace Windows::UI::Xaml::Interop;
	Frame->Navigate(TypeName(MetroBox::SearchBoxSample::typeid));
}