//
// NavigationHelper.cpp
// NavigationHelper および関連するクラスの実装
//

#include "pch.h"
#include "NavigationHelper.h"
#include "RelayCommand.h"
#include "SuspensionManager.h"

using namespace MetroBox::Common;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;

/// <summary>
/// Initializes a new instance of the <see cref="LoadStateEventArgs"/> class.
/// </summary>
/// <param name="navigationParameter">
/// The parameter value passed to <see cref="Frame->Navigate(Type, Object)"/> 
/// when this page was initially requested.
/// </param>
/// <param name="pageState">
/// A dictionary of state preserved by this page during an earlier
/// session.  This will be null the first time a page is visited.
/// </param>
LoadStateEventArgs::LoadStateEventArgs(Object^ navigationParameter, IMap<String^, Object^>^ pageState)
{
	_navigationParameter = navigationParameter;
	_pageState = pageState;
}

/// <summary>
/// Gets the <see cref="NavigationParameter"/> property of <see cref"LoadStateEventArgs"/> class.
/// </summary>
Object^ LoadStateEventArgs::NavigationParameter::get()
{
	return _navigationParameter;
}

/// <summary>
/// Gets the <see cref="PageState"/> property of <see cref"LoadStateEventArgs"/> class.
/// </summary>
IMap<String^, Object^>^ LoadStateEventArgs::PageState::get()
{
	return _pageState;
}

/// <summary>
/// Initializes a new instance of the <see cref="SaveStateEventArgs"/> class.
/// </summary>
/// <param name="pageState">An empty dictionary to be populated with serializable state.</param>
SaveStateEventArgs::SaveStateEventArgs(IMap<String^, Object^>^ pageState)
{
	_pageState = pageState;
}

/// <summary>
/// Gets the <see cref="PageState"/> property of <see cref"SaveStateEventArgs"/> class.
/// </summary>
IMap<String^, Object^>^ SaveStateEventArgs::PageState::get()
{
	return _pageState;
}

/// <summary>
/// Initializes a new instance of the <see cref="NavigationHelper"/> class.
/// </summary>
/// <param name="page">A reference to the current page used for navigation.  
/// This reference allows for frame manipulation and to ensure that keyboard 
/// navigation requests only occur when the page is occupying the entire window.</param>
NavigationHelper::NavigationHelper(Page^ page, RelayCommand^ goBack, RelayCommand^ goForward) :
	_page(page),
	_goBackCommand(goBack),
	_goForwardCommand(goForward)
{
	// このページがビジュアル ツリーの一部である場合、次の 2 つの変更を行います: 
	// 1) アプリケーションのビューステートをページの表示状態にマップする
	// 2) キーボードおよびマウスのナビゲーション要求を処理する
	_loadedEventToken = page->Loaded += ref new RoutedEventHandler(this, &NavigationHelper::OnLoaded);

	//// ページが表示されない場合、同じ変更を元に戻します
	_unloadedEventToken = page->Unloaded += ref new RoutedEventHandler(this, &NavigationHelper::OnUnloaded);
}

/// <summary>
/// ページがビジュアル ツリーの一部である場合に呼び出されます
/// </summary>
/// <param name="sender">イベントをトリガーしたインスタンス。</param>
/// <param name="e">イベントが発生する条件を説明するイベント データ。</param>
void NavigationHelper::OnLoaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// キーボードおよびマウスのナビゲーションは、ウィンドウ全体を使用する場合のみ適用されます
	if (_page->ActualHeight == Window::Current->Bounds.Height &&
		_page->ActualWidth == Window::Current->Bounds.Width)
	{
		// ウィンドウで直接待機するため、フォーカスは不要です
		_acceleratorKeyEventToken = Window::Current->CoreWindow->Dispatcher->AcceleratorKeyActivated +=
			ref new TypedEventHandler<CoreDispatcher^, AcceleratorKeyEventArgs^>(this,
				&NavigationHelper::CoreDispatcher_AcceleratorKeyActivated);

		_pointerPressedEventToken = Window::Current->CoreWindow->PointerPressed +=
			ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this,
				&NavigationHelper::CoreWindow_PointerPressed);

		_navigationShortcutsRegistered = true;
	}
}

