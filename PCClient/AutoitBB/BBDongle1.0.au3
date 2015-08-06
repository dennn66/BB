#cs ----------------------------------------------------------------------------

Boredom Breaker Version: 2.12
12/01/2011

Script Function: Boredom Breaker for Advanced L2 Users


#ce ----------------------------------------------------------------------------

#include <GUIConstantsEx.au3>
#include <TreeViewConstants.au3>
#include <WinAPI.au3>
#include <Array.au3>
#include <StructureConstants.au3>
#include "HIDLib.au3"
#include <Date.au3>



Opt('MustDeclareVars', 1)
Opt("GUIOnEventMode", 1)
Opt("PixelCoordMode", 2)



;Opt("MustDeclareVars", 1)


;HotKeySet("{F11}","OnExit")

Global Const $HP_COLOR = 0x70150B, $CP_COLOR = 0x805300, $MP_COLOR = 0x03327E, $MobHP_COLOR = 0x881D18,  $PET_MP_COLOR1 = 0x074C9C, $PET_MP_COLOR2 = 0x16263B
Global Const $MAXPLAYERS = 10, $MAXCONDITIONSPERPAGE = 12, $MAXTABS = 5, $MAXGROUPS=$MAXCONDITIONSPERPAGE*$MAXTABS, $MAXCONDITIONS = $MAXGROUPS, $PLAYERTIMEOUT = 300000
Global Const $WCLASS = "l2UnrealWWindowsViewportWindow"
;Global Const $WCLASS = "Photo_Lightweight_Viewer" ; debug mode

;DEFAULT CONFIG BEGIN
;MAIN
Global  $DongleConnected = 0
Global  $WalkCoolDownSet=40, $WalkSet = 0, $NicName
Dim $F[$MAXCONDITIONS+1], $FSet[$MAXCONDITIONS+1]
Dim $FCoolDown[$MAXCONDITIONS+1], $FCoolDownSet[$MAXCONDITIONS+1]
;TOGGLE GROUP
;Dim $ToggleGroupName[$MAXCONDITIONS+1]
;Dim  $ToggleGroupState[$MAXCONDITIONS+1]
;CONDITION GROUP
;Dim $ToggleGroupID[$MAXCONDITIONS+1], $ToggleGroup[$MAXCONDITIONS+1]
;Dim $ToggleStateID[$MAXCONDITIONS+1], $ToggleStateConditionID[$MAXCONDITIONS+1], $ToggleStateCondition[$MAXCONDITIONS+1]
Dim $FPause[$MAXCONDITIONS+1], $FPauseSet[$MAXCONDITIONS+1]
Dim $FConditionIdle[$MAXCONDITIONS+1], $FConditionIdleSet[$MAXCONDITIONS+1]
Dim $FHPMin[$MAXCONDITIONS+1], $FHPMinSet[$MAXCONDITIONS+1]
Dim $FHPMax[$MAXCONDITIONS+1], $FHPMaxSet[$MAXCONDITIONS+1]
Dim $FCPMin[$MAXCONDITIONS+1], $FCPMinSet[$MAXCONDITIONS+1]
Dim $FCPMax[$MAXCONDITIONS+1], $FCPMaxSet[$MAXCONDITIONS+1]
Dim $FMPMin[$MAXCONDITIONS+1], $FMPMinSet[$MAXCONDITIONS+1]
Dim $FMPMax[$MAXCONDITIONS+1], $FMPMaxSet[$MAXCONDITIONS+1]
Dim $FMobHPMin[$MAXCONDITIONS+1], $FMobHPMinSet[$MAXCONDITIONS+1]
Dim $FMobHPMax[$MAXCONDITIONS+1], $FMobHPMaxSet[$MAXCONDITIONS+1]

Dim $ButtonsID[$MAXCONDITIONS+1], $Buttons [$MAXCONDITIONS+1] = ["", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12","1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "P1", 	"P2", 	"P3", 	"P4", 	"P5", 	"P6", 	"P7", 	"P8", 	"P9", 	"P0", "KEYPAD_DOT_AND_DELETE", "KEYPAD_SLASH", "KEYPAD_ASTERISK", 	"KEYPAD_MINUS", 	"KEYPAD_PLUS", 	"KEYPAD_ENTER", "RIGHT_ARROW", 	"LEFT_ARROW", 	"DOWN_ARROW", 	"UP_ARROW", "ESCAPE", 	"BACKSPACE", 	"TAB", 	"SPACE"]
Dim $TargetNicID[$MAXCONDITIONS+1], $TargetNic[$MAXCONDITIONS+1]
Dim $CheckNicID[$MAXCONDITIONS+1], $CheckNic[$MAXCONDITIONS+1]
;DEFAULT CONFIG END
Global  $Suspend = 0
Global  $ShiftState, $CtrlState,  $ShiftStateID, $CtrlStateID, $treeShift

Global $DongleCheckTimer
Global  $SaveToDongleID, $YesID, $NoID, $SuspendID, $ExitID, $QuitID, $QuitBoxID, $ActivityBox, $HPBar, $MobHPBar, $CPBar, $IgnorePet=0, $hWndMain, $MPBar, $PetMPBar, $treeTwo, $SaveID, $OpenID, $PlayersListID, $FindL2ID, $FindMobID, $ToggleWideViewID, $ServerID, $PlayersID, $BarCheckTimeID, $MobBarCheckTimeID, $TabID, $NicNameID, $listView, $L2Hwnd, $Title="Lineage"
Global $HP=100, $CP=100,$MP=100,$yHP=0, $yMobHP=0,$yCP=0,$yMP=0,$BarStart=0, $MobBarMiddle=0, $MobBarStart=0,$BarLen=1,$MobBarLen=1, $HPDetected=0, $CPDetected=0, $MPDetected=0, $MobHP=0, $MobHPDetected=0, $MobHPID=0
Global  $working=0, $IsHPConditions=0,  $IsMobHPConditions=0, $IsCPConditions=0,$IsMPConditions=0, $BarCheckTime = 500, $MobBarCheckTime = 500, $ServerIP="127.0.0.1"
Global $Walk, $WalkCoolDown, $UDPSendTimer, $PauseTimer=0, $PauseTime=0, $status, $LastConfig = "BoredomBreaker.cfg"
Dim $treeOne[$MAXCONDITIONS], $treeState[$MAXTABS], $treeStateCondition[$MAXTABS], $LabelID[20]
Dim $Players [$MAXPLAYERS+2][10]
; Timestump, IP, Port, hWnd, NicName, HP, MP,CP
Dim $Windows [10][2]
;Dim $Buttons [11] = ["", "{1}", "{2}", "{3}", "{4}", "{5}", "{6}", "{7}", "{8}", "{9}", "{0}"] ; debug mode
Dim $FTimer [$MAXCONDITIONS+1] , $RcvSocket[4], $SndSocket[4], $SrvSocket[4], $HttpServerIP="127.0.0.1"
Global $StrDev, $WalkTimer,  $BarParseTimer, $HPCheckTimer, $MobBarParseTimer, $MobHPCheckTimer, $MobBarCheckTimeID, $SuspendTimer, $guiXPos=30, $guiYPos=810+90+40+40, $HotKey=0, $isMainWindow, $data ;

_Main()

Func _Main()
	Local    $i, $n, $j, $var
	Dim $message[20], $ClientSocket[4]



	if not HotKeySet("{PAUSE}","ToggleWork") Then
		if not HotKeySet("{INSERT}","ToggleWork") Then
			if not HotKeySet("{DELETE}","ToggleWork") Then
				if not HotKeySet("{PRINTSCREEN}","ToggleWork") Then
					MsgBox(4096,"","Hot Keys Busy")
				EndIf
				$HotKey=	"{PRINTSCREEN}"
			EndIf
			$HotKey="{DELETE}"
		EndIf
		$HotKey="{INSERT}"
	Else
		$HotKey="{PAUSE}"
	EndIf
	;;This is the UDP Client
	;;Start the server first
	Init()

	$var = FileOpenDialog("Open Boredom Breaker Config File", @MyDocumentsDir & "", "Configs (*.cfg)|All Files (*.*)", 1 + 2, $LastConfig )
	If @error Then
		;MsgBox(4096,"","No File(s) chosen")
		LoadIni(0)
	Else
		;LoadConfig($var, 1)
		$LastConfig = $var
		LoadIni($var)
	EndIf
	MainWindowCreate()
	OnFindL2()

	UpdateTab($GUI_HIDE)
	OnToggleWideView()
	;GUICtrlSetData($TabID, 2)
	GUISetState(@SW_SHOW, $hWndMain)  ; display the GUI


	OnUpdateSet()



	; Start The UDP Services
	;==============================================
	UDPStartup()
	OnAutoItExitRegister("Cleanup")



	; Bind to a SOCKET
	;==============================================
	$RcvSocket = UDPBind(@IPAddress1, 65533)
	If @error <> 0 Then
		$isMainWindow = 0
		;MsgBox(4096,"","UDP Port 65532 Busy")
		$RcvSocket = UDPBind(@IPAddress1, 65534)
		If @error <> 0 Then
			;MsgBox(4096,"","UDP Port 65533 Busy")
			$RcvSocket = UDPBind(@IPAddress1, 65535)
		;MsgBox(4096,"","UDP Port Busy")
			If @error <> 0 Then
				;MsgBox(4096,"","UDP Port 65534 Busy")
				$RcvSocket = UDPBind(@IPAddress1, 65536)
				If @error <> 0 Then MsgBox(4096,"","UDP Ports Busy")
			EndIf
		EndIf
		;MsgBox(4096,"","UDP Port Busy")
	Else
		$isMainWindow = 1

	Endif


	$SndSocket = UDPOpen($ServerIP, 65533)
	If @error <> 0 Then MsgBox(4096,"","UDP Port Open Failed")
	$SrvSocket = UDPOpen($HttpServerIP, 65532)
	If @error <> 0 Then MsgBox(4096,"","UDP Port Open Failed")
;	if $isMainWindow Then UpnpOpen()

		;if $WalkSet <> 0 Then  MsgBox(4096,"","Walk#7")


	While (1) ;GuiGetMsg() <> $GUI_EVENT_CLOSE)
#cs
		$data = UDPRecv($RcvSocket, 150)
		If $data <> "" Then
			$message = StringSplit($data, " ")
			Switch $message[1]
				Case "INIT"
					; $message = Command, IP, Port, hWnd, NicName, HP, MP,CP, Timestump
					if HWnd($message[4]) = $L2hwnd and $message[3] = $RcvSocket[3] and $message[2] = $RcvSocket[2] Then
							$n=0
					Else
						if $message[5] = $NicName Or  $message[5] = "" Then   $message[5] = "<" & $message[5] &">"

						$n = FindPlayerByNic($message[5])
						if $n = -1 Then $n = FindFreePlayerSlot()


					Endif
					if($n <> -1) Then


						; $Players = Timestump, IP, Port, hWnd, NicName, HP, MP,CP
						$Players[$n][0] = TimerInit()
						for $i=1 To 4
							$Players[$n][$i] = $message[$i+1]
						Next
						for $i=5 To 8
							if $message[$i+1] >= 0 And $message[$i+1] <= 100 Then
								$Players[$n][$i] = $message[$i+1]
							Else
								$Players[$n][$i] = 100
							Endif
						Next


					Endif
				Case "STOP"
					if $working Then ToggleWork()
				Case "WORK"
					if  $working = 0 Then ToggleWork()
				Case "COMMAND"
					if $PauseTime <>0 Then
						if TimerDiff($PauseTimer) < $PauseTime*1000 Then
							Sleep($PauseTime - TimerDiff($PauseTimer))
							$PauseTime = 0
						EndIf
					EndIf

					if  WinExists($L2Hwnd) Then
						if $working Then
							if WinActive($L2Hwnd) = 0 Then WinActivate($L2Hwnd)
	;						if WinWaitActive($L2Hwnd, "", 15) <> 0 Then
								Send($message[2])
	;						Endif
						Endif
					Else
						$L2Hwnd = 0
					Endif
				Case Else
					MsgBox(4096,"COMMAND",$message[1])
			EndSwitch

		EndIf
