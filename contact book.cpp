#include <windows.h>
#include <stdio.h>

#define ID_NAME     1
#define ID_PHONE    2
#define ID_EMAIL    3
#define ID_BUTTON   4
#define ID_OUTPUT   5

#define ID_MENU_ADD     101
#define ID_MENU_VIEW    102
#define ID_MENU_EDIT    103
#define ID_MENU_DELETE  104
#define ID_MENU_EXIT    105

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

HWND hName, hPhone, hEmail, hOutput;

void AddMenus(HWND hwnd) {
    HMENU hMenubar = CreateMenu();
    HMENU hFileMenu = CreateMenu();

    AppendMenuW(hFileMenu, MF_STRING, ID_MENU_ADD, L"Add Contact");
    AppendMenuW(hFileMenu, MF_STRING, ID_MENU_VIEW, L"View Contacts");
    AppendMenuW(hFileMenu, MF_STRING, ID_MENU_EDIT, L"Edit Contact");
    AppendMenuW(hFileMenu, MF_STRING, ID_MENU_DELETE, L"Delete All Contacts");
    AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hFileMenu, MF_STRING, ID_MENU_EXIT, L"Exit");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hFileMenu, L"Menu");

    SetMenu(hwnd, hMenubar);
}

void AddControls(HWND hwnd) {
    CreateWindowW(L"STATIC", L"Name:", WS_VISIBLE | WS_CHILD,
                 50, 60, 80, 20, hwnd, NULL, NULL, NULL);
    hName = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
                         130, 60, 200, 20, hwnd, (HMENU)ID_NAME, NULL, NULL);

    CreateWindowW(L"STATIC", L"Phone:", WS_VISIBLE | WS_CHILD,
                 50, 90, 80, 20, hwnd, NULL, NULL, NULL);
    hPhone = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
                          130, 90, 200, 20, hwnd, (HMENU)ID_PHONE, NULL, NULL);

    CreateWindowW(L"STATIC", L"Email:", WS_VISIBLE | WS_CHILD,
                 50, 120, 80, 20, hwnd, NULL, NULL, NULL);
    hEmail = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER,
                          130, 120, 200, 20, hwnd, (HMENU)ID_EMAIL, NULL, NULL);

    CreateWindowW(L"BUTTON", L"Add Contact", WS_VISIBLE | WS_CHILD,
                 130, 160, 100, 30, hwnd, (HMENU)ID_BUTTON, NULL, NULL);

    hOutput = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD,
                           50, 200, 350, 25, hwnd, (HMENU)ID_OUTPUT, NULL, NULL);
}

void SaveContact(const char *name, const char *phone, const char *email) {
    FILE *f = fopen("contacts.txt", "a");
    if (f) {
        fprintf(f, "%s|%s|%s\n", name, phone, email);
        fclose(f);
    }
}

void LoadContacts() {
    FILE *f = fopen("contacts.txt", "r");
    if (!f) {
        MessageBoxW(NULL, L"No contacts found.", L"Info", MB_OK);
        return;
    }
    char line[256], all[2000] = "";
    while (fgets(line, sizeof(line), f)) {
        strcat(all, line);
    }
    fclose(f);
    MessageBoxA(NULL, all, "All Contacts", MB_OK);
}

void DeleteAllContacts() {
    FILE *f = fopen("contacts.txt", "w");
    if (f) fclose(f);
    MessageBoxW(NULL, L"All contacts deleted.", L"Deleted", MB_OK);
}

void EditFeatureNotice() {
    MessageBoxW(NULL, L"Edit feature is not implemented yet.", L"Coming Soon", MB_OK);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"ContactBookClass";
    wc.lpfnWndProc = WindowProcedure;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(L"ContactBookClass", L"M Yousuf Rehan Contact Book",
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             100, 100, 500, 400,
                             NULL, NULL, NULL, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            AddMenus(hwnd);
            AddControls(hwnd);
            break;

        case WM_COMMAND:
            switch(wParam) {
                case ID_BUTTON: {
                    char name[100], phone[100], email[100];
                    GetWindowTextA(hName, name, 100);
                    GetWindowTextA(hPhone, phone, 100);
                    GetWindowTextA(hEmail, email, 100);

                    if (strlen(name) == 0 || strlen(phone) == 0 || strlen(email) == 0) {
                        SetWindowTextW(hOutput, L"Fill all fields.");
                    } else {
                        SaveContact(name, phone, email);
                        SetWindowTextW(hOutput, L"Contact saved.");
                        SetWindowTextA(hName, "");
                        SetWindowTextA(hPhone, "");
                        SetWindowTextA(hEmail, "");
                    }
                    break;
                }
                case ID_MENU_VIEW:
                    LoadContacts();
                    break;
                case ID_MENU_DELETE:
                    DeleteAllContacts();
                    break;
                case ID_MENU_EDIT:
                    EditFeatureNotice();
                    break;
                case ID_MENU_EXIT:
                    PostQuitMessage(0);
                    break;
                case ID_MENU_ADD:
                    MessageBoxW(hwnd, L"Use the form to add a contact.", L"Info", MB_OK);
                    break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