/// <summary>
/// ページがビジュアル ツリーから削除される場合に呼び出されます
/// </summary>
/// <param name="sender">イベントをトリガーしたインスタンス。</param>
/// <param name="e">イベントが発生する条件を説明するイベント データ。</param>
void NavigationHelper::OnUnloaded(Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (_navigationShortcutsRegistered)
	{
		Window::Current->CoreWindow->Dispatcher->AcceleratorKeyActivated -= _acceleratorKeyEventToken;
		Window::Current->CoreWindow->PointerPressed -= _pointerPressedEventToken;
		_navigationShortcutsRegistered = false;
	}

	//ハンドラーを削除し、ページへの参照を解放します
	_page->Loaded -= _loadedEventToken;
	_page->Unloaded -= _unloadedEventToken;

	delete _goBackCommand;
	delete _goForwardCommand;

	_page = nullptr;
}

#pragma region Navigation support

/// <summary>
/// Method used by the <see cref="GoBackCommand"/> property
/// to determine if the <see cref="Frame"/> can go back.
/// </summary>
/// <returns>
/// true if the <see cref="Frame"/> has at least one entry 
/// in the back navigation history.
/// </returns>
bool NavigationHelper::CanGoBack()
{
	auto frame = _page->Frame;
	return (frame != nullptr && frame->CanGoBack);
}

/// <summary>
/// Method used by the <see cref="GoBackCommand"/> property
/// to invoke the <see cref="Windows::UI::Xaml::Controls::Frame::GoBack"/> method.
/// </summary>
void NavigationHelper::GoBack()
{
	auto frame = _page->Frame;
	if (frame != nullptr)
	{
		frame->GoBack();
	}
}

/// <summary>
/// Method used by the <see cref="GoForwardCommand"/> property
/// to determine if the <see cref="Frame"/> can go forward.
/// </summary>
/// <returns>
/// true if the <see cref="Frame"/> has at least one entry 
/// in the forward navigation history.
/// </returns>
bool NavigationHelper::CanGoForward()
{
	auto frame = _page->Frame;
	return (frame != nullptr && frame->CanGoForward);
}

/// <summary>
/// Method used by the <see cref="GoForwardCommand"/> property
/// to invoke the <see cref="Windows::UI::Xaml::Controls::Frame::GoBack"/> method.
/// </summary>
void NavigationHelper::GoForward()
{
	auto frame = _page->Frame;
	if (frame != nullptr)
	{
		frame->GoForward();
	}
}

/// <summary>
/// Gets the <see cref="GoBackCommand"/> property of <see cref"NavigationHelper"/> class.
/// </summary>
RelayCommand^ NavigationHelper::GoBackCommand::get()
{
	if (_goBackCommand == nullptr)
	{
		_goBackCommand = ref new RelayCommand(
			[this](Object^) -> bool
			{
				return CanGoBack();
			},
			[this](Object^) -> void
			{
				GoBack();
			}
		);
	}
	return _goBackCommand;
}

/// <summary>
/// Gets the <see cref="GoForwardCommand"/> property of <see cref"NavigationHelper"/> class.
/// </summary>
RelayCommand^ NavigationHelper::GoForwardCommand::get()
{
	if (_goForwardCommand == nullptr)
	{
		_goForwardCommand = ref new RelayCommand(
			[this](Object^) -> bool
			{
				return CanGoForward();
			},
			[this](Object^) -> void
			{
				GoForward();
			}
		);
	}
	return _goForwardCommand;
}

/// <summary>
/// このページがアクティブで、ウィンドウ全体を使用する場合、Alt キーの組み合わせなどのシステム キーを含む、
/// キーボード操作で呼び出されます。 ページがフォーカスされていないときでも、
/// ページ間のキーボード ナビゲーションの検出に使用されます。
/// </summary>
/// <param name="sender">イベントをトリガーしたインスタンス。</param>
/// <param name="e">イベントが発生する条件を説明するイベント データ。</param>
void NavigationHelper::CoreDispatcher_AcceleratorKeyActivated(CoreDispatcher^ sender,
	AcceleratorKeyEventArgs^ e)
{
	sender; // 未使用のパラメーター
	auto virtualKey = e->VirtualKey;

	// 左方向キーや右方向キー、または専用に設定した前に戻るキーや次に進むキーを押した場合のみ、
	// 詳細を調査します
	if ((e->EventType == CoreAcceleratorKeyEventType::SystemKeyDown ||
		e->EventType == CoreAcceleratorKeyEventType::KeyDown) &&
		(virtualKey == VirtualKey::Left || virtualKey == VirtualKey::Right ||
		virtualKey == VirtualKey::GoBack || virtualKey == VirtualKey::GoForward))
	{
		auto coreWindow = Window::Current->CoreWindow;
		auto downState = Windows::UI::Core::CoreVirtualKeyStates::Down;
		bool menuKey = (coreWindow->GetKeyState(VirtualKey::Menu) & downState) == downState;
		bool controlKey = (coreWindow->GetKeyState(VirtualKey::Control) & downState) == downState;
		bool shiftKey = (coreWindow->GetKeyState(VirtualKey::Shift) & downState) == downState;
		bool noModifiers = !menuKey && !controlKey && !shiftKey;
		bool onlyAlt = menuKey && !controlKey && !shiftKey;

		if ((virtualKey == VirtualKey::GoBack && noModifiers) ||
			(virtualKey == VirtualKey::Left && onlyAlt))
		{
			// 前に戻るキーまたは Alt キーを押しながら左方向キーを押すと前に戻ります
			e->Handled = true;
			GoBackCommand->Execute(this);
		}
		else if ((virtualKey == VirtualKey::GoForward && noModifiers) ||
			(virtualKey == VirtualKey::Right && onlyAlt))
		{
			// 次に進むキーまたは Alt キーを押しながら右方向キーを押すと次に進みます
			e->Handled = true;
			GoForwardCommand->Execute(this);
		}
	}
}