#ce
; Find HID Device


 		 if($DongleConnected == 0) Then

			if GetHIDGUID() Then
			  $HIDDebug = 0
			  If (_GetHIDDevInstByVidPid (0x03EB, 0x204D) == 1) Then
				  $hDevice = OpenHID($strDev)
				  If $hDevice <> $INVALID_HANDLE_VALUE Then
					 OnSaveToDongle()
				  EndIf
			  EndIf
			EndIf
		 EndIf

		 if TimerDiff($HPCheckTimer) > 200+$working*($HPDetected+$CPDetected*$IsCPConditions+$MPDetected*$IsMPConditions)*$BarCheckTime*2 Then
			UpdatePlayerList()
			$HPCheckTimer = TimerInit()
			Local $xMobHP, $xHP, $xCP, $xMP
			$xMobHP = 0xFF
			$xHP = 0xFF
			$xCP = 0xFF
			$xMP = 0xFF
			if $HPDetected Then
			   $HP=GetHPCPMP($yHP, $HP_COLOR)
			   $xHP = $HP
			EndIf
			if ($IsCPConditions or not $working) And $CPDetected Then
			   $CP=GetHPCPMP($yCP, $CP_COLOR)
			   $xCP = $CP
			EndIf
			if ($IsMPConditions or not $working) And $MPDetected Then
			   $MP=GetHPCPMP($yMP, $MP_COLOR)
			   $xMP = $MP
			EndIf
			if $MobHPDetected Then
			   $MobHP=GetMobHP($yMobHP, $MobHP_COLOR)
			   $xMobHP = $MobHP
			EndIf
			$DongleConnected = SendCMD_SET_HPCPMP($xMobHP, $xHP, $xCP, $xMP)
			CPHPMPBarUpdate()
		EndIf
		 if TimerDiff($DongleCheckTimer) > 1000 Then
			$DongleCheckTimer = TimerInit()
			if GetFromHID() <>0 Then
			  if $working == 0 Then
				 $working = 1
				 ToggleWork()
			  EndIf
			Else
			  if $working == 1 Then
				 $working = 0
				 ToggleWork()
			   EndIf
			EndIf
		EndIf

#cs
		 if $L2Hwnd<>0 Then SendStatusToServer()
#ce


		if($working) And $L2Hwnd<>0 Then

			;--------------- Кнопконажималка ---------------
;			For $i = 1 To $MAXCONDITIONS Step 1
;
;				$n = FindPlayerByNic($TargetNic[$i])
;				$j = FindToggleGroupByName( $ToggleGroup[$i] )
;				if CheckCustomCondition($i, $n, $j) Then ; Condition N, Player N, ToggleGroup N
;					DoAction($i, $n, $j)
;				EndIf
;			Next
			;--------------- Мышкобродилка  ---------------
;			if $WalkSet <> 0 AND TimerDiff($WalkTimer) > $WalkCoolDownSet*1000 Then
;				if $HPDetected Then $HP=GetHPCPMP($yHP, $HP_COLOR)
;				if $CPDetected Then $CP=GetHPCPMP($yCP, $CP_COLOR)
;				if $MPDetected Then $MP=GetHPCPMP($yMP, $MP_COLOR)

;				MouseClick("left",396,440)
;				if $HPDetected Then
;					$HP=GetHPCPMP($yHP, $HP_COLOR)
;					if $CPDetected Then $CP=GetHPCPMP($yCP, $CP_COLOR)
;					if $MPDetected Then $MP=GetHPCPMP($yMP, $MP_COLOR)
;					$HPCheckTimer = TimerInit()
;				Else
;					Sleep(900+Random(10, 200, 1))
;				EndIf
;				WinActivate($L2Hwnd)
		;		if WinWaitActive($L2Hwnd, "", 5) <> 0 Then
;					MouseClick("left",641,520)
;					$WalkTimer = TimerInit()
		;		EndIf
;			EndIf

		EndIf


	WEnd

EndFunc   ;==>_Main

;--------------- Functions ---------------

Func OnFindL2()
	Local $YesID, $NoID
	Local $i=0, $FindL2BoxID
	Dim $var

	$var = WinList()
	$Windows[0][0] = 0
	For $i = 1 to $var[0][0]
	  ; Only display visble windows that have a title

	  If StringInStr($var[$i][0], $Title) Then
		$Windows[0][0] += 1
		$Windows[$Windows[0][0]][0] = $var[$i][0]
		$Windows[$Windows[0][0]][1] = $var[$i][1]
		;MsgBox(4096,"",$Windows[$Windows[0][0]][0])
	  EndIf
	Next
	if $Windows[0][0] > 1 Then
		$QuitBoxID = GUICreate("Choose L2 Client", 320, 170)
		GUICtrlCreateLabel("Choose L2 Client:", 10, 10)
		; LIST VIEW
		$listView = GuiCtrlCreateListView("HWND                     |Title                                                                                   ", 10, 29, 300, 100)

		For $i = 1 to $Windows[0][0]
		  ; Only display visble windows that have a title
		GuiCtrlCreateListViewItem($Windows[$i][1]&"|"&$Windows[$i][0], $listView)
		Next

		$YesID = GUICtrlCreateButton("ОК", 10, 135, 50, 20)
		GUICtrlSetOnEvent($YesID, "OnOK")
		$QuitID = GUICtrlCreateButton("Cancel", 150, 135, 50, 20)
		GUICtrlSetOnEvent($QuitID, "OnCancel")

		GUISetOnEvent($GUI_EVENT_CLOSE, "OnCancel")

		GUISetState()  ; display the GUIMsgBox
	Elseif $Windows[0][0] = 1 Then
		$L2Hwnd = $Windows[1][1]
		if InitHPCPMP() = -1 Then MsgBox(0, "Result", "HP CP MP bar not detected")

		if $CPDetected Then $CP=GetHPCPMP($yCP, $CP_COLOR)
		if $HPDetected Then $HP=GetHPCPMP($yHP, $HP_COLOR)
		if $MPDetected Then $MP=GetHPCPMP($yMP, $MP_COLOR)
		CPHPMPBarUpdate()

	Else
		$L2Hwnd = 0
	EndIf

EndFunc


Func OnWork() ; working ==> 1
	StartStopHID(1)
EndFunc

Func OnStop()  ; working ==> 0
	StartStopHID(0)
 EndFunc

 Func OnSuspend()  ; working ==> 0
	StartStopHID(0)
EndFunc


Func ToggleWork()  ; working 0 <==> 1
	;if $HotKey Then
	;	HotKeySet($HotKey)
	;	Send($HotKey)
	;	HotKeySet($HotKey,"ToggleWork")
	;EndIf

	;$working = Abs($working-1)
   ConsoleWrite(@LF &'ToggleWork'& @LF)

	If $working == 0 Then
		GuiCtrlSetBkColor($ActivityBox, 0xFF0000)
		GuiCtrlSetState($YesID , $GUI_UNCHECKED)
		GuiCtrlSetState($NoID, $GUI_CHECKED)
	 Else
		GuiCtrlSetState($YesID , $GUI_CHECKED)
		GuiCtrlSetState($NoID, $GUI_UNCHECKED)
		GuiCtrlSetBkColor($ActivityBox, 0x00FF00)
	 EndIf
	 ;$working = Abs($working-1)
EndFunc


Func Init(); find L2
	Local $workstate = $working, $i
	$working = 0
	For $i = 1 To $MAXCONDITIONS
		$FTimer [$i] = 0; TimerInit ( )
	Next
	;if $WalkSet <> 0 Then  MsgBox(4096,"","Walk#12")
	$HPDetected=0
	$CPDetected=0
	$MPDetected=0
	$HP=100
	$CP=100
	$MP=100
	$yHP=0
	$yCP=0
	$yMP=0
	$BarStart=0
	$BarLen=1
	$MobHPDetected=0
	$MobHP=0
	$yMobHP=0
	$MobBarStart=0
	$MobBarLen=1
	;InitToggleList()
	InitPlayerList()

	$HPCheckTimer  = TimerInit ( )
	$DongleCheckTimer  = $HPCheckTimer
 EndFunc

Func OnInit(); find L2
	Local $workstate = $working, $i
	if $working Then ToggleWork()
	;GuiCtrlSetBkColor($ActivityBox, $CP_COLOR)
	Init()
	if $workstate Then
		ToggleWork()
	Else
		GuiCtrlSetBkColor($ActivityBox, 0xFF0000)
	EndIf

	;	if $WalkSet <> 0 Then  MsgBox(4096,"","Walk#13")
EndFunc


Func OnFindMob(); find Mob HP Bar
Local   $MiddlePoint, $Height, $delta=50
Dim $coord[2], $CursorInfo[5]

	$MobBarStart=0
	$MobBarLen=1
	;MsgBox(4096,"",StringFormat("S: %d"), $L2Hwnd)
	If not WinExists ($L2Hwnd) Then
		$L2Hwnd = 0
		Return -1
	EndIf

	$CursorInfo = WinGetPos ($L2Hwnd)
	;MsgBox(4096,"",StringFormat("S: %d %d %d %d", $CursorInfo[0], $CursorInfo[1], $CursorInfo[2], $CursorInfo[3]))
	$MiddlePoint = $CursorInfo[2]/2
	$Height = $CursorInfo[3]/6
	;MsgBox(4096,"",StringFormat("Search area: %d %d %d %d", $MiddlePoint-$delta, 0, $MiddlePoint+$delta, $Height))

	$coord = PixelSearch($MiddlePoint-$delta, 0, $MiddlePoint+$delta, $Height, $MobHP_COLOR, 5, 1, $L2Hwnd) ;$MiddlePoint-$delta, 0, $MiddlePoint+$delta, $Height, $HP_COLOR, 5, 1, $L2Hwnd);$Height
	If  @error Then Return -1

	PixelSearch( $MiddlePoint-$delta, 0, $MiddlePoint+$delta, $coord[1],  $CP_COLOR, 5, 1, $L2Hwnd)
	If  Not @error Then 	Return -1 ; HP CP MP Bar Detected

	$MobHPDetected = 1
	$yMobHP = $coord[1]
	$MobBarStart = $coord[0]
	$MobBarMiddle = $coord[0]
	While($MobBarStart >0)
		;MsgBox(4096,"",StringFormat("Coord: %d %d %d %d ", $MobBarStart-7, $yMobHP-2, $MobBarStart, $yMobHP+2))
		$coord = PixelSearch( $MobBarStart-7, $yMobHP-2, $MobBarStart, $yMobHP+2, $HP_COLOR, 10, 1, $L2Hwnd)
		If Not @error Then
			$MobBarStart -= 7
		Else
			$coord = PixelSearch( $MobBarStart, $yMobHP-2, $MobBarStart+14, $yMobHP+2, $MobHP_COLOR, 10, 1, $L2Hwnd)
			If Not @error Then
				$MobBarStart = $coord[0]
				$MobBarLen = $MobBarMiddle - $MobBarStart
			EndIF
			ExitLoop
		EndIf
	WEnd
	While($MobBarStart+ $MobBarLen < $CursorInfo[2])
		$coord = PixelSearch( $MobBarStart+ $MobBarLen, $yMobHP-2, $MobBarStart+ $MobBarLen+7, $yMobHP+2, $MobHP_COLOR, 10, 1, $L2Hwnd)
		If Not @error Then
			$MobBarLen += 7
		Else
			$coord = PixelSearch( $MobBarStart+ $MobBarLen, $yMobHP-2, $MobBarStart+ $MobBarLen-14, $yMobHP+2, $HP_COLOR, 10, 1, $L2Hwnd)
			If Not @error Then
				$MobBarLen = $coord[0]-$MobBarStart
			EndIf
			ExitLoop
		EndIf
	WEnd
	;MsgBox(4096,"",StringFormat("Set Start: %d %d", $MobBarStart, $MobBarLen))

	Return 1

