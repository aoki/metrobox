//
// Drag.xaml.cpp
// Drag クラスの実装
//

#include "pch.h"
#include "Drag.xaml.h"

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

Drag::Drag()
{
	InitializeComponent();

	// ManipulationDelta イベントのハンドラーを追加
	TestRectangle->ManipulationDelta += ref new ManipulationDeltaEventHandler(this, &Drag::Drag_ManipulationDelta);
	dragTranslation =ref new TranslateTransform();
	TestRectangle->RenderTransform = this->dragTranslation;
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
void Drag::LoadState(Object^ navigationParameter, IMap<String^, Object^>^ pageState)
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
void Drag::SaveState(IMap<String^, Object^>^ pageState)
{
	(void) pageState;	// 未使用のパラメーター
}

void MetroBox::Drag::Drag_ManipulationDelta(Platform::Object^ sender, ManipulationDeltaRoutedEventArgs^ e) {
	dragTranslation->X += e->Delta.Translation.X;
	dragTranslation->Y += e->Delta.Translation.Y;
	VolumeSlider->Value -= e->Delta.Translation.Y / 5.0;
}