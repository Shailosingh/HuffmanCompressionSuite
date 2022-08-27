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
    void Compressor::FileSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        //Make window unclickable (find a way to disable HuffmanMenu from page)
        

        //Setup file dialog
        IFileOpenDialog* fileOpener = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,           // Class ID for the FileOpenFialog
                                    NULL,                          //  Making a non-aggregate object
                                  CLSCTX_INPROC_SERVER,         //   Context where code is run (A DLL that runs in this process)
                                        IID_PPV_ARGS(&fileOpener));  //    Interface pointer for object
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to open the file dialog!");
            return;
        }

        //Setup the title of file dialog
        hr = fileOpener->SetTitle(L"Choose a file to compress!");
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to set title of file dialog!");
            fileOpener->Release();
            return;
        }

        //Show the dialog and wait for the selected file
        hr = fileOpener->Show(NULL);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to show the file dialog!");
            fileOpener->Release();
            return;
        }

        //Get the IShellItem (selected file from the dialog)
        IShellItem* selectedItem;
        hr = fileOpener->GetResult(&selectedItem);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to select file!");
            fileOpener->Release();
            return;
        }

        //Get the filepath out of the selected item
        wchar_t* filePathPointer = nullptr;
        hr = selectedItem->GetDisplayName(SIGDN_FILESYSPATH, &filePathPointer);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to extract path from selected path!");
            fileOpener->Release();
            selectedItem->Release();
            return;
        }

        //Set the filePathPointer to the FileDirectoryBox
        FileDirectoryBox().Text(filePathPointer);

        //Release the selected IShellItem and its DisplayName (filePathPointer)
        selectedItem->Release();
        CoTaskMemFree(filePathPointer);

        //Release the IFileDialog (fileOpener)
        fileOpener->Release();

        //Success message
        StatusBox().Text(L"Waiting to compress...");

        //Let the user click the window again
    }

    void Compressor::FolderSelectButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        //Make window unclickable (find a way to disable HuffmanMenu from page)


        //Setup file dialog
        IFileOpenDialog* folderOpener = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,           // Class ID for the FileOpenFialog
                                    NULL,                          //  Making a non-aggregate object
                                  CLSCTX_INPROC_SERVER,         //   Context where code is run (A DLL that runs in this process)
                                        IID_PPV_ARGS(&folderOpener));//    Interface pointer for object
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to open the file dialog!");
            return;
        }

        //Get the current options of the file dialog
        DWORD optionFlags;
        hr = folderOpener->GetOptions(&optionFlags);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to retrieve options for file dialog!");
            folderOpener->Release();
            return;
        }

        //Force the dialog to only allow selection of folders
        hr = folderOpener->SetOptions(optionFlags | FOS_PICKFOLDERS);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to set options for file dialog!");
            folderOpener->Release();
            return;
        }

        //Setup the title of file dialog
        hr = folderOpener->SetTitle(L"Choose a folder for compressed file to be placed in");
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to set title of file dialog!");
            folderOpener->Release();
            return;
        }

        //Show the dialog and wait for the selected file
        hr = folderOpener->Show(NULL);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to show the file dialog!");
            folderOpener->Release();
            return;
        }

        //Get the IShellItem (selected file from the dialog)
        IShellItem* selectedItem;
        hr = folderOpener->GetResult(&selectedItem);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to select folder!");
            folderOpener->Release();
            return;
        }

        //Get the folder path out of the selected item
        wchar_t* folderPathPointer = nullptr;
        hr = selectedItem->GetDisplayName(SIGDN_FILESYSPATH, &folderPathPointer);
        if (!SUCCEEDED(hr))
        {
            StatusBox().Text(L"Unable to extract path from selected path!");
            folderOpener->Release();
            selectedItem->Release();
            return;
        }

        //Set the folderPathPointer to the FolderDirectoryBox
        FolderDirectoryBox().Text(folderPathPointer);

        //Release the selected IShellItem and its DisplayName (filePathPointer)
        selectedItem->Release();
        CoTaskMemFree(folderPathPointer);

        //Release the IFileDialog (folderOpener)
        folderOpener->Release();

        //Success message
        StatusBox().Text(L"Waiting to compress...");

        //Let the user click the window again
    }
}