EndFunc

Func GetMobHP($Y, $Color) ; Parse Mob HP Bar
Local   $x=0,  $i=1, $coord, $MobBarParseTimer, $MiddlePoint, $delta, $xP
	If not WinExists ($L2Hwnd) Then
		$L2Hwnd = 0
		$MobHPDetected=0
		$MobHP=0
		$yMobHP=0
		$MobBarStart=0
		$MobBarLen=1
		Return(-1)
	EndIf
;MsgBox(4096,"",StringFormat("#1 start len: %d %d", $MobBarStart, $MobBarLen))
;Найти точку изменения цвета и посчитать % параметра
	$MobBarParseTimer = TimerInit()
	$MiddlePoint = $MobBarLen/2+10
	$delta = $MiddlePoint

	If  Not $IgnorePet Then
		PixelSearch( $MobBarStart, $Y+5, $MobBarStart+7, $Y+20,  $PET_MP_COLOR1, 5, 1, $L2Hwnd)
		If  Not @error Then 	Return 0 ; Pet MP Bar Detected
	Endif
	;PixelSearch( $MobBarStart, $Y+5, $MobBarStart+7, $Y+20,  $PET_MP_COLOR2, 5, 1, $L2Hwnd)
	;If  Not @error Then 	Return 0 ; Pet MP Bar Detected

	While($delta > 1)
		$coord = PixelSearch( $MobBarStart+$MiddlePoint, $Y-2, $MobBarStart+$MiddlePoint+7, $Y+2, $Color, 10, 1, $L2Hwnd)
		$delta /= 2
		If Not @error Then
			$x=$coord[0]-$MobBarStart
			iF $MobBarLen < $x Then $MobBarLen = $x
			$MiddlePoint += $delta
		Else
			$MiddlePoint -= $delta
		EndIf
	WEnd
	$MobBarCheckTime = Round(($MobBarCheckTime+TimerDiff($MobBarParseTimer))/2)
	$xP = Round(($x/$MobBarLen)*100)
	;MsgBox(4096,"",StringFormat("#1 start len proc: %d %d %d", $MobBarStart, $MobBarLen, $xP ))
	Return $xP
EndFunc

Func InitHPCPMP(); find HPCPMP Bar
Local  $coord
	;MsgBox(4096,"",StringFormat("S: %d"), $L2Hwnd)
	If not WinExists ($L2Hwnd) Then
		$L2Hwnd = 0
		Return -1
	Else

		WinActivate($L2Hwnd)
		;WinWaitActive($L2Hwnd, "", 5)
	EndIf
	$coord = PixelSearch( 0, 0, 300, 300, $HP_COLOR, 5, 1, $L2Hwnd)

	If Not @error Then
		$HPDetected = 1
		$yHP = $coord[1]
		$BarStart = $coord[0]
	EndIf

	$coord = PixelSearch( 0, 0, 300, 300, $CP_COLOR, 5, 1, $L2Hwnd)
	If Not @error Then
		$CPDetected = 1
		$yCP = $coord[1]
		iF $BarStart > $coord[0] Then $BarStart = $coord[0]
	EndIf

	$coord = PixelSearch( 0, 0, 300, 300,  $MP_COLOR, 5, 1, $L2Hwnd)
	If Not @error Then
		$MPDetected = 1
		$yMP = $coord[1]
		iF $BarStart > $coord[0] Then $BarStart = $coord[0]
	EndIf

	;MsgBox(0, "Result", $yCP & ", " & $yHP & ", " & $yMP & @CRLF & $CP & ", " & $HP & ", " & $MP)
	If ($MPDetected =0) AND ($CPDetected =0) AND ($HPDetected =0) Then Return -1
	Return 1
EndFunc

Func GetHPCPMP($Y, $Color) ; Parse HPCPMP Bar
Local   $x=0,  $i=1, $coord, $BarParseTimer, $MiddlePoint, $delta, $xP
	If not WinExists ($L2Hwnd) Then
		$L2Hwnd = 0
		$HPDetected=0
		$CPDetected=0
		$MPDetected=0
		$HP=100
		$CP=100
		$MP=100
		$yHP=0
		$yCP=0
		$yMP=0
		$BarStart=0
		$BarLen=1
		Return(100)
	EndIf

;Найти точку изменения цвета и посчитать % параметра
	$BarParseTimer = TimerInit()
	$MiddlePoint = $BarLen/2+10
	$delta = $MiddlePoint
	While($delta > 1)
		$coord = PixelSearch( $BarStart+$MiddlePoint, $Y-2, $BarStart+$MiddlePoint+7, $Y+2, $Color, 10, 1, $L2Hwnd)
		$delta /= 2
		If Not @error Then
			$x=$coord[0]-$BarStart
			iF $BarLen < $x Then $BarLen = $x
			$MiddlePoint += $delta
		Else
			$MiddlePoint -= $delta
		EndIf
	WEnd
	$BarCheckTime = Round(($BarCheckTime+TimerDiff($BarParseTimer))/2)
	$xP = Round(($x/$BarLen)*100)

	Return $xP

EndFunc

Func GUICtrlSetDataCondition($X, $XSet)
	if $XSet = 0xFF Then
		GUICtrlSetData($X, "")
	Else
		GUICtrlSetData($X, $XSet)
	EndIf
EndFunc

Func OnUpdateTab()
	UpdateTab($GUI_SHOW)
EndFunc

Func UpdateTab($HS)
	Local  $n, $i, $j
	$j = $HS
	$n = GUICtrlRead($TabID)

   If $n == 0 Then
	  For $i = 0 To 7
		   GUICtrlSetState($LabelID[$i], $GUI_HIDE)
	  Next
	  For $i = 14 To 17
		   GUICtrlSetState($LabelID[$i], $GUI_HIDE)
	  Next

	  For $j = 1 To  $MAXCONDITIONSPERPAGE
	   	GUICtrlSetState($treeOne[$j], $HS)
	  Next
   Else
	  For $i = 0 To 7
		   GUICtrlSetState($LabelID[$i], $HS)
	  Next
	  For $i = 14 To 17
		   GUICtrlSetState($LabelID[$i], $HS)
	  Next
	  For $j = 1 To  $MAXCONDITIONSPERPAGE
	   	GUICtrlSetState($treeOne[$j], $GUI_HIDE)
	  Next
   EndIf

	for $i = 1 To $MAXTABS
		if $i = $n Then
			ToggleTab($i, $HS)
		Else
			ToggleTab($i, $GUI_HIDE)
		EndIf
	Next
	OnUpdateSet()
EndFunc

Func OnToggleWideView()
	Local $state, $Ycoord
	Dim $CursorInfo[5]

	$CursorInfo = WinGetPos ($hWndMain)
	if GUICtrlRead($ToggleWideViewID) = ">>>>>>" Then
		$state = $GUI_HIDE
		$guiYPos = 10
		$Ycoord = $CursorInfo[0] + 800+90+40+40
		GUICtrlSetData($ToggleWideViewID, "<<<<<<")
	Else
		$state = $GUI_SHOW
		$guiYPos = 810+90+40+40
		$Ycoord = $CursorInfo[0] - 800-90-40-40
		GUICtrlSetData($ToggleWideViewID, ">>>>>>")
	Endif

	WinSetState ($hWndMain, "", @SW_HIDE)

	WinMove ( $hWndMain, "", $Ycoord, $CursorInfo[1], 6+110+$guiYPos,  $CursorInfo[3])

	ToggleWideView($state)
	WinSetState ($hWndMain, "", @SW_SHOW)
EndFunc

Func ToggleWideView($state)
	Local $guiFstate, $guiNstate
	GUICtrlSetState($TabID, $state)
	UpdateTab($state)

	GUICtrlSetState($treeShift, $state)
	GUICtrlSetState($treeTwo, $state)
	GUICtrlSetState($WalkCoolDown, $state)
	GUICtrlSetState($NicNameID, $state)
	GUICtrlSetState($ServerID, $state)
	;GUICtrlSetState($ShiftStateID, $state)
	;GUICtrlSetState($CtrlStateID, $state)



;	For $i = 0 To 9
;		GUICtrlSetState($LabelID[$i], $state)
;	Next
;	For $i = 14 To 17
;		GUICtrlSetState($LabelID[$i], $state)
;	Next
	GUICtrlSetState($PlayersListID, $state)

	GUICtrlDelete($LabelID[10])
	$LabelID[10] = GuiCtrlCreateLabel($NicName, $guiYPos, 7)

   Local $CurrentY
   $CurrentY = 29
	GUICtrlSetPos($ExitID, $guiYPos, $CurrentY, 100, 20)
   $CurrentY += 20
	GUICtrlSetPos($SaveID, $guiYPos, $CurrentY, 100, 20)
   $CurrentY += 20
	GUICtrlSetPos($OpenID, $guiYPos, $CurrentY, 100, 20)
   $CurrentY += 20
	GUICtrlSetPos($SaveToDongleID, $guiYPos, $CurrentY, 100, 20)

	; GROUP WITH RADIO Buttons
    $CurrentY += 25
	GUICtrlSetPos($ActivityBox, $guiYPos, $CurrentY, 100, 80)
    $CurrentY += 15
 	GUICtrlSetPos($YesID, $guiYPos+15, $CurrentY, 60)
   $CurrentY += 20
	GUICtrlSetPos($NoID, $guiYPos+15, $CurrentY, 60)
    $CurrentY += 20
	GUICtrlSetPos($SuspendID, $guiYPos+15, $CurrentY, 60)


    $CurrentY += 30
	GUICtrlSetPos ( $ShiftStateID, $guiYPos+5, $CurrentY,50, 18)
 	GUICtrlSetPos($CtrlStateID,$guiYPos+65, $CurrentY, 50, 18)

    $CurrentY += 20

	GUICtrlSetPos($FindL2ID, $guiYPos, $CurrentY, 100, 20)
	GUICtrlSetPos($ToggleWideViewID, $guiYPos, 15+19*18+5, 100, 20)

; HP CP MP Bar
	GUICtrlSetPos($LabelID[11],$guiYPos, 15+11*18+$guiXPos, 101, 10)
	GUICtrlSetPos($LabelID[12],$guiYPos, 15+11*18+10+$guiXPos, 101, 10)
	GUICtrlSetPos($LabelID[13], $guiYPos, 15+11*18+20+$guiXPos, 101, 10)
	CPHPMPBarUpdate()

	;GUICtrlSetPos($LabelID[10], $guiYPos+1, 15+13*18+5+$guiXPos)
	GUICtrlDelete($BarCheckTimeID)
	$BarCheckTimeID = GuiCtrlCreateLabel("Parse, ms:"& $BarCheckTime, $guiYPos, 15+13*18+5+$guiXPos)


	GUICtrlSetPos($FindMobID, $guiYPos, 15+14*18+5+5+$guiXPos, 100, 20)
	GUICtrlSetPos($LabelID[18],$guiYPos, 15+16*18+$guiXPos, 101, 5)
	GUICtrlDelete($MobBarCheckTimeID)
	$MobBarCheckTimeID = GuiCtrlCreateLabel("Parse, ms:"& $MobBarCheckTime, $guiYPos, 15+16*18+5+$guiXPos)
	OnUpdateSet()
EndFunc

