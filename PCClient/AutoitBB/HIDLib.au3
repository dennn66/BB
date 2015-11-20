#include <StructureConstants.au3>
#Include <WinAPI.au3>


Global Const $DIGCF_DEFAULT          = 0x00000001  ;// only valid with DIGCF_DEVICEINTERFACE
Global Const $DIGCF_PRESENT          = 0x00000002
Global Const $DIGCF_ALLCLASSES       = 0x00000004
Global Const $DIGCF_PROFILE          = 0x00000008
Global Const $DIGCF_DEVICEINTERFACE  = 0x00000010

		;/** Constant for a keyboard report modifier byte, indicating that the keyboard's left control key is currently pressed. */
Global Const $HID_KEYBOARD_MODIFIER_NONE             =           0
		;/** Constant for a keyboard report modifier byte, indicating that the keyboard's left control key is currently pressed. */
Global Const $HID_KEYBOARD_MODIFIER_LEFTCTRL         =           1
		;/** Constant for a keyboard report modifier byte, indicating that the keyboard's left shift key is currently pressed. */
Global Const $HID_KEYBOARD_MODIFIER_LEFTSHIFT        =           2
		;/** Constant for a keyboard report modifier byte, indicating that the keyboard's left alt key is currently pressed. */
Global Const $HID_KEYBOARD_MODIFIER_LEFTALT          =           4
		;/** Constant for a keyboard report modifier byte, indicating that the keyboard's left GUI key is currently pressed. */
Global Const $HID_KEYBOARD_MODIFIER_LEFTGUI          =           8
		;/** Constant for a keyboard report modifier byte, indicating that the keyboard's right control key is currently pressed. */
Global Const $HID_KEYBOARD_MODIFIER_RIGHTCTRL        =           16
		;/** Constant for a keyboard report modifier byte, indicating that the keyboard's right shift key is currently pressed. */
Global Const $HID_KEYBOARD_MODIFIER_RIGHTSHIFT       =           32
		;/** Constant for a keyboard report modifier byte, indicating that the keyboard's right alt key is currently pressed. */
Global Const $HID_KEYBOARD_MODIFIER_RIGHTALT         =           64
		;/** Constant for a keyboard report modifier byte, indicating that the keyboard's right GUI key is currently pressed. */
Global Const $HID_KEYBOARD_MODIFIER_RIGHTGUI         =           128


Global Const $SPDRP_DEVICEDESC                 = (0x00000000)  ;// DeviceDesc (R/W)
Global Const $SPDRP_HARDWAREID                 = (0x00000001)  ;// HardwareID (R/W)
Global Const $SPDRP_COMPATIBLEIDS              = (0x00000002)  ;// CompatibleIDs (R/W)
Global Const $SPDRP_UNUSED0                    = (0x00000003)  ;// unused
Global Const $SPDRP_SERVICE                    = (0x00000004)  ;// Service (R/W)
Global Const $SPDRP_UNUSED1                    = (0x00000005)  ;// unused
Global Const $SPDRP_UNUSED2                    = (0x00000006)  ;// unused
Global Const $SPDRP_CLASS                      = (0x00000007)  ;// Class (R--tied to ClassGUID)
Global Const $SPDRP_CLASSGUID                  = (0x00000008)  ;// ClassGUID (R/W)
Global Const $SPDRP_DRIVER                     = (0x00000009)  ;// Driver (R/W)
Global Const $SPDRP_CONFIGFLAGS                = (0x0000000A)  ;// ConfigFlags (R/W)
Global Const $SPDRP_MFG                        = (0x0000000B)  ;// Mfg (R/W)
Global Const $SPDRP_FRIENDLYNAME               = (0x0000000C)  ;// FriendlyName (R/W)
Global Const $SPDRP_LOCATION_INFORMATION       = (0x0000000D)  ;// LocationInformation (R/W)
Global Const $SPDRP_PHYSICAL_DEVICE_OBJECT_NAME= (0x0000000E)  ;// PhysicalDeviceObjectName ®
Global Const $SPDRP_CAPABILITIES               = (0x0000000F)  ;// Capabilities ®
Global Const $SPDRP_UI_NUMBER                  = (0x00000010)  ;// UiNumber ®
Global Const $SPDRP_UPPERFILTERS               = (0x00000011)  ;// UpperFilters (R/W)
Global Const $SPDRP_LOWERFILTERS               = (0x00000012)  ;// LowerFilters (R/W)
Global Const $SPDRP_BUSTYPEGUID                = (0x00000013)  ;// BusTypeGUID ®
Global Const $SPDRP_LEGACYBUSTYPE              = (0x00000014)  ;// LegacyBusType ®
Global Const $SPDRP_BUSNUMBER                  = (0x00000015)  ;// BusNumber ®
Global Const $SPDRP_ENUMERATOR_NAME            = (0x00000016)  ;// Enumerator Name ®
Global Const $SPDRP_SECURITY                   = (0x00000017)  ;// Security (R/W, binary form)
Global Const $SPDRP_SECURITY_SDS               = (0x00000018)  ;// Security (W, SDS form)
Global Const $SPDRP_DEVTYPE                    = (0x00000019)  ;// Device Type (R/W)
Global Const $SPDRP_EXCLUSIVE                  = (0x0000001A)  ;// Device is exclusive-access (R/W)
Global Const $SPDRP_CHARACTERISTICS            = (0x0000001B)  ;// Device Characteristics (R/W)
Global Const $SPDRP_ADDRESS                    = (0x0000001C)  ;// Device Address ®
Global Const $SPDRP_UI_NUMBER_DESC_FORMAT      = (0X0000001D)  ;// UiNumberDescFormat (R/W)
Global Const $SPDRP_DEVICE_POWER_DATA          = (0x0000001E)  ;// Device Power Data ®
Global Const $SPDRP_REMOVAL_POLICY             = (0x0000001F)  ;// Removal Policy ®
Global Const $SPDRP_REMOVAL_POLICY_HW_DEFAULT  = (0x00000020)  ;// Hardware Removal Policy ®
Global Const $SPDRP_REMOVAL_POLICY_OVERRIDE    = (0x00000021)  ;// Removal Policy Override (RW)
Global Const $SPDRP_INSTALL_STATE              = (0x00000022)  ;// Device Install State ®
Global Const $SPDRP_LOCATION_PATHS             = (0x00000023)  ;// Device Location Paths ®

Global Const $SPDRP_MAXIMUM_PROPERTY           = (0x00000024)  ;// Upper bound on ordinals

Global Const $IOCTL_STORAGE_GET_DEVICE_NUMBER = 0x2D1080
Global Const $DRIVE_REMOVABLE = 2
Global Const $DRIVE_FIXED = 3
Global Const $DRIVE_CDROM = 5
Global Const $CR_SUCCESS = 0x0
Global Const $DN_REMOVABLE = 0x4000
Global Const $CM_REMOVE_NO_RESTART = 0x2
Global Const $CR_ACCESS_DENIED = 0x33
Global Const $PNP_VetoTypeUnknown = 0 ; Name is unspecified
Global Const $PNP_VetoLegacyDevice = 1 ; Name is an Instance Path
Global Const $PNP_VetoPendingClose = 2 ; Name is an Instance Path
Global Const $PNP_VetoWindowsApp = 3 ; Name is a Module
Global Const $PNP_VetoWindowsService = 4 ; Name is a Service
Global Const $PNP_VetoOutstandingOpen = 5 ; Name is an Instance Path
Global Const $PNP_VetoDevice = 6 ; Name is an Instance Path
Global Const $PNP_VetoDriver = 7 ; Name is a Driver Service Name
Global Const $PNP_VetoIllegalDeviceRequest = 8 ; Name is an Instance Path
Global Const $PNP_VetoInsufficientPower = 9 ; Name is unspecified
Global Const $PNP_VetoNonDisableable = 10 ; Name is an Instance Path
Global Const $PNP_VetoLegacyDriver = 11 ; Name is a Service
Global Const $PNP_VetoInsufficientRights = 12 ; Name is unspecified

Global Const $CMD_WRITE_CONFIG = 0 ;
Global Const $CMD_READ_CONFIG = 1 ;
Global Const $CMD_ADD_NODE = 2 ;
Global Const $CMD_DELETE_NODE = 3 ;
Global Const $CMD_DELETE_ALL = 4 ;
Global Const $CMD_SET_MODIFIER = 5 ;
Global Const $CMD_SET_HPCPMP = 6 ;
Global Const $CMD_ADD_NODE_CONDITION = 7 ;

