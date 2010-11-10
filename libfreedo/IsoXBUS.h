#ifndef	ISO_3DO_HEADER
#define ISO_3DO_HEADER 0x00000001

#define XBP_INIT		0	//инициализация плагина, возвращает версию плагина
#define XBP_RESET		1	//сброс с параметром (например путь к образу)
#define XBP_SET_COMMAND	2	//XBUS
#define XBP_FIQ			3	//проверка прерывания от устройства
#define XBP_SET_DATA	4	//XBUS
#define XBP_GET_DATA	5	//XBUS
#define XBP_GET_STATUS	6	//XBUS
#define XBP_SET_POLL	7	//XBUS
#define XBP_GET_POLL	8	//XBUS
#define XBP_SELECT		9   //информирует о том что устройство выбрано оперой - не обязательно к реализации
#define XBP_RESERV		10  //резервный порт (чтение из устройства)
#define XBP_DESTROY		11  //деинициализация плагина 

#define XBP_GET_SAVESIZE	19	//для сэйвов - со стороны эмулятора
#define XBP_GET_SAVEDATA	20
#define XBP_SET_SAVEDATA	21

#ifdef XBUS_EXPORTS
#define XBUS_API __declspec(dllexport)
#else
#define XBUS_API __declspec(dllimport)
#endif

/*extern "C"
{
XBUS_API void* _xbplug_MainDevice(int proc, void* data);

#ifdef XBUS_EXPORTS
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 );
#endif
};

#ifndef XBUS_EXPORTS
#pragma comment(lib, "IsoXBUS.lib")
#endif*/

#endif 