Func ToggleTab($TabNum, $state)
	Local $i
	;MsgBox(4096,"",GUICtrlRead($TabID))
		;GUICtrlSetState($treeOne[$TabNum], $state)
	  GUICtrlSetState($treeState[($TabNum-1)], $state)
	  GUICtrlSetState($treeStateCondition[($TabNum-1)], $state)

	 For $i = 1 To $MAXCONDITIONSPERPAGE
 ;		;Fx CheckBox
		 ;Fx Target Player Box
		 GUICtrlSetState($TargetNicID[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 ;Fx Spam Button Box
		 GUICtrlSetState($ButtonsID[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 ;Fx CoolDown Box
		 GUICtrlSetState($FCoolDown[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 ;Fx Pause Box
		 GUICtrlSetState($FPause[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 ;Fx Toggle Box
		 ;GUICtrlSetState($ToggleGroupID[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)

		 ;Condition Idle Time
		 GUICtrlSetState($FConditionIdle[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)

		 ;Fx Conditions Box
		 GUICtrlSetState($CheckNicID[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 GUICtrlSetState($FHPMin[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 GUICtrlSetState($FHPMax[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 GUICtrlSetState($FMPMin[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 GUICtrlSetState($FMPMax[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 GUICtrlSetState($FCPMin[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 GUICtrlSetState($FCPMax[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 GUICtrlSetState($FMobHPMin[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
		 GUICtrlSetState($FMobHPMax[$i+($TabNum-1)*$MAXCONDITIONSPERPAGE], $state)
	 Next
EndFunc


Func MainWindowCreate()

	$hWndMain = GUICreate("Boredom Breaker", $guiYPos+110, 70+18*18)
	; TAB
	$TabID = GuiCtrlCreateTab(0, 0, $guiYPos, 70+14*18)
	GUICtrlSetOnEvent($TabID, "OnUpdateTab")
	;GuiCtrlCreateTabItem("Conditions 1-12")
	;GuiCtrlCreateTabItem("Conditions 13-24")
	GuiCtrlCreateTabItem("")

	$LabelID[0] = GUICtrlCreateLabel("Target Player",  54, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)
	$LabelID[1] = GUICtrlCreateLabel("button",  40+100, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)
	$LabelID[2] = GUICtrlCreateLabel("CoolDown (s)",  79+100, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)
	$LabelID[3] = GUICtrlCreateLabel("Pause (s)",  150+100, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)
	$LabelID[4] = GUICtrlCreateLabel("Check Player",  325, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)

	$LabelID[14] = GUICtrlCreateLabel("Condition Idle Time",  225+200, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)
	$LabelID[15] = GUICtrlCreateLabel("",  215+200+90+40, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)
	$LabelID[16] = GUICtrlCreateLabel("",  215+200+90+50, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)

	$LabelID[5] = GUICtrlCreateLabel("<-- HP, % -->",  225+200+90+40+40, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)
	$LabelID[6] = GUICtrlCreateLabel("<-- CP, % -->", 425+200+90+40+40, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)
	$LabelID[7] = GUICtrlCreateLabel("<-- MP, % -->",  325+200+90+40+40, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)

	$LabelID[17] = GUICtrlCreateLabel("<-Mob HP, %->",  525+200+90+40+40, 10+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)


	; Create Main Dialog Window

   GuiCtrlCreateTabItem("Toggle")

	For $j = 1 To  $MAXCONDITIONSPERPAGE
	   	$treeOne[$j] = GuiCtrlCreateTreeView(9+$j*70, 29+$guiXPos, 40, 200+1*18, BitOR($TVS_CHECKBOXES, $TVS_NOSCROLL))
		For $i = $MAXTABS - 1 To 0 Step -1
			$F[$MAXCONDITIONSPERPAGE*$i+$j] = GuiCtrlCreateTreeViewItem($Buttons[$MAXCONDITIONSPERPAGE*$i+$j], $treeOne[$j]) ; StringFormat("F%d",$i)
			if $FSet[$MAXCONDITIONSPERPAGE*$i+$j] <> 0 Then GuiCtrlSetState(-1, $GUI_CHECKED)
			GUICtrlSetOnEvent($F[$MAXCONDITIONSPERPAGE*$i+$j], "OnUpdateSet")
		Next
	Next
   GUICtrlSetState(-1, $GUI_SHOW) ; будет отображаться выбранной

	For $j = 0 To $MAXTABS - 1
	GuiCtrlCreateTabItem("Line " & 1+$j)
	;$treeOne[$j] = GuiCtrlCreateTreeView(9, 29+$guiXPos, 40, 200+1*18, $TVS_CHECKBOXES)
	$treeState[$j] = GuiCtrlCreateTreeView(215+200+90, 29+$guiXPos, 40, 200+1*18, $TVS_CHECKBOXES)
	$treeStateCondition[$j] = GuiCtrlCreateTreeView(215+200+90+40, 29+$guiXPos, 40, 200+1*18, $TVS_CHECKBOXES)
		For $i = 1 To $MAXCONDITIONSPERPAGE
			;Fx CheckBox
			;$F[$i] = GuiCtrlCreateTreeViewItem($Buttons[$i], $treeOne[0]) ; StringFormat("F%d",$i)
			;Fx Target Player Box
			$TargetNicID[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($TargetNic[$i+$MAXCONDITIONSPERPAGE*$j], 49, 11+$i*18+$guiXPos, 90, 20)
			GUICtrlSetOnEvent($TargetNicID[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			;Fx Spam Button Box
			$ButtonsID[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($Buttons[$i+$MAXCONDITIONSPERPAGE*$j], 43+100, 11+$i*18+$guiXPos, 34, 20)
			GUICtrlSetOnEvent($ButtonsID[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			;Fx CoolDown Box
			$FCoolDown[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FCoolDownSet[$i+$MAXCONDITIONSPERPAGE*$j], 79+100, 11+$i*18+$guiXPos, 65, 20)
			GUICtrlSetOnEvent($FCoolDown[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			;Fx Pause Box
			$FPause[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FPauseSet[$i+$MAXCONDITIONSPERPAGE*$j], 150+100, 11+$i*18+$guiXPos, 50, 20)
			GUICtrlSetOnEvent($FPause[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")

			;Fx Check Player Box
			$CheckNicID[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($CheckNic[$i+$MAXCONDITIONSPERPAGE*$j], 215+100, 11+$i*18+$guiXPos, 90, 20)
			GUICtrlSetOnEvent($CheckNicID[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")


			;Condition Idle Time
			$FConditionIdle[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FConditionIdleSet[$i+$MAXCONDITIONSPERPAGE*$j], 215+200, 11+$i*18+$guiXPos, 90, 20)
			GUICtrlSetOnEvent($FConditionIdle[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")

			;Toggle group

			;$ToggleGroupID[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($ToggleGroup[$i+$MAXCONDITIONSPERPAGE*$j], 215+200, 11+$i*18+$guiXPos, 90, 20)
			;GUICtrlSetOnEvent($ToggleGroup[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")

			;$ToggleStateID[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateTreeViewItem("", $treeState[$j])
			;GUICtrlSetOnEvent($ToggleStateID[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")

			;$ToggleStateConditionID[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateTreeViewItem("", $treeStateCondition[$j])
			;GUICtrlSetOnEvent($ToggleStateConditionID[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")

			;if $ToggleStateCondition[$i+$MAXCONDITIONSPERPAGE*$j] <> 0 Then GuiCtrlSetState(-1, $GUI_CHECKED)

			;Fx Conditions Box
			$FHPMin[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FHPMinSet[$i+$MAXCONDITIONSPERPAGE*$j], 215+200+40+40+90, 11+$i*18+$guiXPos, 40, 20)
			GUICtrlSetOnEvent($FHPMin[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			$FHPMax[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FHPMaxSet[$i+$MAXCONDITIONSPERPAGE*$j], 260+200+40+40+90, 11+$i*18+$guiXPos, 40, 20)
			GUICtrlSetOnEvent($FHPMax[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			$FMPMin[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FMPMinSet[$i+$MAXCONDITIONSPERPAGE*$j], 315+200+40+40+90, 11+$i*18+$guiXPos, 40, 20)
			GUICtrlSetOnEvent($FMPMin[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			$FMPMax[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FMPMaxSet[$i+$MAXCONDITIONSPERPAGE*$j], 360+200+40+40+90, 11+$i*18+$guiXPos, 40, 20)
			GUICtrlSetOnEvent($FMPMax[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			$FCPMin[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FCPMinSet[$i+$MAXCONDITIONSPERPAGE*$j], 415+200+40+40+90, 11+$i*18+$guiXPos, 40, 20)
			GUICtrlSetOnEvent($FCPMin[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			$FCPMax[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FCPMaxSet[$i+$MAXCONDITIONSPERPAGE*$j], 460+200+40+40+90, 11+$i*18+$guiXPos, 40, 20)
			GUICtrlSetOnEvent($FCPMax[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			$FMobHPMin[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FMobHPMinSet[$i+$MAXCONDITIONSPERPAGE*$j], 515+200+40+40+90, 11+$i*18+$guiXPos, 40, 20)
			GUICtrlSetOnEvent($FMobHPMin[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")
			$FMobHPMax[$i+$MAXCONDITIONSPERPAGE*$j] = GuiCtrlCreateInput($FMobHPMaxSet[$i+$MAXCONDITIONSPERPAGE*$j], 560+200+40+40+90, 11+$i*18+$guiXPos, 40, 20)
			GUICtrlSetOnEvent($FMobHPMax[$i+$MAXCONDITIONSPERPAGE*$j], "OnUpdateSet")


			;$FTimer [$i+12*$j] = TimerInit ( ) - $FCoolDownSet[$i+12*$j]*1000+5000
			;$CoolDownTimer [$i+12*$j] = TimerInit ( ) - $CoolDown[$i+12*$j]*1000+5000
		Next
	Next


	GuiCtrlCreateTabItem("")

	$treeTwo = GuiCtrlCreateTreeView(29, 9+14*18+$guiXPos, 50, 18, $TVS_CHECKBOXES)
	$Walk = GuiCtrlCreateTreeViewItem("Walk", $treeTwo)
	GUICtrlSetOnEvent($Walk, "OnUpdateSet")
	if $WalkSet <> 0 Then
		GuiCtrlSetState($Walk, $GUI_CHECKED)
	Else
		GuiCtrlSetState($Walk, $GUI_UNCHECKED)
	EndIf
	$WalkTimer = TimerInit ( )
   Local $xShift
   $xShift = 79
	$WalkCoolDown = GuiCtrlCreateInput($WalkCoolDownSet, $xShift, 9+14*18+$guiXPos, 65, 20)
	GUICtrlSetOnEvent($WalkCoolDown, "OnUpdateSet")

   $xShift += 100
	$LabelID[8] = GUICtrlCreateLabel("Nic Name:",  $xShift, 9+14*18+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)

   $xShift += 60
	$NicNameID = GuiCtrlCreateInput($NicName, $xShift, 9+14*18+$guiXPos, 100, 20)
	GUICtrlSetOnEvent($NicNameID, "OnUpdateSet")

	$xShift += 160
	$LabelID[9] = GUICtrlCreateLabel("Server:",  $xShift, 9+14*18+$guiXPos)
	GuiCtrlSetBkColor(-1, 0xFFFFFF)

	$xShift += 60
	$ServerID = GuiCtrlCreateInput($ServerIP, $xShift, 9+14*18+$guiXPos, 100, 20)
	GUICtrlSetOnEvent($ServerID, "OnUpdateSet")


	GUICtrlSetOnEvent($ShiftStateID, "OnUpdateSet")
	GUICtrlSetOnEvent($CtrlStateID, "OnUpdateSet")
    GuiCtrlSetState($ShiftStateID, $GUI_UNCHECKED)
    GuiCtrlSetState($CtrlStateID, $GUI_UNCHECKED)


;$ShiftState, $CtrlState,  $ShiftStateID, $CtrlStateID


	$PlayersListID = GUICtrlCreateListView("Client IP Address|Port|NicName|HP|MP|CP|Mob HP|Time|",  1, 15*20+$guiXPos, $guiYPos-2, 70)
	GUICtrlSetLimit(-1, 200) 	; to limit horizontal scrolling
	;GUICtrlSetData($PlayersListID, "ehhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhrt")

	;GUICtrlCreateLabel("(при значении равном нулю условие игнорируется)",  225, 11+13*18+$guiXPos)
	;GuiCtrlSetBkColor(-1, 0xFFFFFF)
	;$PlayersID = GUICtrlCreateLabel("",  25, 9+16*18+$guiXPos)
	;GuiCtrlSetBkColor($PlayersID, 0xFFFFFF)


   Local $CurrentY
   $CurrentY = 29
	$ExitID = GUICtrlCreateButton("Exit", $guiYPos, $CurrentY, 100, 20)
	GUICtrlSetOnEvent($ExitID, "OnExit")

   $CurrentY += 20
   $OpenID = GUICtrlCreateButton("Open", $guiYPos, $CurrentY, 100, 20)
	GUICtrlSetOnEvent($OpenID, "OnOpen")
   $CurrentY += 20
	$SaveID = GUICtrlCreateButton("Save", $guiYPos, $CurrentY, 100, 20)
	GUICtrlSetOnEvent($SaveID, "OnSave")
   $CurrentY += 20
	$SaveToDongleID = GUICtrlCreateButton("Save To Dongle", $guiYPos, $CurrentY, 100, 20)
	GUICtrlSetOnEvent($SaveToDongleID, "OnSaveToDongle")

	; GROUP WITH RADIO Buttons
    $CurrentY += 30
	$ActivityBox = GuiCtrlCreateGroup("Activity:", $guiYPos, $CurrentY, 100, 80)
	GuiCtrlSetBkColor($ActivityBox, $CP_COLOR)
    $CurrentY += 15
    $YesID = GuiCtrlCreateRadio("Work", $guiYPos+15, $CurrentY, 60)
    $CurrentY += 20
	$NoID = GuiCtrlCreateRadio("Stop", $guiYPos+15, $CurrentY, 60)
    $CurrentY += 20
	$SuspendID = GuiCtrlCreateRadio("Suspend", $guiYPos+15, $CurrentY, 60)
		GUICtrlCreateGroup ("",-99,-99,1,1)  ;close group

	if $working <> 0 Then
		GuiCtrlSetState($YesID , $GUI_CHECKED)
		GuiCtrlSetState($NoID, $GUI_UNCHECKED)
	Else
		GuiCtrlSetState($YesID , $GUI_UNCHECKED)
		GuiCtrlSetState($NoID, $GUI_CHECKED)
	EndIf

	GUICtrlSetOnEvent($YesID, "OnWork")
	GUICtrlSetOnEvent($NoID, "OnStop")

    $CurrentY += 30

	$ShiftStateID = GUICtrlCreateCheckbox ( "Shift", $guiYPos, $CurrentY,50, 18)
	GUICtrlSetOnEvent($ShiftStateID, "OnSetModifier")

 	$CtrlStateID = GUICtrlCreateCheckbox("Ctrl",$guiYPos+60, $CurrentY, 50, 18)
	GUICtrlSetOnEvent($CtrlStateID, "OnSetModifier")

    $CurrentY += 30
	$FindL2ID = GUICtrlCreateButton("Find L2", $guiYPos, $CurrentY, 100, 20)
	GUICtrlSetOnEvent($FindL2ID, "OnFindL2")


	; HP CP MP Bar
	$LabelID[11] = GuiCtrlCreateLabel("",$guiYPos, 15+11*18+$guiXPos, 101, 10)
	GuiCtrlSetBkColor(-1, 0x808080)
	$LabelID[12] =  GuiCtrlCreateLabel("",$guiYPos, 15+11*18+10+$guiXPos, 101, 10)
	GuiCtrlSetBkColor(-1, 0x808080)
	$LabelID[13] = GuiCtrlCreateLabel("", $guiYPos, 15+11*18+20+$guiXPos, 101, 10)
	GuiCtrlSetBkColor(-1, 0x808080)
	$CPBar = GuiCtrlCreateLabel("",$guiYPos, 15+11*18+$guiXPos, 1, 10)
	GuiCtrlSetBkColor(-1, $CP_COLOR)
	$HPBar = GuiCtrlCreateLabel("",$guiYPos, 15+11*18+10+$guiXPos, 1, 10)
	GuiCtrlSetBkColor(-1, $HP_COLOR)
	$MPBar = GuiCtrlCreateLabel("", $guiYPos, 15+11*18+20+$guiXPos, 1, 10)
	GUICtrlSetBkColor(-1, $MP_COLOR)
	$BarCheckTimeID = GuiCtrlCreateLabel("Parse, ms:"& $BarCheckTime, $guiYPos, 15+13*18+5+$guiXPos)

	$FindMobID = GUICtrlCreateButton("Find Mob", $guiYPos, 15+14*18+5+5+$guiXPos, 100, 20)
	GUICtrlSetOnEvent($FindMobID, "OnFindMob")

	$LabelID[18] = GuiCtrlCreateLabel("",$guiYPos, 15+16*18+$guiXPos, 101, 5)
	GuiCtrlSetBkColor(-1, 0x808080)
	$MobHPBar = GuiCtrlCreateLabel("",$guiYPos, 115+16*18+$guiXPos, 1, 5)
	GuiCtrlSetBkColor(-1, $HP_COLOR)
	$MobBarCheckTimeID = GuiCtrlCreateLabel("Parse, ms:"& $MobBarCheckTime, $guiYPos, 15+16*18+5+$guiXPos)

	$LabelID[10] = GuiCtrlCreateLabel($NicName, $guiYPos, 7)
	$ToggleWideViewID = GUICtrlCreateButton(">>>>>>", $guiYPos, 15+19*18+5, 100, 20)
	GUICtrlSetOnEvent($ToggleWideViewID, "OnToggleWideView")
	GUISetOnEvent($GUI_EVENT_CLOSE, "OnExit")
EndFunc

Func OnSave()
	Local $message, $var, $CfgFile
	; Multiple filter group
	;$message = "Hold down Ctrl or Shift to choose multiple files."

	$var = FileSaveDialog("Save Boredom Breaker Config File", @MyDocumentsDir & "", "Configs (*.cfg)|All Files (*.*)", 0, $LastConfig )

	If @error Then
		;MsgBox(4096,"","No File(s) chosen")
	Else
		$var = StringReplace($var, "|", @CRLF)
		$LastConfig = $var
		OnSaveIni($var)
	EndIf
EndFunc


Func OnSaveToDongle()
   StartStopHID(0)
   $DongleConnected = SendCMD_DELETE_ALL()
   For $i = 1 To $MAXCONDITIONS Step 1
   if  $FSet[$i] == 1 Then
	  ConsoleWrite(@LF &'Send to Dongle: '& $Buttons[$i] & ' ' &  $FCoolDownSet[$i] & @LF)
	  $DongleConnected = SendCMD_ADD_NODE($Buttons[$i], $FPauseSet[$i]*1000, $FCoolDownSet[$i]*1000, $FConditionIdleSet[$i]*1000) ;
	  $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_MobHP, $FMobHPMinSet[$i], $FMobHPMaxSet[$i])
	  $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_playerHP, $FHPMinSet[$i], $FHPMaxSet[$i])
	  $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_playerCP, $FCPMinSet[$i], $FCPMaxSet[$i])
	  $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_playerMP, $FMPMinSet[$i], $FMPMaxSet[$i])
   EndIf
   $DongleConnected = SendCMD_SET_MODIFIER( BitOR($ShiftState*$HID_KEYBOARD_MODIFIER_LEFTSHIFT, $CtrlState*$HID_KEYBOARD_MODIFIER_LEFTCTRL)    )
   Next
   $DongleConnected = SendCMD_WRITE_CONFIG()
   StartStopHID($working)
EndFunc


Func OnOpen()
	Local $message, $var, $CfgFile
	; Multiple filter group
	;$message = "Open Boredom Breaker Config File"

	$var = FileOpenDialog("Open Boredom Breaker Config File", @MyDocumentsDir & "", "Configs (*.cfg)|All Files (*.*)", 1 + 2, $LastConfig )

	If @error Then
		;MsgBox(4096,"","No File(s) chosen")

	Else
		;LoadConfig($var, 1)
		$LastConfig = $var
		OnInit()
		InitHPCPMP()
		LoadIni($var)
	EndIf
EndFunc


Func OnSaveIni($var)

	; CONFIG BEGIN
	IniWrite ( $var, "MAIN", "WalkCoolDownSet", $WalkCoolDownSet)
	IniWrite ( $var, "MAIN", "WalkSet", $WalkSet)
	IniWrite ( $var, "MAIN", "NicName",  $NicName)
	IniWrite ( $var, "MAIN", "ServerIP", $ServerIP)
	IniWrite ( $var, "MAIN", "$HttpServerIP", $HttpServerIP)
	For $i = 1 To $MAXCONDITIONS Step 1
			IniWrite($var, "CONDITION"&$i, "$FSet", $FSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FCoolDownSet", $FCoolDownSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FConditionIdleSet", $FConditionIdleSet[$i])
			;IniWrite($var, "CONDITION"&$i, "$CoolDown", $CoolDown[$i])
			;IniWrite($var, "CONDITION"&$i, "$ToggleGroup", $ToggleGroup[$i])
			;IniWrite($var, "CONDITION"&$i, "$ToggleStateCondition", $ToggleStateCondition[$i])
			IniWrite($var, "CONDITION"&$i, "$FPauseSet", $FPauseSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FHPMinSet", $FHPMinSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FHPMaxSet", $FHPMaxSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FCPMinSet", $FCPMinSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FCPMaxSet", $FCPMaxSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FMPMinSet", $FMPMinSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FMPMaxSet", $FMPMaxSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FMobHPMinSet", $FMobHPMinSet[$i])
			IniWrite($var, "CONDITION"&$i, "$FMobHPMaxSet", $FMobHPMaxSet[$i])
			IniWrite($var, "CONDITION"&$i, "$Buttons", $Buttons[$i])
			IniWrite($var, "CONDITION"&$i, "$TargetNic", $TargetNic[$i])
			IniWrite($var, "CONDITION"&$i, "$CheckNic", $CheckNic[$i])

		Next
		MsgBox(4096,"","Saved")
	; CONFIG END
EndFunc

Func LoadIni($var)
   $DongleConnected = SendCMD_DELETE_ALL()

	$var = StringReplace($var, "|", @CRLF)

	; CONFIG BEGIN
		;if $WalkSet <> 0 Then  MsgBox(4096,"","Walk#4")
	$WalkCoolDownSet = IniRead( $var, "MAIN", "WalkCoolDownSet",  40)
	GuiCtrlSetData($WalkCoolDown, $WalkCoolDownSet)
	$WalkSet = IniRead( $var, "MAIN", "WalkSet",  0)
	if $WalkSet <> 0 Then
		GuiCtrlSetState($Walk, $GUI_CHECKED)
		$WalkSet = 1
	Else
		GuiCtrlSetState($Walk, $GUI_UNCHECKED)
	EndIf
	;GUICtrlSetData($FPause[$i], $WalkSet)
	$NicName = IniRead( $var, "MAIN", "NicName",  "Noname")

	GuiCtrlSetData($NicNameID, $NicName)
	$ServerIP = IniRead( $var, "MAIN", "ServerIP", @IPAddress1)
	GuiCtrlSetData($ServerID, $ServerIP)
	$HttpServerIP = IniRead( $var, "MAIN", "$HttpServerIP", @IPAddress1)

	;MsgBox(4096,"","Nic" & $NicName & " Server: " & $ServerIP)

	For $i = 1 To $MAXCONDITIONS Step 1
		$FSet[$i] = IniRead($var, "CONDITION"&$i, "$FSet", 0)
		if $FSet[$i] <> 0 Then
			GuiCtrlSetState($F[$i], $GUI_CHECKED)
			$FSet[$i] = 1
		Else
			GuiCtrlSetState($F[$i], $GUI_UNCHECKED)
		EndIf
		$FCoolDownSet[$i] = IniRead($var, "CONDITION"&$i, "$FCoolDownSet", 1)
		GUICtrlSetData($FCoolDown[$i], $FCoolDownSet[$i])

		$FConditionIdleSet[$i] = IniRead($var, "CONDITION"&$i, "$FConditionIdleSet", 0)
		GUICtrlSetData($FConditionIdle[$i], $FConditionIdleSet[$i])


		;$CoolDown[$i] = IniRead($var, "CONDITION"&$i, "$CoolDown", 30)
		;GUICtrlSetData($CoolDownID[$i], $CoolDown[$i])

		;$ToggleGroup[$i] = IniRead($var, "CONDITION"&$i, "$ToggleGroup", "")
		;GUICtrlSetData($ToggleGroupID[$i], $ToggleGroup[$i])
		;$ToggleStateCondition[$i] = IniRead($var, "CONDITION"&$i, "$ToggleStateCondition", 1)
		;GUICtrlSetData($ToggleStateConditionID[$i], $ToggleStateCondition[$i])
		;if $ToggleStateCondition[$i] <> 0 Then
		;	GuiCtrlSetState($ToggleStateConditionID[$i], $GUI_CHECKED)
		;	$ToggleStateCondition[$i] = 1
		;Else
		;	GuiCtrlSetState($ToggleStateConditionID[$i], $GUI_UNCHECKED)
		;EndIf


		$FPauseSet[$i] = IniRead($var, "CONDITION"&$i, "$FPauseSet", 0)
		GUICtrlSetData($FPause[$i], $FPauseSet[$i])
		$FHPMinSet[$i] = PercentMask(IniRead($var, "CONDITION"&$i, "$FHPMinSet", 0xFF))
		$FHPMaxSet[$i] = PercentMask(IniRead($var, "CONDITION"&$i, "$FHPMaxSet",0xFF))
		$FCPMinSet[$i] = PercentMask(IniRead($var, "CONDITION"&$i, "$FCPMinSet", 0xFF))
		$FCPMaxSet[$i] = PercentMask(IniRead($var, "CONDITION"&$i, "$FCPMaxSet", 0xFF))
		$FMPMinSet[$i] = PercentMask(IniRead($var, "CONDITION"&$i, "$FMPMinSet", 0xFF))
		$FMPMaxSet[$i] = PercentMask(IniRead($var, "CONDITION"&$i, "$FMPMaxSet", 0xFF))
		$FMobHPMinSet[$i] = PercentMask(IniRead($var, "CONDITION"&$i, "$FMobHPMinSet", 0xFF))
		$FMobHPMaxSet[$i] = PercentMask(IniRead($var, "CONDITION"&$i, "$FMobHPMaxSet",0xFF))

		$Buttons [$i] = IniRead($var, "CONDITION"&$i, "$Buttons", $Buttons [$i] )
		GUICtrlSetData($ButtonsID[$i], $Buttons [$i])
		$TargetNic [$i] = IniRead($var, "CONDITION"&$i, "$TargetNic", "")
		GUICtrlSetData($TargetNicID[$i], $TargetNic [$i])
		$CheckNic [$i] = IniRead($var, "CONDITION"&$i, "$CheckNic", "")
		GUICtrlSetData($CheckNicID[$i], $CheckNic [$i])

		if  $FSet[$i] == 1 Then
			   ConsoleWrite(@LF &'Ini Add Key: '& $Buttons[$i] & ' ' &  $FCoolDownSet[$i] & @LF)
			   $DongleConnected = SendCMD_ADD_NODE($Buttons[$i], $FPauseSet[$i]*1000, $FCoolDownSet[$i]*1000, $FConditionIdleSet[$i]*1000) ;
			   $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_MobHP, $FMobHPMinSet[$i], $FMobHPMaxSet[$i])
			   $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_playerHP, $FHPMinSet[$i], $FHPMaxSet[$i])
			   $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_playerCP, $FCPMinSet[$i], $FCPMaxSet[$i])
			   $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_playerMP, $FMPMinSet[$i], $FMPMaxSet[$i])
		 EndIf
		GUICtrlSetDataCondition($FHPMin[$i], $FHPMinSet[$i])
		GUICtrlSetDataCondition($FHPMax[$i], $FHPMaxSet[$i])
		GUICtrlSetDataCondition($FCPMin[$i], $FCPMinSet[$i])
		GUICtrlSetDataCondition($FCPMax[$i], $FCPMaxSet[$i])
		GUICtrlSetDataCondition($FMPMin[$i], $FMPMinSet[$i])
		GUICtrlSetDataCondition($FMPMax[$i], $FMPMaxSet[$i])
		GUICtrlSetDataCondition($FMobHPMin[$i], $FMobHPMinSet[$i])
		GUICtrlSetDataCondition($FMobHPMax[$i], $FMobHPMaxSet[$i])


	Next
	;UpdateToggleList()
		;if $WalkSet <> 0 Then  MsgBox(4096,"","Walk#5")
	; CONFIG END
EndFunc


Func Cleanup()
    UDPCloseSocket($SndSocket)
	UDPCloseSocket($SrvSocket)
	UDPCloseSocket($RcvSocket)
    UDPShutdown()
EndFunc


Func OnExit()
	Local $YesID, $NoID
	$QuitBoxID = GUICreate("Quit Msgbox", 210, 80)

	GUICtrlCreateLabel("Terminate Script?", 10, 10)
	$YesID = GUICtrlCreateButton("Quit", 10, 50, 50, 20)
	GUICtrlSetOnEvent($YesID, "OnQuit")
	$QuitID = GUICtrlCreateButton("Cancel", 150, 50, 50, 20)
	GUICtrlSetOnEvent($QuitID, "OnCancel")
    CloseHID($hDevice)
	GUISetOnEvent($GUI_EVENT_CLOSE, "OnCancel")

	GUISetState()  ; display the GUIMsgBox
EndFunc


Func OnQuit()
	Exit
EndFunc   ;==>OnQuit

Func OnCancel()
	GUIDelete($QuitBoxID)
EndFunc   ;==>OnCancel

Func OnOK()
	Local $txtHwnd, $txtHwnd2
	$txtHwnd = GUICtrlRead(GUICtrlRead($listview))
	if $txtHwnd <> "" Then ;MsgBox(0, "listview item", $txtHwnd, 2)
		$txtHwnd2 = StringTrimRight($txtHwnd, StringLen($txtHwnd) - StringInStr ($txtHwnd, "|")+1)
		;MsgBox(0, "listview item", $txtHwnd2, 2)
		$L2Hwnd = HWnd($txtHwnd2)
		;MsgBox(0, "listview item", $L2Hwnd, 2)
		GUIDelete($QuitBoxID)
		if InitHPCPMP() = -1 Then MsgBox(0, "Result", "HP CP MP bar not detected")

		if $CPDetected Then $CP=GetHPCPMP($yCP, $CP_COLOR)
		if $HPDetected Then $HP=GetHPCPMP($yHP, $HP_COLOR)
		if $MPDetected Then $MP=GetHPCPMP($yMP, $MP_COLOR)
		CPHPMPBarUpdate()

	Else
		MsgBox(0, "Error", "Select L2 Client", 2)
	Endif
EndFunc   ;==>OnCancel


Func CPHPMPBarUpdate() ; Draw HPCPMP at script window


	if $CPDetected Then

		GUICtrlSetPos($CPBar, $guiYPos, 15+11*18+$guiXPos, $CP+1, 10)
		GuiCtrlSetBkColor($CPBar, $CP_COLOR)

	Else
		GUICtrlSetPos($CPBar, $guiYPos, 15+11*18+$guiXPos, 101, 10)
		GuiCtrlSetBkColor($CPBar, 0x101010)
	EndIf

	if $HPDetected Then
		GUICtrlSetPos($HPBar, $guiYPos, 15+11*18+10+$guiXPos, $HP+1, 10)
		GuiCtrlSetBkColor($HPBar, $HP_COLOR)
	Else
		GUICtrlSetPos($HPBar, $guiYPos, 15+11*18+10+$guiXPos, 101, 10)
		GuiCtrlSetBkColor($HPBar, 0x101010)
	EndIf

	if $MPDetected Then
		GUICtrlSetPos($MPBar, $guiYPos, 15+11*18+20+$guiXPos, $MP+1, 10)
		GuiCtrlSetBkColor($MPBar, $MP_COLOR)
	Else
		GUICtrlSetPos($MPBar, $guiYPos, 15+11*18+20+$guiXPos, 101, 10)
		GuiCtrlSetBkColor($MPBar, 0x101010)
	EndIf


	if $MobHPDetected Then
		;MsgBox(4096,"",StringFormat("#1 start len proc: %d %d %d", $MobBarStart, $MobBarLen, $MobHP ))
		GUICtrlSetPos($MobHPBar, $guiYPos, 15+16*18+$guiXPos, $MobHP+1, 5)
		GuiCtrlSetBkColor($MobHPBar, $HP_COLOR)
	Else
		GUICtrlSetPos($MobHPBar, $guiYPos, 15+16*18+$guiXPos, 101, 5)
		GuiCtrlSetBkColor($MobHPBar, 0x101010)
	EndIf



	GuiCtrlSetData($MobBarCheckTimeID, "Parse, ms:" & $MobBarCheckTime)
	GuiCtrlSetData($BarCheckTimeID, "Parse, ms:" & $BarCheckTime)

EndFunc

Func PercentMask($PercVal)
   if(($PercVal <0) Or (($PercVal >100) And ($PercVal <>0xFF))) Then Return 0xFF
   Return $PercVal
EndFunc


Func OnUpdateSet() ; Update settings from interface window
	Local $ServerNewIP, $CheckState, $n
	$IsHPConditions=0
	$IsCPConditions=0
	$IsMPConditions=0

	For $i = 1 To $MAXCONDITIONS
		;Fx Target Player Box
		$TargetNic[$i] = GUICtrlRead($TargetNicID[$i])
		;Fx Spam Button Box
		$Buttons[$i] = GUICtrlRead($ButtonsID[$i])
		;Fx CoolDown Box
		$FCoolDownSet[$i] = GUICtrlRead($FCoolDown[$i])
		;Fx Pause Box
		$FPauseSet[$i] = GUICtrlRead($FPause[$i])

		;Fx CoolDown Box
		$FConditionIdleSet[$i] = GUICtrlRead($FConditionIdle[$i])

		 ;$ToggleGroup[$i] = GUICtrlRead($ToggleGroupID[$i])
		;UpdateToggleList()

		;if BitAND(GUICtrlRead($ToggleStateConditionID[$i]), $GUI_CHECKED)  Then
		;	$ToggleStateCondition[$i] = 1
		;Else
		;	$ToggleStateCondition[$i] = 0
		;Endif

		;if $i < 5 Then MsgBox(4096,"","$i " & $i & " State: " &  GUICtrlRead($ToggleStateID[$i]) & "No " & $CheckState)

		;if BitAND(GUICtrlRead($ToggleStateID[$i]), $GUI_CHECKED) Then  ;Mod(GUICtrlRead($ToggleStateID[$i]), 256)= $GUI_CHECKED
		;	$CheckState = 1
			;MsgBox(4096,"","No " &  GUICtrlRead($ToggleStateID[$i]) & "No " & $CheckState)
		;Else
		;	$CheckState = 0
			;MsgBox(4096,"","No " &  GUICtrlRead($ToggleStateID[$i]) & "No " & $CheckState)
		;EndIf

;		if $CheckState <> -1 Then
		;$n = FindToggleGroupByName( $ToggleGroup[$i] )
		;if $n > 0 Then
		;	if $ToggleGroupState[$n] <> $CheckState Then
		;		ToggleToggleGroup($ToggleGroup[$i]) ;
		;		ExitLoop
		;	EndIf
		;EndIf
;		Endif

		;Fx Conditions Box
		$CheckNic[$i] = GUICtrlRead($CheckNicID[$i])
		$FHPMinSet[$i] =PercentMask(GUICtrlRead($FHPMin[$i]))
		$IsHPConditions += $FHPMinSet[$i]*$FSet[$i]

		$FHPMaxSet[$i] = PercentMask(GUICtrlRead($FHPMax[$i]))
		$IsHPConditions += $FHPMaxSet[$i]*$FSet[$i]

		$FCPMinSet[$i] = PercentMask(GUICtrlRead($FCPMin[$i]))
		$IsCPConditions += $FCPMinSet[$i]*$FSet[$i]

		$FCPMaxSet[$i] = PercentMask(GUICtrlRead($FCPMax[$i]))
		$IsCPConditions += $FCPMaxSet[$i]*$FSet[$i]

		$FMPMinSet[$i] = PercentMask(GUICtrlRead($FMPMin[$i]))
		$IsMPConditions += $FMPMinSet[$i]*$FSet[$i]

		$FMPMaxSet[$i] = PercentMask(GUICtrlRead($FMPMax[$i]))
		$IsMPConditions += $FMPMaxSet[$i]*$FSet[$i]

		$FMobHPMinSet[$i] = PercentMask(GUICtrlRead($FMobHPMin[$i]))
		$IsMobHPConditions += $FMobHPMinSet[$i]*$FSet[$i]

		$FMobHPMaxSet[$i] = PercentMask(GUICtrlRead($FMobHPMax[$i]))
		$IsMobHPConditions += $FMobHPMaxSet[$i]*$FSet[$i]

		GUICtrlSetDataCondition($FHPMin[$i], $FHPMinSet[$i])
		GUICtrlSetDataCondition($FHPMax[$i], $FHPMaxSet[$i])
		GUICtrlSetDataCondition($FCPMin[$i], $FCPMinSet[$i])
		GUICtrlSetDataCondition($FCPMax[$i], $FCPMaxSet[$i])
		GUICtrlSetDataCondition($FMPMin[$i], $FMPMinSet[$i])
		GUICtrlSetDataCondition($FMPMax[$i], $FMPMaxSet[$i])
		GUICtrlSetDataCondition($FMobHPMin[$i], $FMobHPMinSet[$i])
		GUICtrlSetDataCondition($FMobHPMax[$i], $FMobHPMaxSet[$i])
		;Fx CheckBox
		if BitAND(GUICtrlRead($F[$i]), $GUI_CHECKED) Then
		    if $FSet[$i] == 0 Then
			   $FSet[$i] = 1
			   ConsoleWrite(@LF &'Add Key: '& $Buttons[$i] & ' ' &  $FCoolDownSet[$i] & ' ' &$FConditionIdleSet[$i]&@LF)
			   $DongleConnected = SendCMD_ADD_NODE($Buttons[$i],  $FPauseSet[$i]*1000, $FCoolDownSet[$i]*1000, $FConditionIdleSet[$i]*1000) ;
			   $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_MobHP, $FMobHPMinSet[$i], $FMobHPMaxSet[$i])
			   $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_playerHP, $FHPMinSet[$i], $FHPMaxSet[$i])
			   $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_playerCP, $FCPMinSet[$i], $FCPMaxSet[$i])
			   $DongleConnected = SendCMD_ADD_NODE_CONDITION($Buttons[$i], $t_playerMP, $FMPMinSet[$i], $FMPMaxSet[$i])
			EndIf
		Else
		    if $FSet[$i] == 1 Then
			   $FSet[$i] = 0
			   ConsoleWrite(@LF &'Delete Key: '& $Buttons[$i] & @LF)
			   $DongleConnected = SendCMD_DELETE_NODE($Buttons[$i]) ;
			EndIf
		Endif
	  Next

	if $IsHPConditions > 0 Then $IsHPConditions =1
	if $IsCPConditions > 0 Then
		$IsCPConditions =1
		;MsgBox(4096,"",$IsCPConditions)
	EndIf
	if $IsMPConditions > 0 Then $IsMPConditions =1
	if $IsMobHPConditions > 0 Then $IsMobHPConditions =1

	if BitAND(GUICtrlRead($Walk), $GUI_CHECKED)Then
		$WalkSet = 1
	Else
		$WalkSet = 0
	Endif
	;if $WalkSet <> 0 Then  MsgBox(4096,"","Walk#1")
	$WalkCoolDownSet = GUICtrlRead($WalkCoolDown)
	$NicName = GUICtrlRead($NicNameID)
	GUICtrlSetData($LabelID[10],$NicName )
	WinSetTitle($hWndMain, "", $NicName)
	$ServerNewIP=GUICtrlRead($ServerID)
	if $ServerIP <> $ServerNewIP Then
		UDPCloseSocket ($SndSocket)
		$ServerIP =  $ServerNewIP
		$SndSocket = UDPOpen($ServerIP, 65533)
		If @error <> 0 Then MsgBox(4096,"","UDP Port Open Failed")
		EndIf

   OnSetModifier()
	;if $WalkSet <> 0 Then  MsgBox(4096,"","Walk#2")



EndFunc

Func OnSetModifier()
      $ShiftState = BitAND(GUICtrlRead($ShiftStateID), $GUI_CHECKED)
	  $CtrlState = BitAND(GUICtrlRead($CtrlStateID), $GUI_CHECKED)
      $DongleConnected = SendCMD_SET_MODIFIER( BitOR($ShiftState*$HID_KEYBOARD_MODIFIER_LEFTSHIFT, $CtrlState*$HID_KEYBOARD_MODIFIER_LEFTCTRL)    )
EndFunc

Func UpdateSet($GroupName, $GroupState) ; Update settings to interface window
	Local $ServerNewIP, $CheckState, $n, $i
	;if $WalkSet <> 0 Then  MsgBox(4096,"","Walk#3")
	For $i = 1 To $MAXCONDITIONS


		if $ToggleGroup[$i] = $GroupName Then
					if $GroupState <> 0 Then
						GuiCtrlSetState($ToggleStateID[$i], $GUI_CHECKED)
					Else
						GuiCtrlSetState($ToggleStateID[$i], $GUI_UNCHECKED)
					EndIf
		EndIf

	Next

EndFunc


Func SendStatusToServer()
	if $UDPSendTimer=0 Then $UDPSendTimer = TimerInit()
	if TimerDiff($UDPSendTimer) > 1000 Then
		$UDPSendTimer = TimerInit()
		; Command, IP, Port, hWnd, NicName, HP, MP,CP, MobHP
		$status = UDPSend($SndSocket, "INIT"& " " & $RcvSocket[2] & " " & $RcvSocket[3] & " " & $L2Hwnd & " " & $NicName & " " & $HP & " " & $MP & " " & $CP & " " &  $MobHP )
		If $status = 0 then
			MsgBox(0, "ERROR", "Error while sending UDP message: " & @error)
			Exit
		EndIf

		$status = UDPSend($SrvSocket, "Command=INIT"& "&IP=" & $RcvSocket[2] & "&Port=" & $RcvSocket[3] & "&hWnd=" & $L2Hwnd & "&NicName=" & $NicName & "&HP=" & $HP & "&MP=" & $MP & "&CP=" & $CP & "&MobHP=" &  $MobHP & "&Time=3984753")
		If $status = 0 then
			MsgBox(0, "ERROR", "Error while sending UDP message: " & @error)
			Exit
		EndIf

	EndIf
EndFunc

#cs
Func UpdateToggleList()
	Local $i, $n

	for $n=1 To $MAXGROUPS
		if IsToggleGroupInUse($ToggleGroupName[$n]) = -1 Then $ToggleGroupName[$n] = ""
	Next


	For $i = 1 To $MAXCONDITIONS
		$n = FindToggleGroupByName($ToggleGroup[$i])

		if $n = -1 Then
			$n = FindFreeToggleGroupSlot()
			if $n <> -1 Then $ToggleGroupName[$n] = $ToggleGroup[$i]
		EndIf
	Next
EndFunc


Func IsToggleGroupInUse($Name, $start=1)
Local $n
	if ($Name = "") Then  Return -1
	for $n=$start To $MAXCONDITIONS
		if $ToggleGroup[$n] = $Name Then Return $n
	Next
	Return -1
EndFunc

Func InitToggleList()
	Local $i, $n, $j
	For $n = 1 To $MAXGROUPS
		; $ToggleGroupName[$n]$ToggleGroupCoolDown[$n] $ToggleGroupState[$n]
		$ToggleGroupName[$n] = ""
		$ToggleGroupState[$n] = 0

	Next
EndFunc


Func FindToggleGroupByName($Name)
Local $n
	if ($Name = "") Then  Return -1
	for $n=1 To $MAXGROUPS
		if $ToggleGroupName[$n] = $Name Then Return $n
	Next
	Return -1
EndFunc


Func FindFreeToggleGroupSlot()
Local $n
	for $n=1 To $MAXGROUPS
		if $ToggleGroupName[$n] = "" Then Return $n
	Next
	Return -1
EndFunc

Func ToggleToggleGroup($Name)
Local $j
	if $Name <> "" Then
		$j = FindToggleGroupByName( $Name )
		if($j >0) Then
			$ToggleGroupState[$j] = Abs($ToggleGroupState[$j] -1)
			UpdateSet($Name, $ToggleGroupState[$j])
		Endif
	EndIf
EndFunc
#ce

Func FindPlayerByNic($Nic)
Local $n
; Timestump, IP, Port, hWnd, NicName, HP, MP,CP
	 UpdatePlayerList()
	 if $Nic = $NicName Or  $Nic = "" Then  Return 0
	for $n=1 To $MAXPLAYERS
		if $Players[$n][0] <> 0 Then
			if $Players[$n][4] = $Nic	Then Return $n
		EndIf
	Next
	Return -1
EndFunc

Func FindFreePlayerSlot()
Local $n
; Timestump, IP, Port, hWnd, NicName, HP, MP,CP
	 UpdatePlayerList()
	for $n=1 To $MAXPLAYERS
		if $Players[$n][0] = 0 Then Return $n
	Next
	Return -1
EndFunc


Func UpdatePlayerList()
Local $n, $ListString
; Timestump, IP, Port, hWnd, NicName, HP, MP,CP
	for $n=0 To $MAXPLAYERS
		if $Players[$n][0] <> 0 Then
			if TimerDiff($Players[$n][0]) > $PLAYERTIMEOUT Then
				$Players[$n][0] = 0		; Timestump
				$Players[$n][1] = ""	; IP
				$Players[$n][2] = ""	; Port
				$Players[$n][3] = 0		; hWnd
				$Players[$n][4] = ""	; NicName
				$Players[$n][5] = 100	; HP
				$Players[$n][6] = 100	; MP
				$Players[$n][7] = 100	; CP
				$Players[$n][8] = 0		; Mob HP
				if $Players[$n][9] <> 0 Then
					GUICtrlDelete($Players[$n][9])
					$Players[$n][9] = 0
				EndIf

			Else
				$ListString = $Players[$n][1] & "|" & $Players[$n][2] & "|" & $Players[$n][4]& "|"  & $Players[$n][5]& "|" & $Players[$n][6]& "|"  & $Players[$n][7]& "|"  & $Players[$n][8]& "|"  &Round(TimerDiff($Players[$n][0])) & "|"

				if $Players[$n][9] = 0 Then
					$Players[$n][9] = GuiCtrlCreateListViewItem($ListString, $PlayersListID)
				Else
					GUICtrlSetData($Players[$n][9], $ListString)
				Endif
			EndIf
		EndIf
	Next

EndFunc

Func InitPlayerList()
Local $n
; Timestump, IP, Port, hWnd, NicName, HP, MP,CP
	for $n=1 To $MAXPLAYERS
				$Players[$n][0] = 0		; Timestump
				$Players[$n][1] = ""	; IP
				$Players[$n][2] = ""	; Port
				$Players[$n][3] = 0		; hWnd
				$Players[$n][4] = ""	; NicName
				$Players[$n][5] = 100	; HP
				$Players[$n][6] = 100	; MP
				$Players[$n][7] = 100	; CP
				$Players[$n][8] = 100	; MobHP
	Next
EndFunc


Func DoLocalAction($Fx, $player, $toggle) ; Execute Condition N, Player N, ToggleGroup N
Local $MainString = "MR 100%, 100%; MR 0%, 0%; mD 200,300, 70%, 70%; P 3000; MR 50%, 50%", $i, $command, $button, $y, $IsPercent = 0
Dim $StrCommads[20], $coords[6], $CursorInfo[5], $size[3]
$MainString = StringStripWS($Buttons[$Fx], 8)
$MainString = StringUpper ( $MainString )

$StrCommads =  StringSplit($MainString, ';', 1)

for $i=1 To $StrCommads[0]

	Switch StringLeft($StrCommads[$i], 1)
		Case "{"
			;ConsoleWrite($StrCommads[$i])
			;MsgBox(0,"", @LF & '$Key: ' & $StrCommads[$i] & @LF)
			SendKey2HID($HID_KEYBOARD_MODIFIER_NONE, $StrCommads[$i])
			;Send($StrCommads[$i])
		Case "P"
			$command = StringTrimLeft ($StrCommads[$i], 1)
			if StringIsDigit($command) Then
				Sleep($command)
			Else
				MsgBox(0, "Error1", "Error: "& $StrCommads[$i])
				Return 0
			EndIf
		Case "M"
			$button = StringLeft( StringTrimLeft ($StrCommads[$i], 1),1)
			$command = StringTrimLeft ($StrCommads[$i], 2)
			$coords = StringSplit($command, ',', 1)
			for $y = 1 To $coords[0]
				if StringRight($coords[$y], 1) = "%" Then
					$coords[$y] = StringTrimRight($coords[$y], 1)
					$IsPercent = 1
				EndIf
				if StringIsDigit($coords[$y]) Then
					$CursorInfo = WinGetPos ($L2Hwnd)
					$size = WinGetClientSize($L2Hwnd)
					$CursorInfo[0] += ($CursorInfo[2]- $size[0])/2
					$CursorInfo[1] += ($CursorInfo[3]- $size[1])/2
					$CursorInfo[2] = $size[0]
					$CursorInfo[3] = $size[1]

					if $IsPercent Then
						$coords[$y] = $CursorInfo[Mod($y, 2)]+$coords[$y]*$CursorInfo[2+Mod($y, 2)]/100
					Else
						$coords[$y] = $CursorInfo[0+Mod($y, 2)]+$coords[$y]
					EndIf
					if $coords[$y] > $CursorInfo[Mod($y, 2)]+$CursorInfo[2+Mod($y, 2)] Then
						$coords[$y] = $CursorInfo[Mod($y, 2)]+$CursorInfo[2+Mod($y, 2)]
					EndIf
					;MsgBox(0, "Stripped from both ends", $coords[$y])
				Else
					MsgBox(0, "Error2", "Error: "& $StrCommads[$i])
					Return 0
				EndIf
			Next
			Switch $button
				Case "L"
					if  $coords[0] <> 2 Then
						MsgBox(0, "Error4", "Error: "& $StrCommads[$i]&" Arg="&$coords[0]&" Btn="&$button)
						Return 0
					Endif
					MouseClick("left", $coords[2], $coords[1])
				Case "R"
					if  $coords[0] <> 2 Then
						MsgBox(0, "Error4", "Error: "& $StrCommads[$i]&" Arg="&$coords[0]&" Btn="&$button)
						Return 0
					Endif
					MouseClick("right", $coords[2], $coords[1])
				Case "M"
					if  $coords[0] <> 2 Then
						MsgBox(0, "Error4", "Error: "& $StrCommads[$i]&" Arg="&$coords[0]&" Btn="&$button)
						Return 0
					Endif
					MouseClick("middle", $coords[2], $coords[1])
				Case "D"
					if  $coords[0] <> 4 Then
						MsgBox(0, "Error4", "Error: "& $StrCommads[$i]&" Arg="&$coords[0]&" Btn="&$button)
						Return 0
					Endif
					MouseClickDrag("left", $coords[2], $coords[1], $coords[4], $coords[3])
				Case Else
					MsgBox(0, "Error3", "Error: "& $StrCommads[$i])
					Return 0
			EndSwitch
		Case Else
			MsgBox(0, "Error5", "Error: "& $StrCommads[$i])
			Return 0
	EndSwitch
Next


	Return 1
EndFunc


Func DoRemoteAction($Fx, $player, $toggle) ; Execute Condition N, Player N, ToggleGroup N
	Dim  $ClientSocket[4]

$ClientSocket = UDPOpen($Players[$player][1], $Players[$player][2])
	If @error <> 0 Then Return 0

	$status = UDPSend($ClientSocket, "COMMAND"& " " & $Buttons[$Fx])
	If $status = 0 then Return 0

	UDPCloseSocket ($ClientSocket)
	Return 1
EndFunc

Func DoAction($Fx, $player, $toggle) ; ; Execute Condition N, Player N, ToggleGroup N,  start timers, toggle group

	Switch  $player
	Case 0
		if ($IsMainWindow And WinActive(HWnd($Players[$player][3])) = 0 ) Then WinActivate($L2Hwnd)
		;ConsoleWrite('DoAction start')
		;if WinWaitActive($L2Hwnd, "", 15) <> 0 Then
			;ConsoleWrite('DoAction 0')
			if DoCustomLocalAction($Fx, $player, $toggle) = 1 Then
				if $FPauseSet[$Fx] <> 0 Then
					$PauseTime = $FPauseSet[$Fx]*1000
					$PauseTimer = TimerInit()
				EndIf
				ToggleToggleGroup($ToggleGroup[$Fx] )

				$FTimer[$Fx] = TimerInit()
			Endif
		;Endif
	Case -1
	Case Else
		; ControlID, IP, Port, hWnd, NicName, HP, MP,CP, Timestump
		if  WinExists(HWnd($Players[$player][3])) Then
			if WinActive(HWnd($Players[$player][3])) = 0 Then WinActivate(HWnd($Players[$player][3]))
		;	if WinWaitActive(HWnd($Players[$player][3]), "", 15) <> 0 Then
				;ConsoleWrite('DoAction Else')
				if DoCustomLocalAction($Fx, $player, $toggle)  = 1 Then
					if $FPauseSet[$Fx] <> 0 Then
						if $PauseTimer <> 0 Then
							if $PauseTime - TimerDiff($PauseTimer) < $FPauseSet[$Fx]*1000 Then
								$PauseTime = $FPauseSet[$Fx]*1000
								$PauseTimer = TimerInit()
							Endif
						Else
							$PauseTime = $FPauseSet[$Fx]*1000
							$PauseTimer = TimerInit()
						Endif
					EndIf
					$FTimer[$Fx] = TimerInit()
				Endif
		;	Endif
		Else
			if DoCustomRemoteAction($Fx, $player, $toggle)  = 1 Then
				if $FPauseSet[$Fx] <> 0 Then
					$PauseTime = $FPauseSet[$Fx]*1000
					$PauseTimer = TimerInit()
				EndIf
				ToggleToggleGroup($ToggleGroup[$Fx] )

				$FTimer[$Fx] = TimerInit()
			Endif
		Endif

	EndSwitch

EndFunc


Func CheckCondition($Fx, $player, $toggle) ; Check push button conditions
	Local $IsCondition =1, $checkPlayer

	if $FSet[$Fx] = 0 Then Return 0

	if $PauseTimer <>0 And $player = 0 Then
		if $PauseTime <>0 Then
			if TimerDiff($PauseTimer) < $PauseTime Then
				Return 0
			Else
				$PauseTimer = 0
				$PauseTime = 0
			EndIf
		Else
			$PauseTimer = 0
		EndIf
	Endif

	if $FTimer[$Fx] <> 0 Then
		if TimerDiff($FTimer[$Fx]) < $FCoolDownSet[$Fx] *1000 Then Return 0
		EndIf

	if $toggle <> -1 Then
		if $ToggleStateCondition[$Fx] <> $ToggleGroupState[$toggle] Then Return 0
	EndIf

	$checkPlayer = FindPlayerByNic($CheckNic [$Fx])
	Switch  $checkPlayer
		Case 0
			if($HPDetected) Then
				if($FHPMinSet[$Fx] <>0) AND $HP < $FHPMinSet[$Fx] Then $IsCondition =0
				if($FHPMaxSet[$Fx] <>0) AND $HP > $FHPMaxSet[$Fx] Then $IsCondition =0
			EndIf

			if($CPDetected) Then
				if($FCPMinSet[$Fx] <>0) AND $CP < $FCPMinSet[$Fx] Then $IsCondition =0
				if($FCPMaxSet[$Fx] <>0) AND $CP > $FCPMaxSet[$Fx] Then $IsCondition =0
			EndIf

			if($MPDetected) Then
				if($FMPMinSet[$Fx] <>0) AND $MP < $FMPMinSet[$Fx] Then $IsCondition =0
				if($FMPMaxSet[$Fx] <>0) AND $MP > $FMPMaxSet[$Fx] Then $IsCondition =0
			EndIf

			if($MobHPDetected) Then
				if($FMobHPMinSet[$Fx] <>0) AND $MobHP < $FMobHPMinSet[$Fx] Then $IsCondition =0
				if($FMobHPMaxSet[$Fx] <>0) AND $MobHP > $FMobHPMaxSet[$Fx] Then $IsCondition =0
			EndIf

		Case -1
			Return 0
		Case Else
			if($FHPMinSet[$Fx] <>0) AND $Players[$checkPlayer][5] < $FHPMinSet[$Fx] Then $IsCondition =0
			if($FHPMaxSet[$Fx] <>0) AND  $Players[$checkPlayer][5] > $FHPMaxSet[$Fx] Then $IsCondition =0

			if($FCPMinSet[$Fx] <>0) AND  $Players[$checkPlayer][7] < $FCPMinSet[$Fx] Then $IsCondition =0
			if($FCPMaxSet[$Fx] <>0) AND  $Players[$checkPlayer][7] > $FCPMaxSet[$Fx] Then $IsCondition =0

			if($FMPMinSet[$Fx] <>0) AND $Players[$checkPlayer][6] < $FMPMinSet[$Fx] Then $IsCondition =0
			if($FMPMaxSet[$Fx] <>0) AND  $Players[$checkPlayer][6] > $FMPMaxSet[$Fx] Then $IsCondition =0

			if($FMobHPMinSet[$Fx] <>0) AND $Players[$checkPlayer][5] < $FMobHPMinSet[$Fx] Then $IsCondition =0
			if($FMobHPMaxSet[$Fx] <>0) AND  $Players[$checkPlayer][5] > $FMobHPMaxSet[$Fx] Then $IsCondition =0


	EndSwitch
	Return $IsCondition
EndFunc



Func CheckCustomCondition($Fx, $player, $toggle) ; Check push button conditions
	; Add custom condition. Return 0 or 1.

	return CheckCondition($Fx, $player, $toggle) ; Check push button interface defined conditions

EndFunc

Func DoCustomLocalAction($Fx, $player, $toggle) ; Execute  Condition N, Player N, ToggleGroup N
; Add custom actions. Return 1 to start interface defined timers and toggle group.
	Return DoLocalAction($Fx, $player, $toggle)
EndFunc


Func DoCustomRemoteAction($Fx, $player, $toggle) ; Execute CCondition N, Player N, ToggleGroup N
; Add custom actions. Return 1 to start interface defined timers and toggle group.

	Return DoRemoteAction($Fx, $player, $toggle)
EndFunc
