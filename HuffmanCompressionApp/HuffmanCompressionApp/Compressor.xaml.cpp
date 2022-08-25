#include "pch.h"
#include "Compressor.xaml.h"
#if __has_include("Compressor.g.cpp")
#include "Compressor.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::HuffmanCompressionApp::implementation
{
    Compressor::Compressor()
    {
        InitializeComponent();
    }

    int32_t Compressor::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Compressor::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    //https://cplusplus.com/forum/windows/275617/
    //https://docs.microsoft.com/en-us/windows/win32/shell/common-file-dialog#specifying-file-types-for-a-dialog
    void winrt::HuffmanCompressionApp::implementation::Compressor::FileSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        //Setup file dialog
        IFileOpenDialog* fileOpener = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,           // Class ID for the FileOpenFialog
                                    NULL,                          //  Making a non-aggregate object
                                  CLSCTX_INPROC_SERVER,         //   Context where code is run (A DLL that runs in this process)
                                        IID_PPV_ARGS(&fileOpener));  //    Interface pointer for object
        
        //If failure, do nothing
        if (!SUCCEEDED(hr))
        {
            return;
        }
    }

    void winrt::HuffmanCompressionApp::implementation::Compressor::FolderSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {

    }
}