/// <summary>
/// このページがアクティブで、ウィンドウ全体を使用する場合、マウスのクリック、タッチ スクリーンのタップなどの
/// 操作で呼び出されます。 ページ間を移動するため、マウス ボタンのクリックによるブラウザー スタイルの
/// 次に進むおよび前に戻る操作の検出に使用されます。
/// </summary>
/// <param name="sender">イベントをトリガーしたインスタンス。</param>
/// <param name="e">イベントが発生する条件を説明するイベント データ。</param>
void NavigationHelper::CoreWindow_PointerPressed(CoreWindow^ sender, PointerEventArgs^ e)
{
	auto properties = e->CurrentPoint->Properties;

	// 左、右、および中央ボタンを使用したボタン操作を無視します
	if (properties->IsLeftButtonPressed || 
		properties->IsRightButtonPressed ||
		properties->IsMiddleButtonPressed)
	{
		return;
	}

	// [戻る] または [進む] を押すと適切に移動します (両方同時には押しません)
	bool backPressed = properties->IsXButton1Pressed;
	bool forwardPressed = properties->IsXButton2Pressed;
	if (backPressed ^ forwardPressed)
	{
		e->Handled = true;
		if (backPressed)
		{
			if (GoBackCommand->CanExecute(this))
			{
				GoBackCommand->Execute(this);
			}
		}
		else
		{
			if (GoForwardCommand->CanExecute(this))
			{
				GoForwardCommand->Execute(this);
			}
		}
	}
}

#pragma endregion

#pragma region Process lifetime management

/// <summary>
/// このページがフレームに表示されるときに呼び出されます。
/// </summary>
/// <param name="e">このページにどのように到達したかを説明するイベント データ。 Parameter 
/// プロパティは、表示するグループを示します。</param>
void NavigationHelper::OnNavigatedTo(NavigationEventArgs^ e)
{
	auto frameState = SuspensionManager::SessionStateForFrame(_page->Frame);
	_pageKey = "Page-" + _page->Frame->BackStackDepth;

	if (e->NavigationMode == NavigationMode::New)
	{
		// 新しいページをナビゲーション スタックに追加するとき、次に進むナビゲーションの
		// 既存の状態をクリアします
		auto nextPageKey = _pageKey;
		int nextPageIndex = _page->Frame->BackStackDepth;
		while (frameState->HasKey(nextPageKey))
		{
			frameState->Remove(nextPageKey);
			nextPageIndex++;
			nextPageKey = "Page-" + nextPageIndex;
		}

		// ナビゲーション パラメーターを新しいページに渡します
		LoadState(this, ref new LoadStateEventArgs(e->Parameter, nullptr));
	}
	else
	{
		// ナビゲーション パラメーターおよび保存されたページの状態をページに渡します。
		// このとき、中断状態の読み込みや、キャッシュから破棄されたページの再作成と同じ対策を
		// 使用します
		LoadState(this, ref new LoadStateEventArgs(e->Parameter, safe_cast<IMap<String^, Object^>^>(frameState->Lookup(_pageKey))));
	}
}

/// <summary>
/// このページがフレームに表示されなくなるときに呼び出されます。
/// </summary>
/// <param name="e">このページにどのように到達したかを説明するイベント データ。 Parameter 
/// プロパティは、表示するグループを示します。</param>
void NavigationHelper::OnNavigatedFrom(NavigationEventArgs^ e)
{
	auto frameState = SuspensionManager::SessionStateForFrame(_page->Frame);
	auto pageState = ref new Map<String^, Object^>();
	SaveState(this, ref new SaveStateEventArgs(pageState));
	frameState->Insert(_pageKey, pageState);
}
#pragma endregion