Global Const $t_mobHP = 0 ;/** Mob HP */
Global Const $t_playerHP = 1 ;/** Player HP */
Global Const $t_playerCP = 2 ;/** Player CP */
Global Const $t_playerMP = 3 ;/** Player MP */

; Structures
;Global Const $STORAGE_DEVICE_NUMBER = "ulong DeviceType;ulong DeviceNumber;ulong PartitionNumber"
Global Const $SP_DEV_BUF = "byte[2052]"
Global Const $SP_DEVICE_INTERFACE_DETAIL_DATA = "dword cbSize;wchar DevicePath[1024]" ; created at SP_DEV_BUF ptr
Global Const $SP_DEVICE_INTERFACE_DATA = "dword cbSize;byte InterfaceClassGuid[16];dword Flags;ulong_ptr Reserved" ; GUID struct = 16 bytes
Global Const $SP_DEVINFO_DATA = "dword cbSize;byte ClassGuid[16];dword DevInst;ulong_ptr Reserved"

; http://hg.dsource.org/projects/ddbg_continued/rev/4a9dcbd9e54f
; + const IID GUID_HIDClass = {0x745A17A0, 0x74D3, 0x11D0, [0xB6, 0xFE, 0x00, 0xA0, 0xC9, 0x0F, 0x57, 0xDA]};
; + const IID GUID_Joystick = {0x6F1D2B70, 0xD5A0, 0x11CF, [0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00]};
; + const IID GUID_KeyboardClass = {0x4D36E96B, 0xE325, 0x11CE, [0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18]};
; + const IID GUID_LEGACY_DEVICE_DETECTION_STANDAR = {0x50FEB0DE, 0x596A, 0x11D2, [0xA5, 0xB8, 0x00, 0x00, 0xF8, 0x1A, 0x46, 0x19]};
; + const IID GUID_MouseClass = {0x4D36E96F, 0xE325, 0x11CE, [0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18]};
;+ const IID GUID_DEVCLASS_USB = {0x36FC9E60, 0xC465, 0x11CF, [0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00]};
;+ const IID GUID_HIDClass = {0x745A17A0, 0x74D3, 0x11D0, [0xB6, 0xFE, 0x00, 0xA0, 0xC9, 0x0F, 0x57, 0xDA]};
;+ const IID GUID_KeyboardClass = {0x4D36E96B, 0xE325, 0x11CE, [0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18]};
Global Const $guidHIDClass = DllStructCreate($tagGUID)
DllStructSetData($guidHIDClass, "Data1", 0x745A17A0)
DllStructSetData($guidHIDClass, "Data2", 0x74D3)
DllStructSetData($guidHIDClass, "Data3", 0x11D0)
DllStructSetData($guidHIDClass, "Data4", Binary("0xB6FE00A0C90F57DA"))

$guidMouse = DllStructCreate($tagGUID)
DllStructSetData($guidMouse, "Data1", 0x4D36E96F)
DllStructSetData($guidMouse, "Data2", 0xE325)
DllStructSetData($guidMouse, "Data3", 0x11CE)
DllStructSetData($guidMouse, "Data4", Binary("0xBFC108002BE10318"))

$guidKeyboard = DllStructCreate($tagGUID)
DllStructSetData($guidKeyboard, "Data1", 0x50FEB0DE)
DllStructSetData($guidKeyboard, "Data2", 0x596A)
DllStructSetData($guidKeyboard, "Data3", 0x11D2)
DllStructSetData($guidKeyboard, "Data4", Binary("0xA5B80000F81A4619"))
Global Const $guidDisk = DllStructCreate($tagGUID)
DllStructSetData($guidDisk, "Data1", 0x53f56307)
DllStructSetData($guidDisk, "Data2", 0xb6bf)
DllStructSetData($guidDisk, "Data3", 0x11d0)
DllStructSetData($guidDisk, "Data4", Binary("0x94f200a0c91efb8b"))
Global Const $guidCDROM = DllStructCreate($tagGUID)
DllStructSetData($guidCDROM, "Data1", 0x53f56308)
DllStructSetData($guidCDROM, "Data2", 0xb6bf)
DllStructSetData($guidCDROM, "Data3", 0x11d0)
DllStructSetData($guidCDROM, "Data4", Binary("0x94f200a0c91efb8b"))
Global Const $guidFloppy = DllStructCreate($tagGUID)
DllStructSetData($guidFloppy, "Data1", 0x53f56311)
DllStructSetData($guidFloppy, "Data2", 0xb6bf)
DllStructSetData($guidFloppy, "Data3", 0x11d0)
DllStructSetData($guidFloppy, "Data4", Binary("0x94f200a0c91efb8b"))



Global Const $tagHIDD_ATTRIBUTES = _
  'ULONG  Size;' & _
  'USHORT VendorID;' & _
  'USHORT ProductID;' & _
  'USHORT VersionNumber;'

Global Const $tagREPORT_BUF_STRUCT = _
	'byte reportID;' & _
	'byte modKey;' & _
	'byte reserved;' & _
	'byte keys[6];'

Global Const $tagHIDP_CAPS = _
  'uInt  Usage;' & _
  'uInt  UsagePage;' & _
  'USHORT InputReportByteLength;' & _
  'USHORT OutputReportByteLength;' & _
  'USHORT FeatureReportByteLength;' & _
  'USHORT Reserved[17];' & _
  'USHORT NumberLinkCollectionNodes;' & _
  'USHORT NumberInputButtonCaps;' & _
  'USHORT NumberInputValueCaps;' & _
  'USHORT NumberInputDataIndices;' & _
  'USHORT NumberOutputButtonCaps;' & _
  'USHORT NumberOutputValueCaps;' & _
  'USHORT NumberOutputDataIndices;' & _
  'USHORT NumberFeatureButtonCaps;' & _
  'USHORT NumberFeatureValueCaps;' & _
  'USHORT NumberFeatureDataIndices;'

Global Const $INTERFACE_ID_Keyboard = 0   ; /**< Keyboard interface descriptor ID */
Global Const $INTERFACE_ID_Mouse    = 1   ; /**< Mouse interface descriptor ID */
Global Const $INTERFACE_ID_GenericHID = 2 ; /**< GenericHID interface descriptor ID */

Global $MAXKEYS = 219

