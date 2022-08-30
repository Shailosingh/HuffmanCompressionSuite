#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::HuffmanCompressionApp::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        
        //Set title of window
        Title(L"Huffman Compression Suite");
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    //https://github.com/microsoft/WindowsAppSDK-Samples/blob/67cb2ca49dc1d07bcbbc6d4f869687385b0a8d73/Samples/Windowing/cpp-winui/SampleApp/MainWindow.xaml.cpp
    //https://docs.microsoft.com/en-us/windows/apps/design/controls/navigationview#code-example
    void winrt::HuffmanCompressionApp::implementation::MainWindow::HuffmanMenu_ItemInvoked(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs const& args)
    {
        if (args.InvokedItemContainer() != nullptr)
        {
            IInspectable tag = args.InvokedItemContainer().Tag();
            hstring tagValue = unbox_value<hstring>(tag);

            if (tagValue == L"Compressor")
            {
                ContentFrame().Navigate(xaml_typename<HuffmanCompressionApp::Compressor>(),*this);
            }

            else if (tagValue == L"Decompressor")
            {
                ContentFrame().Navigate(xaml_typename<HuffmanCompressionApp::Decompressor>(), *this);
            }
        }
    }
}