Dim  $KeyMnemonic[$MAXKEYS] = ["ERROR_ROLLOVER", 	"POST_FAIL", 	"ERROR_UNDEFINED", 	"A", 	"B", 	"C", 	"D", 	"E", 	"F", 	"G", 	"H", 	"I", 	"J", 	"K", 	"L", 	"M", 	"N", 	"O", 	"P", 	"Q", 	"R", 	"S", 	"T", 	"U", 	"V", 	"W", 	"X", 	"Y", 	"Z", 	"1", 	"2", 	"3", 	"4", 	"5", 	"6", 	"7", 	"8", 	"9", 	"0", 	"ENTER", 	"ESCAPE", 	"BACKSPACE", 	"TAB", 	"SPACE", 	"-", 	"=", 	"[", 	"]", 	"\", 	"`", 	";", 	"", 	"GRAVE_ACCENT_AND_TILDE", 	",", 	".", 	"/", 	"CAPS_LOCK", 	"F1", 	"F2", 	"F3", 	"F4", 	"F5", 	"F6", 	"F7", 	"F8", 	"F9", 	"F10", 	"F11", 	"F12", 	"PRINT_SCREEN", 	"SCROLL_LOCK", 	"PAUSE", 	"INSERT", 	"HOME", 	"PAGE_UP", 	"DELETE", 	"END", 	"PAGE_DOWN", 	"RIGHT_ARROW", 	"LEFT_ARROW", 	"DOWN_ARROW", 	"UP_ARROW", 	"NUM_LOCK", 	"KEYPAD_SLASH", 	"KEYPAD_ASTERISK", 	"KEYPAD_MINUS", 	"KEYPAD_PLUS", 	"KEYPAD_ENTER", 	"P1", 	"P2", 	"P3", 	"P4", 	"P5", 	"P6", 	"P7", 	"P8", 	"P9", 	"P0", 	"KEYPAD_DOT_AND_DELETE", 	"NON_US_BACKSLASH_AND_PIPE", 	"APPLICATION", 	"POWER", 	"KEYPAD_EQUAL_SIGN", 	"F13", 	"F14", 	"F15", 	"F16", 	"F17", 	"F18", 	"F19", 	"F20", 	"F21", 	"F22", 	"F23", 	"F24", 	"EXECUTE", 	"HELP", 	"MANU", 	"SELECT", 	"STOP", 	"AGAIN", 	"UNDO", 	"CUT", 	"COPY", 	"PASTE", 	"FIND", 	"MUTE", 	"VOLUME_UP", 	"VOLUME_DOWN", 	"LOCKING_CAPS_LOCK", 	"LOCKING_NUM_LOCK", 	"LOCKING_SCROLL_LOCK", 	"KEYPAD_COMMA", 	"KEYPAD_EQUAL_SIGN_AS400", 	"INTERNATIONAL1", 	"INTERNATIONAL2", 	"INTERNATIONAL3", 	"INTERNATIONAL4", 	"INTERNATIONAL5", 	"INTERNATIONAL6", 	"INTERNATIONAL7", 	"INTERNATIONAL8", 	"INTERNATIONAL9", 	"LANG1", 	"LANG2", 	"LANG3", 	"LANG4", 	"LANG5", 	"LANG6", 	"LANG7", 	"LANG8", 	"LANG9", 	"ALTERNATE_ERASE", 	"SISREQ", 	"CANCEL", 	"CLEAR", 	"PRIOR", 	"RETURN", 	"SEPARATOR", 	"OUT", 	"OPER", 	"CLEAR_AND_AGAIN", 	"CRSEL_ANDPROPS", 	"EXSEL", 	"KEYPAD_00", 	"KEYPAD_000", 	"THOUSANDS_SEPARATOR", 	"DECIMAL_SEPARATOR", 	"CURRENCY_UNIT", 	"CURRENCY_SUB_UNIT", 	"KEYPAD_OPENING_PARENTHESIS", 	"KEYPAD_CLOSING_PARENTHESIS", 	"KEYPAD_OPENING_BRACE", 	"KEYPAD_CLOSING_BRACE", 	"KEYPAD_TAB", 	"KEYPAD_BACKSPACE", 	"KEYPAD_A", 	"KEYPAD_B", 	"KEYPAD_C", 	"KEYPAD_D", 	"KEYPAD_E", 	"KEYPAD_F", 	"KEYPAD_XOR", 	"KEYPAD_CARET", 	"KEYPAD_PERCENTAGE", 	"KEYPAD_LESS_THAN_SIGN", 	"KEYPAD_GREATER_THAN_SIGN", 	"KEYPAD_AMP", 	"KEYPAD_AMP_AMP", 	"KEYPAD_PIPE", 	"KEYPAD_PIPE_PIPE", 	"KEYPAD_COLON", 	"KEYPAD_HASHMARK", 	"KEYPAD_SPACE", 	"KEYPAD_AT", 	"KEYPAD_EXCLAMATION_SIGN", 	"KEYPAD_MEMORY_STORE", 	"KEYPAD_MEMORY_RECALL", 	"KEYPAD_MEMORY_CLEAR", 	"KEYPAD_MEMORY_ADD", 	"KEYPAD_MEMORY_SUBTRACT", 	"KEYPAD_MEMORY_MULTIPLY", 	"KEYPAD_MEMORY_DIVIDE", 	"KEYPAD_PLUS_AND_MINUS", 	"KEYPAD_CLEAR", 	"KEYPAD_CLEAR_ENTRY", 	"KEYPAD_BINARY", 	"KEYPAD_OCTAL", 	"KEYPAD_DECIMAL", 	"KEYPAD_HEXADECIMAL", 	"LEFT_CONTROL", 	"LEFT_SHIFT", 	"LEFT_ALT", 	"LEFT_GUI", 	"RIGHT_CONTROL", 	"RIGHT_SHIFT", 	"RIGHT_ALT", 	"RIGHT_GUI", 	"MOUSELEFT"]
Dim  $KeyCodes[$MAXKEYS] = [0x01, 	0x02, 	0x03, 	0x04, 	0x05, 	0x06, 	0x07, 	0x08, 	0x09, 	0x0A, 	0x0B, 	0x0C, 	0x0D, 	0x0E, 	0x0F, 	0x10, 	0x11, 	0x12, 	0x13, 	0x14, 	0x15, 	0x16, 	0x17, 	0x18, 	0x19, 	0x1A, 	0x1B, 	0x1C, 	0x1D, 	0x1E, 	0x1F, 	0x20, 	0x21, 	0x22, 	0x23, 	0x24, 	0x25, 	0x26, 	0x27, 	0x28, 	0x29, 	0x2A, 	0x2B, 	0x2C, 	0x2D, 	0x2E, 	0x2F, 	0x30, 	0x31, 	0x32, 	0x33, 	0x34, 	0x35, 	0x36, 	0x37, 	0x38, 	0x39, 	0x3A, 	0x3B, 	0x3C, 	0x3D, 	0x3E, 	0x3F, 	0x40, 	0x41, 	0x42, 	0x43, 	0x44, 	0x45, 	0x46, 	0x47, 	0x48, 	0x49, 	0x4A, 	0x4B, 	0x4C, 	0x4D, 	0x4E, 	0x4F, 	0x50, 	0x51, 	0x52, 	0x53, 	0x54, 	0x55, 	0x56, 	0x57, 	0x58, 	0x59, 	0x5A, 	0x5B, 	0x5C, 	0x5D, 	0x5E, 	0x5F, 	0x60, 	0x61, 	0x62, 	0x63, 	0x64, 	0x65, 	0x66, 	0x67, 	0x68, 	0x69, 	0x6A, 	0x6B, 	0x6C, 	0x6D, 	0x6E, 	0x6F, 	0x70, 	0x71, 	0x72, 	0x73, 	0x74, 	0x75, 	0x76, 	0x77, 	0x78, 	0x79, 	0x7A, 	0x7B, 	0x7C, 	0x7D, 	0x7E, 	0x7F, 	0x80, 	0x81, 	0x82, 	0x83, 	0x84, 	0x85, 	0x86, 	0x87, 	0x88, 	0x89, 	0x8A, 	0x8B, 	0x8C, 	0x8D, 	0x8E, 	0x8F, 	0x90, 	0x91, 	0x92, 	0x93, 	0x94, 	0x95, 	0x96, 	0x97, 	0x98, 	0x99, 	0x9A, 	0x9B, 	0x9C, 	0x9D, 	0x9E, 	0x9F, 	0xA0, 	0xA1, 	0xA2, 	0xA3, 	0xA4, 	0xB0, 	0xB1, 	0xB2, 	0xB3, 	0xB4, 	0xB5, 	0xB6, 	0xB7, 	0xB8, 	0xB9, 	0xBA, 	0xBB, 	0xBC, 	0xBD, 	0xBE, 	0xBF, 	0xC0, 	0xC1, 	0xC2, 	0xC3, 	0xC4, 	0xC5, 	0xC6, 	0xC7, 	0xC8, 	0xC9, 	0xCA, 	0xCB, 	0xCC, 	0xCD, 	0xCE, 	0xCF, 	0xD0, 	0xD1, 	0xD2, 	0xD3, 	0xD4, 	0xD5, 	0xD6, 	0xD7, 	0xD8, 	0xD9, 	0xDA, 	0xDB, 	0xDC, 	0xDD, 	0xE0, 	0xE1, 	0xE2, 	0xE3, 	0xE4, 	0xE5, 	0xE6, 	0xE7, 	0xFF]
Dim  $Command4HID[8] = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

Global $HIDDebug = 0
Global $GUID, $tGUID

;Opt("MustDeclareVars", 1)
Global Const $NULL = Chr(0)
Dim $dev, $strData, $strRead, $hand, $read, $res, $CapsKeyboard
Global $pKbdPreparcedData
Global $strDev

Global $hDevice

;Test()
;if GetHIDGUID() Then ConsoleWrite(_GetHIDDevInstByVidPid (0x03EB, 0x204F))

Func HIDConsoleWrite($dbgStr)
	if $HIDDebug == 1 Then ConsoleWrite($dbgStr)
EndFunc

Func Test()
	if GetHIDGUID() Then
		$HIDDebug =1
		While ((_GetHIDDevInstByVidPid (0x03EB, 0x204D) <> 1))
		   ConsoleWrite(".")
		WEnd
		 MsgBox(0,"Dongle","Found ")

		$HIDDebug = 1
		$hDevice = OpenHID($strDev)
		 MsgBox(0,"Dongle","Openned ")

		;_GetHIDDevInstByVidPid(0x4242, 0x6857)
		;_GetHIDDevInstByVidPid(0x16C0, 0x05DF)
		SendCMD_DELETE_ALL()
		 MsgBox(0,"Dongle","Nodes deleted ")
		;SendCMD_ADD_NODE("3", 60, 4000) ;
		;SendCMD_ADD_NODE_CONDITION("3", $t_playerHP, 40, 70)
		;SendCMD_ADD_NODE_CONDITION("3", $t_MobHP, 102, 102)
		;While (1)
			;if $HIDDebug == 1 Then HIDConsoleWrite(@LF &'Send F1: ' & @LF)
			;ConsoleWrite(@LF &'Start test: ' & @LF)
			;SendCMD_DELETE_ALL()
			;SendCMD_ADD_NODE("{F3}", 60, 4000) ;
			;SendCMD_ADD_NODE("{F4}", 60, 4000) ;
			;SendCMD_ADD_NODE("{F5}", 60, 4000) ;
			;SendCMD_ADD_NODE("{F6}", 60, 4000) ;
			;SendCMD_ADD_NODE("{F8}", 60, 4000) ;
			;SendCMD_ADD_NODE("{F9}", 60, 4000) ;
			;SendCMD_ADD_NODE("{F10}", 60, 4000) ;
			;SendCMD_ADD_NODE("{3}", 60, 4000) ;
			;SendCMD_ADD_NODE("{5}", 60, 4000) ;
			;SendCMD_ADD_NODE("{6}", 60, 4000) ;
			;SendCMD_ADD_NODE("{7}", 60, 4000) ;
			;SendCMD_DELETE_ALL()
			;SendCMD_SET_HPCPMP(50, 50, 50, 50)
			;SendCMD_ADD_NODE("MOUSELEFT", 60, 1000) ;
			SendCMD_ADD_NODE("3", 60, 1000, 0) ;
		 MsgBox(0,"Dongle","Key added ")
			;SendCMD_DELETE_ALL()
			;SendCMD_READ_CONFIG()
			;SendCMD_WRITE_CONFIG()
			;SendCMD_DELETE_NODE("{8}") ;
			;SendCMD_SET_MODIFIER( $HID_KEYBOARD_MODIFIER_NONE     )
;			SendKey2HID($HID_KEYBOARD_MODIFIER_NONE , "{1}")
;			Sleep(5000)
;			$HIDDebug = 1
;			StartStopHID(0)
;			Sleep(15000)
;			StartStopHID(1)
;			$HIDDebug = 0
;			Sleep(5000)


		;WEnd
	EndIf
EndFunc


Func SendCMD_WRITE_CONFIG() ;
   $Command4HID[0] = $INTERFACE_ID_Keyboard
   $Command4HID[1] = $CMD_WRITE_CONFIG
   Return SendCommand2HID() ; Send command for keyboard
EndFunc

Func SendCMD_READ_CONFIG() ;
   $Command4HID[0] = $INTERFACE_ID_Keyboard
   $Command4HID[1] = $CMD_READ_CONFIG
   Return SendCommand2HID() ; Send command for keyboard
EndFunc

Func SendCMD_ADD_NODE($Key, $PauseTime, $ReleaseTime, $ConditionTime) ;

   Local $X, $KeyCode
   For $X = 0 to $MAXKEYS-1
	  if $KeyMnemonic[$X] ==  $Key Then
		  $KeyCode = $KeyCodes[$X]
		  ExitLoop
	  EndIf
   Next
   Local $xPauseTime = Round($PauseTime/500, 0)  ; 0.5s grade
   if($xPauseTime > 0xFF) Then $xPauseTime = 0xFF

   Local $xReleaseTime = Round($ReleaseTime/100, 0) ; 0.1s grade
   ConsoleWrite('$xReleaseTime: ' & $xReleaseTime&  @LF)
   if($xReleaseTime > 0xFFFF) Then $xReleaseTime = 0xFFFF

   Local $xConditionTime = Round($ConditionTime/500, 0)  ; 0.5s grade
   if($xConditionTime > 0xFF) Then $xConditionTime = 0xFF

   $Command4HID[0] = $INTERFACE_ID_Keyboard
   $Command4HID[1] = $CMD_ADD_NODE
   $Command4HID[2] = $KeyCode
   $Command4HID[3] = $xPauseTime
   $Command4HID[4] = BitAND(0xFF, $xReleaseTime)
   $Command4HID[5] = BitAND(0xFF, $xReleaseTime/256)
   $Command4HID[6] = $xConditionTime

   ConsoleWrite('KEY: ' & $Command4HID[2]& ' Pause: ' & $Command4HID[3]*0.5 & 's Cooldown: ' & ($Command4HID[4] + $Command4HID[5]*256)*0.1& 's LOW: ' & $Command4HID[4] &' HIGH: ' & $Command4HID[5] & ' Condition time: ' & $Command4HID[6]*0.5& 's' & @LF)
   Return SendCommand2HID() ; Send command for keyboard
EndFunc

Func SendCMD_DELETE_NODE($Key) ;


   Local $X, $KeyCode
   For $X = 0 to $MAXKEYS-1
	  if $KeyMnemonic[$X] ==  $Key Then
		  $KeyCode = $KeyCodes[$X]
		  ExitLoop
	  EndIf
   Next
   $Command4HID[0] = $INTERFACE_ID_Keyboard
   $Command4HID[1] = $CMD_DELETE_NODE
   $Command4HID[2] = $KeyCode
   Return SendCommand2HID() ; Send command for keyboard
EndFunc
Func SendCMD_DELETE_ALL() ;
   $Command4HID[0] = $INTERFACE_ID_Keyboard
   $Command4HID[1] = $CMD_DELETE_ALL
   Return SendCommand2HID() ; Send command for keyboard
EndFunc
Func SendCMD_SET_MODIFIER($Modifier) ;
   $Command4HID[0] = $INTERFACE_ID_Keyboard
   $Command4HID[1] = $CMD_SET_MODIFIER
   $Command4HID[2] = $Modifier
   ConsoleWrite('SendCMD_SET_MODIFIER: ' & $Command4HID[2]  & @LF)

Return SendCommand2HID() ; Send command for keyboard
EndFunc

Func SendCMD_SET_HPCPMP($MobHP, $HP, $CP, $MP) ;
   $Command4HID[0] = $INTERFACE_ID_Keyboard
   $Command4HID[1] = $CMD_SET_HPCPMP
   $Command4HID[2] = $MobHP
   $Command4HID[3] = $HP
   $Command4HID[4] = $CP
   $Command4HID[5] = $MP
   Return SendCommand2HID() ; Send command for keyboard
EndFunc

Func SendCMD_ADD_NODE_CONDITION($Key, $Type, $Min, $Max) ;
   Local $X, $KeyCode
   For $X = 0 to $MAXKEYS-1
	  if $KeyMnemonic[$X] ==  $Key Then
		  $KeyCode = $KeyCodes[$X]
		  ExitLoop
	  EndIf
   Next
   $Command4HID[0] = $INTERFACE_ID_Keyboard
   $Command4HID[1] = $CMD_ADD_NODE_CONDITION
   $Command4HID[2] = $KeyCode
   $Command4HID[3] = $Type
   if($Min <> "") Then
	  $Command4HID[4] = $Min
   Else
	  $Command4HID[4] = 0xFF
   EndIf
   if($Max <> "") Then
	  $Command4HID[5] = $Max
   Else
	  $Command4HID[5] = 0xFF
   EndIf

   ConsoleWrite('COND: ' & $Command4HID[2]& ' ' & $Command4HID[3] & ' ' & $Command4HID[4] & '  '& $Command4HID[5] & @LF)

   Return SendCommand2HID() ; Send command for keyboard
EndFunc


Func SendCommand2HID() ; Send command for keyboard
   HIDConsoleWrite(@LF &'Send Command ' &  @LF)

   If $hDevice <> $INVALID_HANDLE_VALUE Then
	  HIDConsoleWrite(@LF &'$hDevice: ' & $hDevice & @LF)
	  ;HidD_GetCAPS($hDevice)
	  Local $ReportBufferW = DllStructCreate('byte reportID; byte report[8]')	 ;/* 8 данные + место для dummy репорт ID */
	  ;		Local $ReportBufferW = DllStructCreate('byte reportID; byte report')	 ;/* репорт ID и один байт*/
	  If @ERROR Then
		 HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  ;			MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	     Retorn 0
	  EndIf
	  Local $X
	  DllStructSetData($ReportBufferW, 'reportID', 0x02, 1); $INTERFACE_ID_Keyboard)
	  For $X = 1 to 8
		 DllStructSetData($ReportBufferW, 'report', $Command4HID[$X-1], $X)
	  Next
	  HIDConsoleWrite(@LF & 'Send keyboard command: ' & hex (DllStructGetData($ReportBufferW, 'report'), 2) & ' -> ')

	  Local $TryCount = 0
	  Local $ret=0
	  While($TryCount < 10 And  $ret<> 1)
		 $ret = HidD_SetReport( $hDevice, $ReportBufferW)
		 $TryCount+=1
	  WEnd
	  Return $ret
	  ;CloseHID($hDevice)
   EndIf
   Return 0
EndFunc




Func SendKey2HID($Modifier, $Key) ; Send command for keyboard
   Local $KeyCode

   HIDConsoleWrite(@LF &'Key: ' & $Key & @LF)
   Local $X
   For $X = 0 to $MAXKEYS-1
	  if $KeyMnemonic[$X] ==  $Key Then
		  $KeyCode = $KeyCodes[$X]
		  ExitLoop
	  EndIf
   Next
   ConsoleWrite(@LF &'KeyCode: ' & hex ($KeyCode) & @LF)

   If $hDevice <> $INVALID_HANDLE_VALUE Then
	  HIDConsoleWrite(@LF &'$hDevice: ' & $hDevice & @LF)
	  HidD_GetCAPS($hDevice)
	  Local $ReportBufferW = DllStructCreate('byte reportID; byte report[8]')	 ;/* 128 данные + место для dummy репорт ID */
	  ;		Local $ReportBufferW = DllStructCreate('byte reportID; byte report')	 ;/* репорт ID и один байт*/
	  If @ERROR Then
		 HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  ;			MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  EndIf
	  Local $X
	  DllStructSetData($ReportBufferW, 'reportID', 0x00, 1); $INTERFACE_ID_Keyboard)
	  For $X = 1 to 8
		 DllStructSetData($ReportBufferW, 'report', 0x00, $X)
	  Next
	  DllStructSetData($ReportBufferW, 'report', $INTERFACE_ID_Keyboard, 1)
	  DllStructSetData($ReportBufferW, 'report', $Modifier, 2)
	  DllStructSetData($ReportBufferW, 'report', $KeyCode, 3)
	  HIDConsoleWrite(@LF & 'Send keyboard command: ' & hex (DllStructGetData($ReportBufferW, 'report'), 2) & ' -> ')
	  HidD_SetReport( $hDevice, $ReportBufferW)
	  ;CloseHID($hDevice)
   EndIf
EndFunc


Func SendKey2Mouse($X, $Y, $Button) ; Send command for mouse

   If $hDevice <> $INVALID_HANDLE_VALUE Then
	  HIDConsoleWrite(@LF &'$hDevice: ' & $hDevice & @LF)
	  HidD_GetCAPS($hDevice)
	  Local $ReportBufferW = DllStructCreate('byte reportID; byte report[8]')	 ;/* 128 данные + место для dummy репорт ID */
	  ;		Local $ReportBufferW = DllStructCreate('byte reportID; byte report')	 ;/* репорт ID и один байт*/
	  If @ERROR Then
		 HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  ;			MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  EndIf
	  Local $X
	  DllStructSetData($ReportBufferW, 'reportID', 0x00)
	  For $X = 1 to 8
		 DllStructSetData($ReportBufferW, 'report', 0x00, $X)
	  Next
	  DllStructSetData($ReportBufferW, 'report',$INTERFACE_ID_Mouse, 1)
	  DllStructSetData($ReportBufferW, 'report', $X+128, 2)
	  DllStructSetData($ReportBufferW, 'report', $Y+128, 3)
	  DllStructSetData($ReportBufferW, 'report', $Button+128, 4)
	  HIDConsoleWrite(@LF & 'Send mouse command: ' & hex (DllStructGetData($ReportBufferW, 'report'), 2) & ' -> ')
	  HidD_SetReport( $hDevice, $ReportBufferW)
	  ;CloseHID($hDevice)
   EndIf
EndFunc

Func StartStopHID($State) ; Start/stop device

   If $hDevice <> $INVALID_HANDLE_VALUE Then
	  HIDConsoleWrite(@LF &'$hDevice: ' & $hDevice & @LF)
	  HidD_GetCAPS($hDevice)
	  Local $ReportBufferW = DllStructCreate('byte reportID; byte report[8]')	 ;/* 128 данные + место для dummy репорт ID */
	  ;		Local $ReportBufferW = DllStructCreate('byte reportID; byte report')	 ;/* репорт ID и один байт*/
	  If @ERROR Then
		 HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  ;			MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  EndIf
	  Local $X
	  DllStructSetData($ReportBufferW, 'reportID', 0x00)
	  For $X = 1 to 8
		 DllStructSetData($ReportBufferW, 'report', 0x00, $X)
	  Next
	  DllStructSetData($ReportBufferW, 'report', $INTERFACE_ID_GenericHID, 1)
	  DllStructSetData($ReportBufferW, 'report', $State, 2)
	  HIDConsoleWrite(@LF & 'Set device working status: ' & hex (DllStructGetData($ReportBufferW, 'report'), 2) & ' -> ')
	  HidD_SetReport( $hDevice, $ReportBufferW)
	  ;CloseHID($hDevice)
   EndIf
EndFunc


Func GetFromHID() ; Get device working status

   If $hDevice <> $INVALID_HANDLE_VALUE Then
	  HIDConsoleWrite(@LF &'$hDevice: ' & $hDevice & @LF)
	  HidD_GetCAPS($hDevice)
	  If @ERROR Then
		 HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  EndIf
	  Local $ReportBufferR = DllStructCreate('byte reportID; byte report[8]')	 ;/* 128 данные + место для dummy репорт ID */
	  If @ERROR Then
		 HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  EndIf
	  Local $X
	  DllStructSetData($ReportBufferR, 'reportID', 0x02)
	  For $X = 1 to 8
		 DllStructSetData($ReportBufferR, 'report', 0x00, $X)
	  Next
	  HidD_GetReport( $hDevice, $ReportBufferR)
	  HIDConsoleWrite(@LF & 'Get: ' & hex (DllStructGetData($ReportBufferR, 'report'), 2) & ' <- ')
	  Local $ret = DllStructGetData($ReportBufferR, 'report')
	  Return $ret
   EndIf
   Return 0
EndFunc


Func GetHIDGUID()
	$tGUID = DllStructCreate($tagGUID)
    $GUID = DllStructGetPtr($tGUID)
  	HIDConsoleWrite(@CRLF & "hid.dll HidD_GetHidGuid  получаем ID класса HID -> $GUID" & @CRLF)
	DllCall("hid.dll", "BOOLEAN", "HidD_GetHidGuid", "HWnd", $GUID)
	If @ERROR Then
		;MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
		HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
		Return -1
	Else
		;Check for WinAPI error
		Local $WinAPI_Error	 = _WinAPI_GetLastError()
		If $WinAPI_Error <> 0 Then
			;MsgBox(0,"HidD_GetHidGuid","Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
			HIDConsoleWrite("HidD_GetHidGuid Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
			Return -1
		EndIf
	EndIf

	HIDConsoleWrite('HID GUID: ' & hex(DllStructGetData($tGUID, 'Data1'), 8) & '-')
	HIDConsoleWrite(hex(DllStructGetData($tGUID, 'Data2'), 4) & '-')
	HIDConsoleWrite(hex(DllStructGetData($tGUID, 'Data3'), 4) & '-')
	HIDConsoleWrite(hex(DllStructGetData($tGUID, 'Data4'), 16) & @CRLF)
	Return 1
EndFunc

Func _GetHIDDevInstByVidPid($VID, $PID)

   ;Local $KeyboardActivated
   ; Get device interface info set handle
   ; for all devices attached to system
   HIDConsoleWrite(@CRLF & "setupapi.dll SetupDiGetClassDevsW получаем список устройств ($DIGCF_PRESENT, $DIGCF_DEVICEINTERFACE) $GUID -> $hDevInfo " & @CRLF)
   Local $hDevInfo = DllCall("setupapi.dll", "ptr", "SetupDiGetClassDevsW", "ptr", $GUID, "ptr", 0, "hwnd", 0, _
		 "dword", BitOR($DIGCF_PRESENT, $DIGCF_DEVICEINTERFACE) ) ;  BitOR($DIGCF_PRESENT, $DIGCF_DEVICEINTERFACE)
   Debug($hDevInfo)

   $hDevInfo = $hDevInfo[0]

   If $hDevInfo <> $INVALID_HANDLE_VALUE Then
	  ; Retrieve a context structure for a device interface
	  ; of a device information set.
	  Local $dwIndex = 0
	  Local $bRet
	  Local $buf = DllStructCreate($SP_DEV_BUF) ; "byte[2052]"
	  Local $pspdidd = DllStructCreate($SP_DEVICE_INTERFACE_DETAIL_DATA, DllStructGetPtr($buf)) ;"dword cbSize;wchar DevicePath[1024]"
	  Local $cb_spdidd = 6 ; size of fixed part of structure
	  If @AutoItX64 Then $cb_spdidd = 8 ; fix for x64
	  Local $spdid = DllStructCreate($SP_DEVICE_INTERFACE_DATA) ;"dword cbSize;byte InterfaceClassGuid[16];dword Flags;ulong_ptr Reserved" ; GUID struct = 16 bytes
	  Local $spdd = DllStructCreate($SP_DEVINFO_DATA) ; "dword cbSize;byte ClassGuid[16];dword DevInst;ulong_ptr Reserved"
	  DllStructSetData($spdid, "cbSize", DllStructGetSize($spdid))
	  While True
		 HIDConsoleWrite(@CRLF & "setupapi.dll SetupDiEnumDeviceInterfaces получение $SP_DEVICE_INTERFACE_DATA " & $dwIndex & "-го элемента списка в $spdid (context structure)" & @CRLF)
		 $bRet = DllCall("setupapi.dll", "int", "SetupDiEnumDeviceInterfaces", "ptr", $hDevInfo, "ptr", 0, _
				 "ptr", $GUID, "dword", $dwIndex, "ptr", DllStructGetPtr($spdid))
		 If Not $bRet[0] Then
			 HIDConsoleWrite("HID2KBD Absent" & @LF)
			 Return -1
		 EndIf
		 HIDConsoleWrite('$SP_DEVICE_INTERFACE_DATA: ' & DllStructGetData($spdid, 'cbSize') & '-')
		 HIDConsoleWrite(hex(DllStructGetData($spdid, 'InterfaceClassGuid'), 16) & '-')
		 HIDConsoleWrite(hex(DllStructGetData($spdid, 'Flags'), 4) & '-')
		 HIDConsoleWrite(hex(DllStructGetData($spdid, 'Reserved'), 8) & @CRLF)

		 HIDConsoleWrite(@CRLF & "SetupDiGetDeviceInterfaceDetailW получаем длину  DevicePath в $dwSize" & @CRLF)
		 Local $res = DllCall("setupapi.dll", "int", "SetupDiGetDeviceInterfaceDetailW", "ptr", $hDevInfo, "ptr", DllStructGetPtr($spdid), _
				 "ptr", 0, "dword", 0, "dword*", 0, "ptr", 0)
		 Local $dwSize = $res[5]
		 Debug($res)
		 HIDConsoleWrite("dwSize: " & $dwSize & @CRLF)


		 If $dwSize <> 0 And $dwSize <= DllStructGetSize($buf) Then
			 DllStructSetData($pspdidd, "cbSize", $cb_spdidd)
			 _ZeroMemory(DllStructGetPtr($spdd), DllStructGetSize($spdd))
			 DllStructSetData($spdd, "cbSize", DllStructGetSize($spdd))

	  ;        Local $spdd = DllStructCreate($SP_DEVINFO_DATA) ; "dword cbSize;byte ClassGuid[16];dword DevInst;ulong_ptr Reserved"
	  ;        Local $pspdidd = DllStructCreate($SP_DEVICE_INTERFACE_DETAIL_DATA, DllStructGetPtr($buf)) ;"dword cbSize;wchar DevicePath[1024]"

			 HIDConsoleWrite(@CRLF & "setupapi.dll SetupDiGetDeviceInterfaceDetailW получаем $SP_DEVICE_INTERFACE_DETAIL_DATA и  $SP_DEVINFO_DATA" & @CRLF)
			 $res = DllCall("setupapi.dll", "int", "SetupDiGetDeviceInterfaceDetailW", "ptr", $hDevInfo, "ptr", DllStructGetPtr($spdid), _
					 "ptr", DllStructGetPtr($pspdidd), "dword", $dwSize, "dword*", 0, "ptr", DllStructGetPtr($spdd))
			 Debug($res)
			 If $res[0] Then
		 HIDConsoleWrite('$SP_DEVICE_INTERFACE_DETAIL_DATA: ' & DllStructGetData($pspdidd, 'cbSize') & '-')
		 HIDConsoleWrite(DllStructGetData($pspdidd, 'DevicePath') & @CRLF)

		 HIDConsoleWrite('$SP_DEVINFO_DATA: ' & DllStructGetData($spdd, 'cbSize') & '-')
		 HIDConsoleWrite(hex(DllStructGetData($spdd, 'ClassGuid'), 32) & '-')
		 HIDConsoleWrite(hex(DllStructGetData($spdd, 'DevInst'), 4) & '-')
		 HIDConsoleWrite(hex(DllStructGetData($spdd, 'Reserved'), 8) & @CRLF)

				 HIDConsoleWrite(@CRLF & "CreateFileW создаем hDrive для DevicePath" & @CRLF)
				 Local $hDrive = DllCall("kernel32.dll", "ptr", "CreateFileW", "wstr", DllStructGetData($pspdidd, "DevicePath"), "dword", 0, _
						 "dword", BitOR($FILE_SHARE_READ, $FILE_SHARE_WRITE), "ptr", 0, "dword", $OPEN_EXISTING, _
						 "dword", 0, "ptr", 0)
				 $hDrive = $hDrive[0]
				 HIDConsoleWrite("hDrive: " & $hDrive & @CRLF)

				 If $hDrive <> $INVALID_HANDLE_VALUE Then
					 Local $kbdAttributes = DllStructCreate($tagHIDD_ATTRIBUTES)	 ;  Size VendorID ProductID VersionNumber
					 HIDConsoleWrite(@CRLF & "hid.dll HidD_GetAttributes $kbdAttributes" & @CRLF)
					 $res = DllCall("hid.dll", "BOOLEAN", "HidD_GetAttributes", "HWnd", $hDrive, "ptr", DllStructGetPtr($kbdAttributes))
					 Debug($res)
					 If @ERROR Then
						 ;MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
						 HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
					 Else
						 ;Check for WinAPI error
						 Local $WinAPI_Error = _WinAPI_GetLastError()
						 If $WinAPI_Error <> 0 Then
							 ;MsgBox(0,"HidD_GetAttributes","Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
							 HIDConsoleWrite("HidD_GetAttributes Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
						EndIf
						;Else
						if 1 Then
							 If $res[0] Then
								 HIDConsoleWrite(DllStructGetData($kbdAttributes, 'Size') & ' -> ')
								 HIDConsoleWrite(hex(DllStructGetData($kbdAttributes, 'VendorID'), 4) & ' ?= ')
								 HIDConsoleWrite(hex($VID) & ' -> ')
								 HIDConsoleWrite(hex(DllStructGetData($kbdAttributes, 'ProductID'), 4) & ' ?= ')
								 HIDConsoleWrite(hex($PID) & ' -> ')
								 HIDConsoleWrite(DllStructGetData($kbdAttributes, 'VersionNumber') & @LF)
								 HidD_GetCAPS($hDrive)
								 If ($VID == DllStructGetData($kbdAttributes, "VendorID") And $PID == DllStructGetData($kbdAttributes, "ProductID")) Then HIDConsoleWrite("VID PID FOUND! DEVICE PATH: " & DllStructGetData($pspdidd, "DevicePath") & @LF)
								 HIDConsoleWrite(DllStructGetData($pspdidd, "DevicePath") & @LF & @LF & @LF)
								 ;if $HIDDebug == 1 Then MsgBox(0,"HidD_GetPreparsedData",DllStructGetData($pspdidd, "DevicePath")) ;Error 13: The data is invalid.

								 If ($VID == DllStructGetData($kbdAttributes, "VendorID") And $PID == DllStructGetData($kbdAttributes, "ProductID")  And StringInStr(DllStructGetData($pspdidd, "DevicePath"),"MI_02")) Then
									 HIDConsoleWrite(@CRLF & "CloseHandle $hDrive" & @CRLF)
									 $res = DllCall("kernel32.dll", "int", "CloseHandle", "ptr", $hDrive)
									 If Not $res[0] Then HIDConsoleWrite("Error closing volume: " & $hDrive & @CRLF)
									 HIDConsoleWrite(@CRLF & "SetupDiDestroyDeviceInfoList $hDevInfo" & @CRLF)
									 $res = DllCall("setupapi.dll", "int", "SetupDiDestroyDeviceInfoList", "ptr", $hDevInfo)
									 If Not $res[0] Then HIDConsoleWrite("SetupDiDestroyDeviceInfoList error." & @CRLF)
									 HIDConsoleWrite("USB2KBD DevicePath: " & DllStructGetData($pspdidd, "DevicePath") & @LF)
									 $StrDev = DllStructGetData($pspdidd, "DevicePath")

									 ;$KeyboardActivated = 1 ; Устройство USB2KBD найдено
									 Return 1
								 EndIf

							 EndIf
						 EndIf
					 EndIf

					 HIDConsoleWrite(@CRLF & "CloseHandle $hDrive" & @CRLF)
					 $res = DllCall("kernel32.dll", "int", "CloseHandle", "ptr", $hDrive)
				   If Not $res[0] Then HIDConsoleWrite("Error closing volume: " & $hDrive & @CRLF)
				 EndIf
			 EndIf
		 EndIf
		 $dwIndex += 1
	  WEnd
	  HIDConsoleWrite(@CRLF & "SetupDiDestroyDeviceInfoList $hDevInfo" & @CRLF)
	  $res = DllCall("setupapi.dll", "int", "SetupDiDestroyDeviceInfoList", "ptr", $hDevInfo)
	  If Not $res[0] Then HIDConsoleWrite("Destroy error." & @CRLF)
   EndIf

   Return -1
EndFunc   ;==>_GetDrivesDevInstByDeviceNumber


Func _ZeroMemory($ptr, $size)
    DllCall("kernel32.dll", "none", "RtlZeroMemory", "ptr", $ptr, "ulong_ptr", $size)
EndFunc   ;==>_ZeroMemory


;Dump array to console
Func Debug($aArray)
    For $X = 0 to Ubound($aArray)-1
        HIDConsoleWrite("["&$X&"]: " & $aArray[$X] & @CRLF)
    Next
    HIDConsoleWrite(@CRLF)
EndFunc


Func OpenHID($strDevPath)
  	HIDConsoleWrite(@CRLF & "kernel32.dll CreateFile $strDevPath -> $hDevice" & @CRLF)
	Local $res = DllCall("kernel32.dll", "ptr", "CreateFile", "str", $strDevPath,             "dword", 0, "dword", BitOR($FILE_SHARE_READ, $FILE_SHARE_WRITE), "ptr",$NULL, "dword", $OPEN_EXISTING, "dword", 0, "ptr", $NULL)
	Return $res[0]
EndFunc

Func HidD_GetCAPS($hDevice)
		Local $pKbdPreparcedData = DllStructCreate('Ptr PreparcedData;')
	  	HIDConsoleWrite(@CRLF & "hid.dll HidD_GetPreparsedData $hDevice -> $pKbdPreparcedData" & @CRLF)
		Local $res = DllCall("hid.dll", "int", "HidD_GetPreparsedData", "ptr", $hDevice, "ptr", DllStructGetPtr($pKbdPreparcedData, 'PreparcedData'))
		Debug($res)
		If @ERROR Then
			;MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
			HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
		Else
			;Check for WinAPI error
			Local $WinAPI_Error = _WinAPI_GetLastError()
			If $WinAPI_Error <> 0 Then
				;MsgBox(0,"HidD_GetPreparsedData","Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
				HIDConsoleWrite("HidD_GetPreparsedData Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
			Else

				HIDConsoleWrite(DllStructGetData('PreparcedData:' & $pKbdPreparcedData, 'PreparcedData') & @LF)
			EndIf
		EndIf

		Local $CapsKeyboard = DllStructCreate($tagHIDP_CAPS)
	  	HIDConsoleWrite(@CRLF & "hid.dll HidP_GetCaps $hDevice -> $CapsKeyboard" & @CRLF)
		$res = DllCall("hid.dll", "int", "HidP_GetCaps", "ptr", DllStructGetData($pKbdPreparcedData, 'PreparcedData'), "ptr", DllStructGetPtr($CapsKeyboard))
		Debug($res)
		If @ERROR Then
			;MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
			HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
		Else
			;Check for WinAPI error
			Local $WinAPI_Error = _WinAPI_GetLastError()
			If $WinAPI_Error <> 0 Then
				;MsgBox(0,"HidP_GetCaps","Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
				HIDConsoleWrite("HidP_GetCaps Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
			Else

				HIDConsoleWrite('Usage = ' & hex(DllStructGetData($CapsKeyboard, 'Usage'), 8)  &  @LF)
				HIDConsoleWrite('UsagePage = ' & hex(DllStructGetData($CapsKeyboard, 'UsagePage'), 8)  &  @LF)
				HIDConsoleWrite('InputReportLength = ' & hex(DllStructGetData($CapsKeyboard, 'InputReportByteLength'), 4)  &  @LF)
				HIDConsoleWrite('OutputReportLength = ' & hex(DllStructGetData($CapsKeyboard, 'OutputReportByteLength'), 4)  &  @LF)
				HIDConsoleWrite('FeatureReportLength = ' & hex(DllStructGetData($CapsKeyboard, 'FeatureReportByteLength'), 4)  &  @LF)
				HIDConsoleWrite('Reserved = ' & hex(DllStructGetData($CapsKeyboard, 'Reserved'), 4)  &  @LF)
				HIDConsoleWrite('NumberLinkCollectionNodes = ' & hex(DllStructGetData($CapsKeyboard, 'NumberLinkCollectionNodes'), 4)  &  @LF)
				HIDConsoleWrite('NumberInputButtonCaps = ' & hex(DllStructGetData($CapsKeyboard, 'NumberInputButtonCaps'), 4)  &  @LF)
				HIDConsoleWrite('NumberInputValueCaps = ' & hex(DllStructGetData($CapsKeyboard, 'NumberInputValueCaps'), 4)  &  @LF)
				HIDConsoleWrite('NumberInputDataIndices = ' & hex(DllStructGetData($CapsKeyboard, 'NumberInputDataIndices'), 4)  &  @LF)
				HIDConsoleWrite('NumberOutputButtonCaps = ' & hex(DllStructGetData($CapsKeyboard, 'NumberOutputButtonCaps'), 4)  &  @LF)
				HIDConsoleWrite('NumberOutputValueCaps = ' & hex(DllStructGetData($CapsKeyboard, 'NumberOutputValueCaps'), 4)  &  @LF)
				HIDConsoleWrite('NumberOutputDataIndices = ' & hex(DllStructGetData($CapsKeyboard, 'NumberOutputDataIndices'), 4)  &  @LF)
				HIDConsoleWrite('NumberFeatureButtonCaps = ' & hex(DllStructGetData($CapsKeyboard, 'NumberFeatureButtonCaps'), 4)  &  @LF)
				HIDConsoleWrite('NumberFeatureValueCaps = ' & hex(DllStructGetData($CapsKeyboard, 'NumberFeatureValueCaps'), 4)  &  @LF)
				HIDConsoleWrite('NumberFeatureDataIndices = ' & hex(DllStructGetData($CapsKeyboard, 'NumberFeatureDataIndices'), 4)  & @LF)
			EndIf
		EndIf

EndFunc

Func CloseHID($hDevice)
	  	HIDConsoleWrite(@CRLF & "kernel32.dll CloseHandle $hDevice" & @CRLF)
		Local $res = DllCall("kernel32.dll", "int", "CloseHandle", "ptr", $hDevice)
		If @ERROR Then
			;MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
			HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
		Else
			;Check for WinAPI error
			Local $WinAPI_Error = _WinAPI_GetLastError()
			If $WinAPI_Error <> 0 Then
				;MsgBox(0,"HidP_GetCaps","Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
				HIDConsoleWrite("HidP_GetCaps Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
			Else
				HIDConsoleWrite(@LF  & "CloseHandle: " & $res  & @LF )
				Debug(DllStructGetPtr($res))
			EndIf
		EndIf
	EndFunc

Func HidD_GetFeature( $hDevice, $tReportBufferR)
		;Local $tReportBufferR = DllStructCreate('byte reportID; byte report[8]')	 ;/* 128 данные + место для dummy репорт ID */
		;If @ERROR Then
		;	MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
		;EndIf

		Local $pReportBufferR = DllStructGetPtr($tReportBufferR)
		HIDConsoleWrite('Address: ' & hex($pReportBufferR,8) & @LF)
		Local $iReportBufferR = DllStructCreate('ulong iSize');
		DllStructSetData($iReportBufferR, 'iSize', DllStructGetSize($tReportBufferR))
	    Local $piReportBufferR = DllStructGetPtr($iReportBufferR);
		Local $ppReportBufferR = DllStructCreate('Ptr preport;')
		DllStructSetData($ppReportBufferR, 'preport', DllStructGetSize($pReportBufferR))


	  	HIDConsoleWrite(@CRLF & "hid.dll HidD_GetFeature $hDevice -> $tReportBufferR" & @CRLF)
		Local $res = DllCall("hid.dll", "BOOLEAN", "HidD_GetFeature", "HWnd", $hDevice, "PTR", DllStructGetPtr($tReportBufferR), "ulong", DllStructGetSize($tReportBufferR))
		Debug($res)
		If @ERROR Then
			;MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
			HIDConsoleWrite("@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
		Else
			;Check for WinAPI error
			Local $WinAPI_Error = _WinAPI_GetLastError()
			If $WinAPI_Error <> 0 Then
				;MsgBox(0,"HidD_GetFeature","Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
				HIDConsoleWrite("HidD_GetFeature Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
				Else
				if Not $res[0] Then
					;MsgBox(16, "HidD_GetFeature", "Fail")
					HIDConsoleWrite("HidD_GetFeature Fail")
				EndIf
				HIDConsoleWrite(@LF & 'Get: ' & hex (DllStructGetData($tReportBufferR, 'report'), $iReportBufferR) & ' -> ')

			EndIf
		EndIf
	EndFunc

Func HidD_SetFeature( $hDevice, $tReportBufferW)
		Local $pReportBufferW = DllStructGetPtr($tReportBufferW)
		Local $iReportBufferW = DllStructGetSize($tReportBufferW)

		HIDConsoleWrite(@LF & 'Set: ' & hex (DllStructGetData($tReportBufferW, 'reportID'), $iReportBufferW) & hex (DllStructGetData($tReportBufferW, 'report'), $iReportBufferW) & ' -> ')
	  	HIDConsoleWrite(@CRLF & "hid.dll HidD_SetFeature $tReportBufferW -> $hDevice" & @CRLF)
;		BOOLEAN __stdcall HidD_SetOutputReport(
;  _In_  HANDLE HidDeviceObject,
;  _In_  PVOID ReportBuffer,
;  _In_  ULONG ReportBufferLength
;);
;		Local $res = DllCall("hid.dll", "BOOLEAN", "HidD_SetOutputReport", "HWnd", $hDevice, "PTR", DllStructGetPtr($tReportBufferW), "ulong", DllStructGetSize($tReportBufferW))
		Local $res = DllCall("hid.dll", "BOOLEAN", "HidD_SetFeature", "HWnd", $hDevice, "PTR", DllStructGetPtr($tReportBufferW), "ulong", DllStructGetSize($tReportBufferW))
;	 HIDConsoleWrite($res)
	 Debug($res)
		If @ERROR Then
			HIDConsoleWrite(	"@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
			;MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
		Else
			;Check for WinAPI error
			Local $WinAPI_Error = _WinAPI_GetLastError()
			If $WinAPI_Error <> 0 Then
				HIDConsoleWrite("HidD_SetFeature Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
				;MsgBox(0,"HidD_SetFeature","Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
		Else
				if Not $res[0]  then
					;MsgBox(16, "HidD_SetFeature", "Fail")
					HIDConsoleWrite( @LF & "HidD_SetFeature Fail"& @LF)
				EndIf
				HIDConsoleWrite(hex(DllStructGetData($tReportBufferW, 'preport'), 8) & ' -> ')
			EndIf
		EndIf

EndFunc


Func HidD_SetReport( $hDevice, $tReportBufferW)
   Local $pReportBufferW = DllStructGetPtr($tReportBufferW)
   Local $iReportBufferW = DllStructGetSize($tReportBufferW)

   HIDConsoleWrite(@LF & 'Set: ' & hex (DllStructGetData($tReportBufferW, 'reportID'), $iReportBufferW) & hex (DllStructGetData($tReportBufferW, 'report'), $iReportBufferW) & ' -> ')
   HIDConsoleWrite(@CRLF & "hid.dll HidD_SetOutputReport $tReportBufferW -> $hDevice" & @CRLF)
   ;		BOOLEAN __stdcall HidD_SetOutputReport(
;  _In_  HANDLE HidDeviceObject,
;  _In_  PVOID ReportBuffer,
;  _In_  ULONG ReportBufferLength
;);
	  HIDConsoleWrite('report' & hex(DllStructGetData($tReportBufferW, 'report '), 8) & ' -> ' & @CRLF)
	  Local $res = DllCall("hid.dll", "BOOLEAN", "HidD_SetOutputReport", "HWnd", $hDevice, "PTR", DllStructGetPtr($tReportBufferW), "ulong", DllStructGetSize($tReportBufferW))
	  HIDConsoleWrite($res)
	  Debug($res)
	  If @ERROR Then
		 HIDConsoleWrite(	"@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
		 ;MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  Else
		 ;Check for WinAPI error
		 Local $WinAPI_Error = _WinAPI_GetLastError()
		 If $WinAPI_Error <> 0 Then
			 HIDConsoleWrite("HidD_SetOutputReport Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
			 ;MsgBox(0,"HidD_SetOutputReport","Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
		 Else
			 if Not $res[0]  then
				 ;MsgBox(16, "HidD_SetOutputReport", "Fail")
				 HIDConsoleWrite( @LF & "HidD_SetOutputReport Fail"& @LF)
			 EndIf
			 HIDConsoleWrite('reportID ' & hex(DllStructGetData($tReportBufferW, 'reportID'), 8) & ' -> ')
			 HIDConsoleWrite('report' & hex(DllStructGetData($tReportBufferW, 'report'), 8) & ' -> ')
		 EndIf
	  EndIf
	  Return $res[0]
EndFunc


Func HidD_GetReport( $hDevice, $tReportBufferR)
   Local $pReportBufferR = DllStructGetPtr($tReportBufferR)
   Local $iReportBufferR = DllStructGetSize($tReportBufferR)

   HIDConsoleWrite(@LF & '.Get: ' & hex (DllStructGetData($tReportBufferR, 'reportID'), $iReportBufferR) & ' ' & hex (DllStructGetData($tReportBufferR, 'report'), $iReportBufferR) & ' <- ' & @CRLF)
   HIDConsoleWrite(@CRLF & "hid.dll HidD_GetInputReport $tReportBufferR -> $hDevice")
;		BOOLEAN __stdcall HidD_GetInputReport(
;  _In_  HANDLE HidDeviceObject,
;  _In_  PVOID ReportBuffer,
;  _In_  ULONG ReportBufferLength
;);
   HIDConsoleWrite('report' & hex(DllStructGetData($tReportBufferR, 'report '), 8) & ' -> ' & @CRLF)
   Local $res = DllCall("hid.dll", "BOOLEAN", "HidD_GetInputReport", "HWnd", $hDevice, "PTR", DllStructGetPtr($tReportBufferR), "ulong", DllStructGetSize($tReportBufferR))
   HIDConsoleWrite($res)
   Debug($res)
   If @ERROR Then
	  HIDConsoleWrite(	"@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
	  ;MsgBox(0,"","@ERROR: " & @ERROR & @CRLF & "@EXTENDED: " & @EXTENDED)
   Else
	  ;Check for WinAPI error
	  Local $WinAPI_Error = _WinAPI_GetLastError()
	  If $WinAPI_Error <> 0 Then
		 HIDConsoleWrite("HidD_GetInputReport Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
		  ;MsgBox(0,"HidD_SetOutputReport","Error " & _WinAPI_GetLastError() & @CRLF & _WinAPI_GetLastErrorMessage()) ;Error 13: The data is invalid.
	  Else
		  if Not $res[0]  then
			;MsgBox(16, "HidD_SetOutputReport", "Fail")
			HIDConsoleWrite( @LF & "HidD_GetInputReport Fail"& @LF)
		  EndIf
		  HIDConsoleWrite('reportID ' & hex(DllStructGetData($tReportBufferR, 'reportID'), 8) & ' -> ')
		  HIDConsoleWrite('report' & hex(DllStructGetData($tReportBufferR, 'report'), 8) & ' -> ')
	  EndIf
   EndIf

EndFunc